#include "TopCPToolkit/ParticleLevelAlg.h"

#include "FourMomUtils/xAODP4Helpers.h"
#include "TopCPToolkit/Tokenize.h"

#ifdef XAOD_STANDALONE
// access to tds in non-athena release
#define TDS() evtStore()->tds()
#else
// access to tds in athena release
#define TDS() evtStore()
#endif

namespace top {
  ParticleLevelAlg::ParticleLevelAlg(const std::string& name,
				     ISvcLocator* pSvcLocator)
    : EL::AnaAlgorithm(name, pSvcLocator),
      m_useTruthElectrons(true),
      m_useTruthMuons(true),
      m_useTruthPhotons(false),
      m_useTruthTaus(false),
      m_useTruthJets(true),
      m_useTruthLargeRJets(false),
      m_useTruthNeutrinos(false),
      m_useTruthMET(true),
      m_doOverlapRemoval(true),
      m_el_ptMin(25e3),
      m_el_etaMax(2.5),
      m_el_notFromHadron(true),
      m_el_tauIsHadron(false),
      m_mu_ptMin(25e3),
      m_mu_etaMax(2.5),
      m_mu_notFromHadron(true),
      m_mu_tauIsHadron(false),
      m_ph_ptMin(25e3),
      m_ph_etaMax(2.5),
      m_ph_origin(""),
      m_ph_isolation(""),
      m_tau_ptMin(25e3),
      m_tau_etaMax(2.5),
      m_jet_ptMin(25e3),
      m_jet_etaMax(2.5),
      m_ljet_ptMin(25e3),
      m_ljet_etaMax(2.5),
      m_ljet_collection("AntiKt10TruthTrimmedPtFrac5SmallR20Jets"),
      m_nu_ptMin(0.),
      m_nu_etaMax(10.),
      photon_isolationCut(0)
  {
    declareProperty("useTruthElectrons", m_useTruthElectrons,
		    "config: use electrons?");
    declareProperty("useTruthMuons", m_useTruthMuons, "config: use muons?");
    declareProperty("useTruthPhotons", m_useTruthPhotons, "config: use photons?");
    declareProperty("useTruthTaus", m_useTruthTaus, "config: use taus?");
    declareProperty("useTruthJets", m_useTruthJets, "config: use jets?");
    declareProperty("useTruthLargeRJets", m_useTruthLargeRJets,
		    "config: use large-R jets?");
    declareProperty("useTruthNeutrinos", m_useTruthNeutrinos,
		    "config: use neutrinos?");
    declareProperty("useTruthMET", m_useTruthMET, "config: use MET?");
    declareProperty("doOverlapRemoval", m_doOverlapRemoval,
		    "config: perform default overlap removal?");
    declareProperty("el_ptMin", m_el_ptMin, "electrons: minimum pT [MeV]");
    declareProperty("el_etaMax", m_el_etaMax, "electrons: maximum abs(eta)");
    declareProperty("el_notFromHadron", m_el_notFromHadron,
		    "electrons: reject if originating from hadronic decay?");
    declareProperty("el_tauIsHadron", m_el_tauIsHadron,
		    "electrons: consider tauons as hadrons?");
    declareProperty("mu_ptMin", m_mu_ptMin, "muons: minimum pT [MeV]");
    declareProperty("mu_etaMax", m_mu_etaMax, "muons: maximum abs(eta)");
    declareProperty("mu_notFromHadron", m_mu_notFromHadron,
		    "muons: reject if originating from hadronic decay?");
    declareProperty("mu_tauIsHadron", m_mu_tauIsHadron,
		    "muons: consider tauons as hadrons?");
    declareProperty("ph_ptMin", m_ph_ptMin, "photons: minimum pT [MeV]");
    declareProperty("ph_etaMax", m_ph_etaMax, "photons: maximum abs(eta)");
    declareProperty("ph_origin", m_ph_origin,
		    "photons: comma-separated list of possible origins");
    declareProperty(
		    "ph_isolation", m_ph_isolation,
		    "photons: space-separated isolation variable and isolation cut");
    declareProperty("tau_ptMin", m_tau_ptMin, "taus: minimum pT [MeV]");
    declareProperty("tau_etaMax", m_tau_etaMax, "taus: maximum abs(eta)");
    declareProperty("jet_ptMin", m_jet_ptMin, "jets: minimum pT [MeV]");
    declareProperty("jet_etaMax", m_jet_etaMax, "jets: maximum abs(eta)");
    declareProperty("ljet_ptMin", m_ljet_ptMin, "large-R jets: minimum pT [MeV]");
    declareProperty("ljet_etaMax", m_ljet_etaMax,
		    "large-R jets: maximum abs(eta)");
    declareProperty("ljet_collection", m_ljet_collection, "large-R jets: collection name");
    declareProperty("nu_ptMin", m_nu_ptMin, "neutrinos: minimum pT [MeV]");
    declareProperty("nu_etaMax", m_nu_etaMax, "neutrinos: maximum abs(eta)");
  }

  StatusCode ParticleLevelAlg::initialize() {
    ANA_MSG_INFO("Initializing ParticleLevel " << name());

    // parse the photon origin string
    if (m_ph_origin == "" || m_ph_origin == " " || m_ph_origin == "False" ||
	m_ph_origin == "None") {
      // do nothing
    } else {
      // This allows us to convert from string name to enum value.
      MCTruthPartClassifier::ParticleDef def;

      // Tokenize at comma.
      std::vector<std::string> tokens;
      tokenize(m_ph_origin, tokens, ",");

      while (tokens.size()) {
	const auto& token = tokens.back();

	auto it = std::find(def.sParticleOrigin.begin(),
			    def.sParticleOrigin.end(), token);

	if (it == def.sParticleOrigin.end()) {
	  ANA_MSG_ERROR("Invalid particle-level photon origin: " + token);
	}

	photon_origins.push_back(
				 static_cast<MCTruthPartClassifier::ParticleOrigin>(
										    std::distance(def.sParticleOrigin.begin(), it)));
	tokens.pop_back();
      }
    }

    // parse the photon isolation string
    if (m_ph_isolation == "" || m_ph_isolation == " " ||
	m_ph_isolation == "False" || m_ph_isolation == "None") {
      photon_isolationVar = "NoIsolation";  // accessor cannot be empty!
    } else {
      // Split at space, should be exactly 2 tokens.
      std::vector<std::string> tokens;
      tokenize(m_ph_isolation, tokens, " ");
      if (tokens.size() != 2) {
	ANA_MSG_ERROR(
		      "Invalid particle-level photon isolation: expected 2 tokens.");
      }

      photon_isolationVar = tokens.at(0);
      photon_isolationCut = std::stof(tokens.at(1));
    }

    return StatusCode::SUCCESS;
  }

  StatusCode ParticleLevelAlg::execute() {

    // retrieve the truth containers
    if (m_useTruthElectrons)
      ANA_CHECK(evtStore()->retrieve(inputElectrons, "TruthElectrons"));
    if (m_useTruthMuons)
      ANA_CHECK(evtStore()->retrieve(inputMuons, "TruthMuons"));
    if (m_useTruthPhotons)
      ANA_CHECK(evtStore()->retrieve(inputPhotons, "TruthPhotons"));
    if (m_useTruthTaus)
      ANA_CHECK(evtStore()->retrieve(inputTaus, "TruthTaus"));
    if (m_useTruthJets){
      ANA_CHECK(evtStore()->retrieve(inputJets, "AntiKt4TruthDressedWZJets"));
      ANA_CHECK(evtStore()->retrieve(evtInfo,"EventInfo"));
    }
    if (m_useTruthLargeRJets)
      ANA_CHECK(evtStore()->retrieve(inputLargeRJets, m_ljet_collection));
    if (m_useTruthNeutrinos)
      ANA_CHECK(evtStore()->retrieve(inputNeutrinos, "TruthNeutrinos"));
    if (m_useTruthMET)
      ANA_CHECK(evtStore()->retrieve(inputMissingET, "MET_Truth"));

    // accessors
    static const SG::AuxElement::Accessor<float> acc_pt_dressed("pt_dressed");
    static const SG::AuxElement::Accessor<float> acc_eta_dressed("eta_dressed");
    static const SG::AuxElement::Accessor<unsigned int> acc_particle_type(
									  "classifierParticleType");
    static const SG::AuxElement::Accessor<float> acc_photon_isolation(
								      photon_isolationVar);

    // decorators
    static const SG::AuxElement::Decorator<float> dec_particle_charge("charge");
    static const SG::AuxElement::Decorator<float> dec_particle_pt("pt");
    static const SG::AuxElement::Decorator<float> dec_particle_eta("eta");
    static const SG::AuxElement::Decorator<float> dec_particle_phi("phi");
    static const SG::AuxElement::Decorator<float> dec_particle_energy("e");
    static const SG::AuxElement::Decorator<int> dec_num_truth_bjets_nocuts("num_truth_bjets_nocuts");
    static const SG::AuxElement::Decorator<int> dec_num_truth_cjets_nocuts("num_truth_cjets_nocuts");

    // indices of selected objects
    std::list<std::size_t> idx_electrons;
    std::list<std::size_t> idx_muons;
    std::list<std::size_t> idx_photons;
    std::list<std::size_t> idx_taus;
    std::list<std::size_t> idx_jets;
    std::list<std::size_t> idx_ljets;
    std::list<std::size_t> idx_neutrinos;

    // apply electron selection
    if (m_useTruthElectrons) {
      for (std::size_t i = 0; i < inputElectrons->size(); ++i) {
	const auto* electron = inputElectrons->at(i);

	// pT and eta cuts
	if (acc_pt_dressed(*electron) < m_el_ptMin)
	  continue;
	if (std::abs(acc_eta_dressed(*electron)) > m_el_etaMax)
	  continue;

	// if requested, accept electrons iff they do not come from a hadron
	if (m_el_notFromHadron &&
	    acc_particle_type(*electron) !=
	    MCTruthPartClassifier::ParticleType::IsoElectron)
	  continue;

	// if requested, consider tauons as hadrons and repeat the check
	if (m_el_tauIsHadron) {
	  auto truthProxy =
            electron->auxdata<ElementLink<xAOD::TruthParticleContainer> >(
									  "originalTruthParticle");
	  if (not truthProxy.isValid()) {
	    throw std::runtime_error(
				     "ParticleLevel electron has no 'originalTruthParticle' "
				     "decoration.");
	  }
	  if (ParticleLevel::isLeptonFromTau(*truthProxy))
	    continue;
	}

	// electron is accepted
	idx_electrons.push_back(i);
      }
    }

    // apply muon selection
    if (m_useTruthMuons) {
      for (std::size_t i = 0; i < inputMuons->size(); ++i) {
	const auto* muon = inputMuons->at(i);

	// pT and eta cuts
	if (acc_pt_dressed(*muon) < m_mu_ptMin)
	  continue;
	if (std::abs(acc_eta_dressed(*muon)) > m_mu_etaMax)
	  continue;

	// if requested, accept muons iff they do not come from a hadron
	if (m_mu_notFromHadron &&
	    acc_particle_type(*muon) !=
	    MCTruthPartClassifier::ParticleType::IsoMuon)
	  continue;

	// if requested, consider tauons as hadrons and repeat the check
	if (m_mu_tauIsHadron) {
	  auto truthProxy =
            muon->auxdata<ElementLink<xAOD::TruthParticleContainer> >(
								      "originalTruthParticle");
	  if (not truthProxy.isValid()) {
	    throw std::runtime_error(
				     "ParticleLevel muon has no 'originalTruthParticle' decoration.");
	  }
	  if (ParticleLevel::isLeptonFromTau(*truthProxy))
	    continue;
	}

	// muon is accepted
	idx_muons.push_back(i);
      }
    }

    // apply photon selection
    if (m_useTruthPhotons) {
      for (std::size_t i = 0; i < inputPhotons->size(); i++) {
	const auto* photon = inputPhotons->at(i);

	// check that the photon is stable
	if (photon->status() != 1)
	  continue;

	// pT and eta cuts
	if (photon->pt() < m_ph_ptMin)
	  continue;
	if (std::abs(photon->eta()) > m_ph_etaMax)
	  continue;

	// if requested, apply origin cut
	if (photon_origins.size()) {
	  unsigned int origin = acc_particle_type(*photon);
	  if (std::find(photon_origins.begin(), photon_origins.end(), origin) ==
	      photon_origins.end())
	    continue;
	}

	// if requested, apply isolation selection
	if (photon_isolationVar.size()) {
	  if (acc_photon_isolation.isAvailable(*photon)) {
	    float isolation = acc_photon_isolation(*photon);
	    if (photon_isolationCut <= isolation / photon->pt())
	      continue;
	  }
	}

	// photon is accepted
	idx_photons.push_back(i);
      }
    }

    // apply tau selection
    if (m_useTruthTaus) {
      for (std::size_t i = 0; i < inputTaus->size(); ++i) {
	const auto* tau = inputTaus->at(i);

	// pT and eta cuts
	if (tau->pt() < m_tau_ptMin)
	  continue;
	if (std::abs(tau->eta()) > m_tau_etaMax)
	  continue;

	// accept taus iff they do not come from a hadron
	if (acc_particle_type(*tau) !=
	    MCTruthPartClassifier::ParticleType::IsoTau)
	  continue;

	// tau is accepted
	idx_taus.push_back(i);
      }
    }

    // apply jet selection
    if (m_useTruthJets) {
      int num_truth_bjets_nocuts = 0;
      int num_truth_cjets_nocuts = 0;
      for (std::size_t i = 0; i < inputJets->size(); i++) {
	const auto* jet = inputJets->at(i);

  //get number of truth HF jets before any cuts
  if (jet->isAvailable<int>("HadronConeExclTruthLabelID")){
    int flavourlabel(0);
    jet->getAttribute("HadronConeExclTruthLabelID", flavourlabel);
    if(flavourlabel == 5) {num_truth_bjets_nocuts++;}
    else if(flavourlabel == 4) {num_truth_cjets_nocuts++;}
  } else {
    num_truth_bjets_nocuts = -999; num_truth_cjets_nocuts = -999;
  }

	// pT and eta cuts
	if (jet->pt() < m_jet_ptMin)
	  continue;
	if (std::abs(jet->eta()) > m_jet_etaMax)
	  continue;

	// jet is accepted
	idx_jets.push_back(i);
      }
      //save number of truth HF jets as decoration of EventInfo
      dec_num_truth_bjets_nocuts(*evtInfo) = num_truth_bjets_nocuts;
      dec_num_truth_cjets_nocuts(*evtInfo) = num_truth_cjets_nocuts;
    }

    // apply large-R jet selection
    if (m_useTruthLargeRJets) {
      for (std::size_t i = 0; i < inputLargeRJets->size(); i++) {
	const auto* ljet = inputLargeRJets->at(i);

	// pT and eta cuts
	if (ljet->pt() < m_ljet_ptMin)
	  continue;
	if (std::abs(ljet->eta()) > m_ljet_etaMax)
	  continue;

	// large-R jet is accepted
	idx_ljets.push_back(i);
      }
    }

    // apply neutrino selection
    if (m_useTruthNeutrinos) {
      for (std::size_t i = 0; i < inputNeutrinos->size(); ++i) {
	const auto* neutrino = inputNeutrinos->at(i);

	// pT and eta cuts
	if (neutrino->pt() < m_nu_ptMin)
	  continue;
	if (std::abs(neutrino->eta()) > m_nu_etaMax)
	  continue;

	// neutrino is accepted
	idx_neutrinos.push_back(i);
      }
    }

    // user-friendly MET decorations
    if (m_useTruthMET) {
      for (const xAOD::MissingET* etmiss : *inputMissingET) {
	etmiss->auxdecor<float>("met_met") = etmiss->met();
	etmiss->auxdecor<float>("met_phi") = etmiss->phi();
      }
    }

    // ----------------------
    // OVERLAP REMOVAL
    // ----------------------
    // Removal Steps:
    //   1. Jets & Muons:
    //      Remove Muons with dR < 0.4
    //   2. Jets & Electrons:
    //      Remove Electrons with dR < 0.4
    //   3. Photons & Jets:
    //      Remove Jets with dR < 0.4

    if (m_doOverlapRemoval) {
      // Jets and Muons: Remove Muon with dR < 0.4
      if (m_useTruthMuons && m_useTruthJets) {
	idx_muons.remove_if([&idx_jets, this](std::size_t m) {
	    for (auto j : idx_jets) {
	      if (xAOD::P4Helpers::deltaR(inputJets->at(j), inputMuons->at(m),
					  false) < 0.4) {
		return true;
	      }
	    }
	    return false;
	  });
      }
      // Jets and Electrons: Remove Electron with dR < 0.4
      if (m_useTruthElectrons && m_useTruthJets) {
	idx_electrons.remove_if([&idx_jets, this](std::size_t e) {
	    for (auto j : idx_jets) {
	      if (xAOD::P4Helpers::deltaR(inputJets->at(j), inputElectrons->at(e),
					  false) < 0.4) {
		return true;
	      }
	    }
	    return false;
	  });
      }
      // Photons and Jets: Remove Jet with dR < 0.4
      if (m_useTruthPhotons && m_useTruthJets) {
	idx_jets.remove_if([&idx_photons, this](std::size_t j) {
	    for (auto ph : idx_photons) {
	      if (xAOD::P4Helpers::deltaR(inputPhotons->at(ph), inputJets->at(j),
					  false) < 0.4) {
		return true;
	      }
	    }
	    return false;
	  });
      }
    }

    // write out the electron container after selection
    if (m_useTruthElectrons) {
      outputElectrons = new xAOD::TruthParticleContainer();
      outputElectronsAux = new xAOD::TruthParticleAuxContainer();
      outputElectrons->setStore(outputElectronsAux);
      for (auto e : idx_electrons) {
	const auto& elPtr = inputElectrons->at(e);
	xAOD::TruthParticle* electron = new xAOD::TruthParticle();
	electron->makePrivateStore(*elPtr);

	dec_particle_charge(*electron) = electron->charge();

	outputElectrons->push_back(electron);
      }
      outputSGKey = "ParticleLevelElectrons_NOSYS";
      save = TDS()->record(outputElectrons, outputSGKey);
      saveAux = TDS()->record(outputElectronsAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the muon container after selection
    if (m_useTruthMuons) {
      outputMuons = new xAOD::TruthParticleContainer();
      outputMuonsAux = new xAOD::TruthParticleAuxContainer();
      outputMuons->setStore(outputMuonsAux);
      for (auto m : idx_muons) {
	const auto& muPtr = inputMuons->at(m);
	xAOD::TruthParticle* muon = new xAOD::TruthParticle();
	muon->makePrivateStore(*muPtr);

	dec_particle_charge(*muon) = muon->charge();

	outputMuons->push_back(muon);
      }
      outputSGKey = "ParticleLevelMuons_NOSYS";
      save = TDS()->record(outputMuons, outputSGKey);
      saveAux = TDS()->record(outputMuonsAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the photon container after selection
    if (m_useTruthPhotons) {
      outputPhotons = new xAOD::TruthParticleContainer();
      outputPhotonsAux = new xAOD::TruthParticleAuxContainer();
      outputPhotons->setStore(outputPhotonsAux);
      for (auto p : idx_photons) {
	const auto& phPtr = inputPhotons->at(p);
	xAOD::TruthParticle* photon = new xAOD::TruthParticle();
	photon->makePrivateStore(*phPtr);

	dec_particle_pt(*photon) = photon->pt();
	dec_particle_eta(*photon) = photon->eta();
	dec_particle_phi(*photon) = photon->phi();

	outputPhotons->push_back(photon);
      }
      outputSGKey = "ParticleLevelPhotons_NOSYS";
      save = TDS()->record(outputPhotons, outputSGKey);
      saveAux = TDS()->record(outputPhotonsAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the tau container after selection
    if (m_useTruthTaus) {
      outputTaus = new xAOD::TruthParticleContainer();
      outputTausAux = new xAOD::TruthParticleAuxContainer();
      outputTaus->setStore(outputTausAux);
      for (auto t : idx_taus) {
	const auto& tauPtr = inputTaus->at(t);
	xAOD::TruthParticle* tau = new xAOD::TruthParticle();
	tau->makePrivateStore(*tauPtr);

	dec_particle_pt(*tau) = tau->pt();
	dec_particle_eta(*tau) = tau->eta();
	dec_particle_phi(*tau) = tau->phi();
	dec_particle_charge(*tau) = tau->charge();

	outputTaus->push_back(tau);
      }
      outputSGKey = "ParticleLevelTaus_NOSYS";
      save = TDS()->record(outputTaus, outputSGKey);
      saveAux = TDS()->record(outputTausAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the jet container after selection
    if (m_useTruthJets) {
      outputJets = new xAOD::JetContainer();
      outputJetsAux = new xAOD::JetAuxContainer();
      outputJets->setStore(outputJetsAux);
      for (auto j : idx_jets) {
	const auto& jetPtr = inputJets->at(j);
	xAOD::Jet* jet = new xAOD::Jet();
	jet->makePrivateStore(*jetPtr);

	dec_particle_energy(*jet) = jet->e();

	outputJets->push_back(jet);
      }
      outputSGKey = "ParticleLevelJets_NOSYS";
      save = TDS()->record(outputJets, outputSGKey);
      saveAux = TDS()->record(outputJetsAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the large-R jet container after selection
    if (m_useTruthLargeRJets) {
      outputLargeRJets = new xAOD::JetContainer();
      outputLargeRJetsAux = new xAOD::JetAuxContainer();
      outputLargeRJets->setStore(outputLargeRJetsAux);
      for (auto j : idx_ljets) {
	const auto& jetPtr = inputLargeRJets->at(j);
	xAOD::Jet* ljet = new xAOD::Jet();
	ljet->makePrivateStore(*jetPtr);

	dec_particle_energy(*ljet) = ljet->e();

	outputLargeRJets->push_back(ljet);
      }
      outputSGKey = "ParticleLevelLargeRJets_NOSYS";
      save = TDS()->record(outputLargeRJets, outputSGKey);
      saveAux = TDS()->record(outputLargeRJetsAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    // write out the neutrino container (no selection)
    if (m_useTruthNeutrinos) {
      outputNeutrinos = new xAOD::TruthParticleContainer();
      outputNeutrinosAux = new xAOD::TruthParticleAuxContainer();
      outputNeutrinos->setStore(outputNeutrinosAux);
      for (auto v : idx_neutrinos) {
	const auto& nuPtr = inputNeutrinos->at(v);
	xAOD::TruthParticle* nu = new xAOD::TruthParticle();
	nu->makePrivateStore(*nuPtr);

	dec_particle_pt(*nu) = nu->pt();
	dec_particle_eta(*nu) = nu->eta();
	dec_particle_phi(*nu) = nu->phi();

	outputNeutrinos->push_back(nu);
      }
      outputSGKey = "ParticleLevelNeutrinos_NOSYS";
      save = TDS()->record(outputNeutrinos, outputSGKey);
      saveAux = TDS()->record(outputNeutrinosAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
        return StatusCode::FAILURE;
    }

    // write out the MET container
    if (m_useTruthMET) {
      outputMissingET = new xAOD::MissingETContainer();
      outputMissingETAux = new xAOD::MissingETAuxContainer();
      outputMissingET->setStore(outputMissingETAux);
      for (unsigned int m = 0; m < inputMissingET->size(); m++) {
	const auto& metPtr = inputMissingET->at(m);
	// keep only the MET that we need
	if (metPtr->name() != "NonInt")
	  continue;
	xAOD::MissingET* met = new xAOD::MissingET();
	met->makePrivateStore(*metPtr);

	outputMissingET->push_back(met);
      }
      outputSGKey = "ParticleLevelMissingET_NOSYS";
      save = TDS()->record(outputMissingET, outputSGKey);
      saveAux = TDS()->record(outputMissingETAux, outputSGKey + "Aux.");
      if (!save || !saveAux)
	return StatusCode::FAILURE;
    }

    return StatusCode::SUCCESS;
  }

  StatusCode ParticleLevelAlg::finalize() {
    ANA_MSG_INFO("Finalizing ParticleLevel");

    return StatusCode::SUCCESS;
  }

}  // namespace top
