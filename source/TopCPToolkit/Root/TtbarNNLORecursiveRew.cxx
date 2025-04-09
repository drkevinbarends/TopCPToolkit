#include "TopNNLOReweighter/TtbarNNLORecursiveRew.h"

// Constructor
TtbarNNLORecursiveRew::TtbarNNLORecursiveRew(const std::string& name)
    : asg::AsgTool(name)
    , m_systVar{NNLORewEnums::SystVar::NoSyst}
{
    declareProperty("SampleID", m_sampleId = 0, "Sample ID");
    declareProperty("Type", m_type = 1, "Type");
    // TO-DO: Change this once files are on cvmfs
    declareProperty("InputDirectory", m_dir = "dev/AnalysisTop/TTbarNNLOReweighter", "Input Directory");
    declareProperty("InputSuffix", m_suffix = "_3iter_2023_LUX", "Input Suffix");
}

TtbarNNLORecursiveRew::~TtbarNNLORecursiveRew() {
    for (auto& entry : m_rewMap) entry.second.clear();
    m_rewMap.clear();
    for (auto& entry : m_h_nnlo) entry.second.clear();
    m_h_nnlo.clear();
    for (auto& entry : m_h_mc) entry.second.clear();
    m_h_mc.clear();
}

// Initialization method
StatusCode TtbarNNLORecursiveRew::initialize()
{

    // get correct sample name
    std::string smpStr = "";
    switch ( m_sampleId ) {
        case NNLORewEnums::SampleId::Nominal:
            smpStr = "PP8";
            break;
        default:
            smpStr = NNLORewEnums::sampleIdToStr(m_sampleId);
            break;
    }

    // access the input directory
    if ( m_type == NNLORewEnums::RewType::Rew2D ) {
        m_dir += "/data_2d/";
    }
    else if ( m_type ==  NNLORewEnums::RewType::Rew3D ) {
        m_dir += "/data_3d/";
    }

    m_dir = PathResolverFindCalibDirectory(m_dir);

    // set up our cache of systematics, to be filled in buildCachedSystematics()
    CP::SystematicSet affSysts = CP::SystematicSet();
    m_calibCache.initialize(
        std::move(affSysts),
        [this] (const CP::SystematicSet& sys, NNLORewEnums::SystVar& systvar) {
            ANA_MSG_WARNING("Mapping for " << sys.name() << "missing, defaulting to no systematic.");
            systvar = NNLORewEnums::SystVar::NoSyst;
            return StatusCode::SUCCESS;
        }
    );

    // loop over all systematics and map the corresponding histograms and reweighting recipes
    for (const auto& pair : NNLORewEnums::strToSystVar) {
        auto thisSystVar = pair.second;

        // first, the series of reweightings to apply:
        // we only define here the default recipe (based on reference histograms)
        // --> USERS: if you want a custom recipe, check out the code and modify the
        // "SetDefault2D", "SetDefault3D", etc. methods below!
        std::vector<std::string> thisRewList;
        if ( m_type == NNLORewEnums::RewType::Rew2D ) {
            if ( thisSystVar == NNLORewEnums::SystVar::Inv2D ) {
                SetInverted2D(thisRewList);
            }
            else {
                SetDefault2D(thisRewList);
            }
        }
        else if ( m_type == NNLORewEnums::RewType::Rew3D ) {
            if ( thisSystVar >= NNLORewEnums::SystVar::Inv3D1 && thisSystVar <= NNLORewEnums::SystVar::Inv3D5 ) {
                SetInverted3D(thisRewList, thisSystVar-1000);
            }
            else {
                SetDefault3D(thisRewList);
            }
        }

        // fill the map of reweightings with the recipe corresponding to this systematic
        m_rewMap.emplace(thisSystVar, thisRewList);

        // get correct systematic name
        std::string sysStr = "";
        std::string suffix = m_suffix;
        switch ( thisSystVar ) {
            case NNLORewEnums::SystVar::NoSyst:
                break;
            case NNLORewEnums::SystVar::FOvsPSTopPt:
                break;
            case NNLORewEnums::SystVar::FOvsPSTTbarM:
                break;
            case NNLORewEnums::SystVar::QEDPDFChoice:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.erase(suffix.find("_LUX"), 4);
                else suffix.append("_LUX");
                break;
            case NNLORewEnums::SystVar::Inv2D:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv_LUX");
                else suffix.append("_inv");
                break;
            case NNLORewEnums::SystVar::Inv3D1:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv1_LUX");
                else suffix.append("_inv1");
                break;
            case NNLORewEnums::SystVar::Inv3D2:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv2_LUX");
                else suffix.append("_inv2");
                break;
            case NNLORewEnums::SystVar::Inv3D3:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv3_LUX");
                else suffix.append("_inv3");
                break;
            case NNLORewEnums::SystVar::Inv3D4:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv4_LUX");
                else suffix.append("_inv4");
                break;
            case NNLORewEnums::SystVar::Inv3D5:
                if ( suffix.find("_LUX") != std::string::npos ) suffix.replace(suffix.find("_LUX"), 4, "_inv5_LUX");
                else suffix.append("_inv5");
                break;
            default:
                sysStr = "_" + NNLORewEnums::systVarToStr(thisSystVar);
                break;
        }
        // additional edge cases
        if (
            ( ( thisSystVar == NNLORewEnums::SystVar::ScaleUpTTbarPt || thisSystVar == NNLORewEnums::SystVar::ScaleDownTTbarPt )
                && m_type == NNLORewEnums::RewType::Rew2D
            ) ||
            ( thisSystVar == NNLORewEnums::SystVar::Inv2D && m_type == NNLORewEnums::RewType::Rew3D
            ) ||
            ( ( thisSystVar >= NNLORewEnums::SystVar::Inv3D1 && thisSystVar <= NNLORewEnums::SystVar::Inv3D5 )
                && m_type == NNLORewEnums::RewType::Rew2D
            )
        ) {
            ANA_MSG_INFO("Unsupported systematic variation " << NNLORewEnums::systVarToStr(thisSystVar) << " for reweighting of type " << NNLORewEnums::rewTypeToStr(m_type) << ". Will be ignored.");
            // here we treat it as nominal
            sysStr = "";
            suffix = m_suffix;
        }

        if ( m_sampleId != NNLORewEnums::SampleId::Nominal ) {
            // no systematics for alternative samples!
            sysStr = "";
            suffix = m_suffix;
        }

        // open the relevant file
        std::unique_ptr<TFile> f ( TFile::Open((m_dir+"/RecursiveReweighting__"+smpStr+sysStr+suffix+".root").c_str()) );
        if ( !f ) ANA_MSG_ERROR("Cannot open the file! Aborting.");
	else ANA_MSG_INFO("Opening NNLO reweighting file " << f->GetName());

        // put the necessary histograms in our vectors
        std::string histname;
        std::vector<std::unique_ptr<TH1>> this_h_nnlo;
        std::vector<std::unique_ptr<TH1>> this_h_mc;
        for ( unsigned int i_rew=0; i_rew<m_rewMap[thisSystVar].size(); i_rew++ ) {
            // NNLO predictions
            histname = "h_" + m_rewMap[thisSystVar][i_rew];
            auto tmp_nnlo = dynamic_cast<TH1*>( f->Get(histname.c_str()) );
            if ( !tmp_nnlo ) ANA_MSG_ERROR("Could not retrieve NNLO histogram " << histname << "! Aborting.");
            this_h_nnlo.emplace_back(tmp_nnlo);
            this_h_nnlo[i_rew]->SetDirectory(nullptr);

            // MC predictions
            if ( i_rew == 0 ) histname = "h_" + m_rewMap[thisSystVar][i_rew] + "_" + smpStr;
            else              histname = "h_" + m_rewMap[thisSystVar][i_rew] + "_" + smpStr + "_rew_" + std::to_string(i_rew-1);
            auto tmp_mc = dynamic_cast<TH1*>( f->Get(histname.c_str()) );
            if ( !tmp_mc ) ANA_MSG_ERROR("Could not retrieve MC histogram " << histname << "! Aborting.");
            this_h_mc.emplace_back(tmp_mc);
            this_h_mc[i_rew]->SetDirectory(nullptr);
        }
        f->Close();

        // fill the map of reweightings with the histograms corresponding to this systematic
        m_h_nnlo.emplace(thisSystVar, std::move(this_h_nnlo));
        m_h_mc.emplace(thisSystVar, std::move(this_h_mc));

    } // end loop over systematics

    // finally build the cache of systematics indexed against our SystVar enum
    ANA_CHECK( buildCachedSystematics() );

    // and add them to the registry
    CP::SystematicRegistry& registry = CP::SystematicRegistry::getInstance();
    if ( !registry.registerSystematics(*this) ) {
        ANA_MSG_ERROR("Unable to register the systematics!");
        return StatusCode::FAILURE;
    }

    return StatusCode::SUCCESS;
}

// Systematics methods
CP::SystematicSet TtbarNNLORecursiveRew::affectingSystematics() const
{
    return m_calibCache.affectingSystematics();
}

StatusCode TtbarNNLORecursiveRew::buildCachedSystematics()
{

    // No systematic to register if the sample is not nominal!
    if ( m_sampleId != NNLORewEnums::SampleId::Nominal ) {
        ANA_MSG_INFO("Running on alternative sample of type " << NNLORewEnums::sampleIdToStr(m_sampleId) << " for which there are no systematic.");
        return StatusCode::SUCCESS;
    }

    std::string sysPrefix = "NNLO_";

    for (const auto& pair : NNLORewEnums::strToSystVar) {
        std::string sysname = pair.first;
        auto thisSystVar = pair.second;

        // ignore nominal
        if ( thisSystVar == NNLORewEnums::SystVar::NoSyst ) continue;

        // the same additional edge cases that we warned about in the initialize() method
        if (
            ( ( thisSystVar == NNLORewEnums::SystVar::ScaleUpTTbarM || thisSystVar == NNLORewEnums::SystVar::ScaleDownTTbarM)
                && m_type == NNLORewEnums::RewType::Rew2D
            ) ||
            ( thisSystVar == NNLORewEnums::SystVar::Inv2D && m_type == NNLORewEnums::RewType::Rew3D
            ) ||
            ( ( thisSystVar >= NNLORewEnums::SystVar::Inv3D1 && thisSystVar <= NNLORewEnums::SystVar::Inv3D5 )
                && m_type == NNLORewEnums::RewType::Rew2D
            )
        ) continue;

        // handle different types of systematics
        if ( sysname.find("Up") != std::string::npos ) {
            sysname.erase(sysname.find("Up"), 2);
            ANA_CHECK( m_calibCache.add( CP::SystematicVariation( (sysPrefix + sysname).c_str(), 1), thisSystVar) );
        }
        else if ( sysname.find("Down") != std::string::npos ) {
            sysname.erase(sysname.find("Down"), 2);
            ANA_CHECK( m_calibCache.add( CP::SystematicVariation( (sysPrefix + sysname).c_str(), -1), thisSystVar ) );
        }
        else {
            // one-point systematic
            ANA_CHECK( m_calibCache.add( CP::SystematicVariation( (sysPrefix + sysname).c_str()), thisSystVar ) );
        }
    }

    return StatusCode::SUCCESS;
}

CP::SystematicSet TtbarNNLORecursiveRew::recommendedSystematics() const
{
    return affectingSystematics();
}

// Initialisation of the recursive reweightings
void TtbarNNLORecursiveRew::SetDefault2D(std::vector<std::string>& rewList)
{
    rewList.clear();
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
}

void TtbarNNLORecursiveRew::SetDefault3D(std::vector<std::string>& rewList)
{
    rewList.clear();
    rewList.emplace_back( "TTbarPt" );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarPt" );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarPt" );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
}

void TtbarNNLORecursiveRew::SetInverted2D(std::vector<std::string>& rewList)
{
    rewList.clear();
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarM"  );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TopPt"   );
    rewList.emplace_back( "TTbarM"  );
}

void TtbarNNLORecursiveRew::SetInverted3D(std::vector<std::string>& rewList, int i)
{
    rewList.clear();
    if(i==1){
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
    }
    else if(i==2){
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
    }
    else if(i==3){
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
    }
    else if(i==4){
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarM"  );
    }
    else if(i==5){
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarPt" );
        rewList.emplace_back( "TTbarM"  );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TopPt"   );
        rewList.emplace_back( "TTbarPt" );
    }
    else{
        ANA_MSG_ERROR("Invalid argument for SetInverted3D: " << i << " (valid options are 1, 2, 3, 4, 5)");
    }
}

// Reweighting functions
double TtbarNNLORecursiveRew::Reweight(double x, int idx)
{
    int bin = m_h_nnlo[m_systVar][idx]->FindBin(x);
    if ( bin==0 || bin>m_h_nnlo[m_systVar][idx]->GetNbinsX() ) return 1.;
    double y_nnlo = m_h_nnlo[m_systVar][idx]->GetBinContent(bin);
    double y_mc   = m_h_mc[m_systVar][idx]->GetBinContent(bin);
    if ( y_nnlo==0 ) return 1.;
    if ( y_mc  ==0 ) return 1.;
    return y_nnlo/y_mc;
}

double TtbarNNLORecursiveRew::GetSysWeight(const CP::SystematicSet& sys, double Top_pt, double ATop_pt, double TTbar_M, double TTbar_Pt)
{
    // switch to the appropriate systematic variation
    const NNLORewEnums::SystVar *systVar {nullptr};
    m_calibCache.get(sys, systVar).ignore();

    m_systVar = *systVar;

    double result = 1.;
    for(unsigned int i_rew=0; i_rew<m_rewMap[m_systVar].size(); i_rew++){
        if      ( m_rewMap[m_systVar][i_rew]=="TopPt" )   result *= sqrt( Reweight(Top_pt, i_rew) * Reweight(ATop_pt, i_rew) );
        else if ( m_rewMap[m_systVar][i_rew]=="TTbarPt" ) result *= Reweight(TTbar_Pt, i_rew);
        else if ( m_rewMap[m_systVar][i_rew]=="TTbarM" )  result *= Reweight(TTbar_M, i_rew);
    }
    if ( m_systVar==NNLORewEnums::SystVar::FOvsPSTopPt ) {
        double weight_top  = Top_pt  > 0 ? 9.78044e-01 + sqrt( 4.35553e-06 * Top_pt/1e3  ) : 1.;
        double weight_atop = ATop_pt > 0 ? 9.78044e-01 + sqrt( 4.35553e-06 * ATop_pt/1e3 ) : 1.;
        result *= sqrt( weight_top * weight_atop );
    }
    else if ( m_systVar==NNLORewEnums::SystVar::FOvsPSTTbarM ) {
        result *= TTbar_M > 0 ? 9.62236e-01 + sqrt( 2.78789e-06 * TTbar_M/1e3 ) : 1.;
    }
    return result;

}
