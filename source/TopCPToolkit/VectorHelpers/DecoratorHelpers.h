#ifndef DECORATORHELPERS_H
#define DECORATORHELPERS_H

#include <Math/Vector4D.h>
#include <TMath.h>

/**
 * @brief Template function to fill particle information into a given object.
 *
 * This template function assigns a value to a decorator associated with an
 * object.
 *
 * @tparam T Type of the value being decorated (e.g., float, int).
 * @tparam U Type of the value to be assigned.
 * @tparam V Type of the object that will be decorated.
 * @param dec The decorator that will hold the value.
 * @param value The value to be assigned to the decorator.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U, typename V>
void FillParticleInfo(const SG::AuxElement::Decorator<T>& dec, const U& value,
                      const V* object) {
  dec(*object) = value;
}

/**
 * @brief Template function to fill particle information into a given object.
 *
 * This template function assigns a value to a decorator associated with an
 * object.
 *
 * @tparam T Type of the value being decorated (e.g., float, int).
 * @tparam U Type of the value to be assigned.
 * @tparam V Type of the object that will be decorated.
 * @param dec The decorator that will hold the value.
 * @param value The value to be assigned to the decorator.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U, typename V>
void FillVectorParticleInfo(
    const SG::AuxElement::Decorator<std::vector<T>>& dec, const U& value,
    const V* object) {
  dec(*object).push_back(value);
}

/**
 * @brief Template function to fill basic particle kinematics (mass, pt, eta,
 * phi) into an object.
 *
 * This function populates an object's decorators with mass, pt, eta, and phi
 * values extracted from a ROOT::Math::PtEtaPhiMVector representing the
 * particle.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param particle The particle vector containing pt, eta, phi, and mass.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillParticleInfo(const SG::AuxElement::Decorator<T>& dec_pt,
                      const SG::AuxElement::Decorator<T>& dec_eta,
                      const SG::AuxElement::Decorator<T>& dec_phi,
                      const SG::AuxElement::Decorator<T>& dec_m,
                      const ROOT::Math::PtEtaPhiMVector& particle, U* object) {
  FillParticleInfo(dec_pt, particle.Pt(), object);
  FillParticleInfo(dec_eta, particle.Eta(), object);
  FillParticleInfo(dec_phi, particle.Phi(), object);
  FillParticleInfo(dec_m, particle.M(), object);
}

/**
 * @brief Template function to fill basic vector particle kinematics (mass, pt,
 * eta, phi) into an object.
 *
 * This function populates an object's decorators with mass, pt, eta, and phi
 * values extracted from a ROOT::Math::PtEtaPhiMVector representing the
 * particle.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param particle The particle vector containing pt, eta, phi, and mass.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillVectorParticleInfo(
    const SG::AuxElement::Decorator<std::vector<T>>& dec_pt,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_eta,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_phi,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_m,
    const ROOT::Math::PtEtaPhiMVector& particle, U* object) {
  FillVectorParticleInfo(dec_pt, particle.Pt(), object);
  FillVectorParticleInfo(dec_eta, particle.Eta(), object);
  FillVectorParticleInfo(dec_phi, particle.Phi(), object);
  FillVectorParticleInfo(dec_m, particle.M(), object);
}

/**
 * @brief Template function to fill particle kinematics and PDG ID into an
 * object.
 *
 * This function populates an object's decorators with mass, pt, eta, phi, and
 * PDG ID values extracted from a ROOT::Math::PtEtaPhiMVector representing the
 * particle and an integer PDG ID.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param dec_pdgId Decorator for PDG ID.
 * @param particle The particle vector containing pt, eta, phi, and mass.
 * @param pdgId The PDG ID of the particle.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillParticleInfo(const SG::AuxElement::Decorator<T>& dec_pt,
                      const SG::AuxElement::Decorator<T>& dec_eta,
                      const SG::AuxElement::Decorator<T>& dec_phi,
                      const SG::AuxElement::Decorator<T>& dec_m,
                      const SG::AuxElement::Decorator<int>& dec_pdgId,
                      const ROOT::Math::PtEtaPhiMVector& particle, int pdgId,
                      U* object) {
  FillParticleInfo(dec_pdgId, pdgId, object);
  FillParticleInfo(dec_pt, dec_eta, dec_phi, dec_m, particle, object);
}

/**
 * @brief Template function to fill vector particle kinematics and PDG ID into
 * an object.
 *
 * This function populates an object's decorators with mass, pt, eta, phi, and
 * PDG ID values extracted from a ROOT::Math::PtEtaPhiMVector representing the
 * particle and an integer PDG ID.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param dec_pdgId Decorator for PDG ID.
 * @param particle The particle vector containing pt, eta, phi, and mass.
 * @param pdgId The PDG ID of the particle.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillVectorParticleInfo(
    const SG::AuxElement::Decorator<std::vector<T>>& dec_pt,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_eta,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_phi,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_m,
    const SG::AuxElement::Decorator<std::vector<int>>& dec_pdgId,
    const ROOT::Math::PtEtaPhiMVector& particle, int pdgId, U* object) {
  FillVectorParticleInfo(dec_pdgId, pdgId, object);
  FillVectorParticleInfo(dec_pt, dec_eta, dec_phi, dec_m, particle, object);
}

/**
 * @brief Template function to fill default (placeholder) particle kinematics
 * into an object.
 *
 * This function populates an object's decorators with default values for pt,
 * eta, phi and mass.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillDefaultParticleInfo(const SG::AuxElement::Decorator<T>& dec_pt,
                             const SG::AuxElement::Decorator<T>& dec_eta,
                             const SG::AuxElement::Decorator<T>& dec_phi,
                             const SG::AuxElement::Decorator<T>& dec_m,
                             U* object) {
  FillParticleInfo(dec_pt, -1, object);
  FillParticleInfo(dec_eta, -999, object);
  FillParticleInfo(dec_phi, -999, object);
  FillParticleInfo(dec_m, -1, object);
}

/**
 * @brief Template function to fill default (placeholder) vector particle
 * kinematics into an object.
 *
 * This function populates an object's decorators with default values for pt,
 * eta, phi and mass.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillDefaultVectorParticleInfo(
    const SG::AuxElement::Decorator<std::vector<T>>& dec_pt,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_eta,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_phi,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_m, U* object) {
  dec_pt(*object) = {};
  dec_eta(*object) = {};
  dec_phi(*object) = {};
  dec_m(*object) = {};
}

/**
 * @brief Template function to fill default (placeholder) particle kinematics
 * and PDG ID into an object.
 *
 * This function populates an object's decorators with default values for pt,
 * eta, phi, mass and PDG ID.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param dec_pdgId Decorator for PDG ID.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillDefaultParticleInfo(const SG::AuxElement::Decorator<T>& dec_pt,
                             const SG::AuxElement::Decorator<T>& dec_eta,
                             const SG::AuxElement::Decorator<T>& dec_phi,
                             const SG::AuxElement::Decorator<T>& dec_m,
                             const SG::AuxElement::Decorator<int>& dec_pdgId,
                             U* object) {
  FillParticleInfo(dec_pdgId, 0, object);
  FillDefaultParticleInfo(dec_pt, dec_eta, dec_phi, dec_m, object);
}

/**
 * @brief Template function to fill default (placeholder) vector particle
 * kinematics and PDG ID into an object.
 *
 * This function populates an object's decorators with default values for pt,
 * eta, phi, mass and PDG ID.
 *
 * @tparam T Type of the value being decorated (e.g., float).
 * @tparam U Type of the object that will be decorated.
 * @param dec_pt Decorator for transverse momentum (pt).
 * @param dec_eta Decorator for pseudorapidity (eta).
 * @param dec_phi Decorator for azimuthal angle (phi).
 * @param dec_m Decorator for mass.
 * @param dec_pdgId Decorator for PDG ID.
 * @param object Pointer to the object that will be decorated.
 */
template <typename T, typename U>
void FillDefaultVectorParticleInfo(
    const SG::AuxElement::Decorator<std::vector<T>>& dec_pt,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_eta,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_phi,
    const SG::AuxElement::Decorator<std::vector<T>>& dec_m,
    const SG::AuxElement::Decorator<std::vector<int>>& dec_pdgId, U* object) {
  dec_pdgId(*object) = {};
  FillDefaultVectorParticleInfo(dec_pt, dec_eta, dec_phi, dec_m, object);
}

/**
 * @brief Struct to manage and apply decorators to objects.
 *
 * PartonDecorator provides methods to initialize, retrieve, and apply
 * decorators for various properties like mass, pt, eta, phi, and PDG ID. It
 * manages both float and integer decorators and allows custom decoration as
 * well as default or particle-specific decoration.
 */
struct PartonDecorator {
  std::map<std::string, std::unique_ptr<SG::AuxElement::Decorator<float>>>
      floatDecorators;
  std::map<std::string,
           std::unique_ptr<SG::AuxElement::Decorator<std::vector<float>>>>
      vectorfloatDecorators;
  std::map<std::string, std::unique_ptr<SG::AuxElement::Decorator<int>>>
      intDecorators;
  std::map<std::string,
           std::unique_ptr<SG::AuxElement::Decorator<std::vector<int>>>>
      vectorintDecorators;

  /**
   * @brief Initialize a float decorator by name.
   *
   * @param name The name of the float decorator to initialize.
   */
  void initializeFloatDecorator(const std::string& name) {
    floatDecorators[name] =
        std::make_unique<SG::AuxElement::Decorator<float>>(name);
  }

  /**
   * @brief Initialize a vector of float decorator by name.
   *
   * @param name The name of the vector offloat decorator to initialize.
   */
  void initializeVectorFloatDecorator(const std::string& name) {
    vectorfloatDecorators[name] =
        std::make_unique<SG::AuxElement::Decorator<std::vector<float>>>(name);
  }

  /**
   * @brief Initialize multiple float decorators by a list of names.
   *
   * @param names A vector of decorator names.
   */
  void initializeFloatDecorator(const std::vector<std::string>& names) {
    for (const auto& name : names) {
      initializeFloatDecorator(name);
    }
  }

  /**
   * @brief Initialize pT, eta, phi, m float decorators.
   *
   * @param names A vector of decorator names.
   */
  void initializePtEtaPhiMDecorator(const std::string& prefix) {
    initializeFloatDecorator(prefix + "_pt");
    initializeFloatDecorator(prefix + "_eta");
    initializeFloatDecorator(prefix + "_phi");
    initializeFloatDecorator(prefix + "_m");
  }

  /**
   * @brief Initialize vector of pT, eta, phi, m float decorators.
   *
   * @param names A vector of decorator names.
   */
  void initializeVectorPtEtaPhiMDecorator(const std::string& prefix) {
    initializeVectorFloatDecorator(prefix + "_pt");
    initializeVectorFloatDecorator(prefix + "_eta");
    initializeVectorFloatDecorator(prefix + "_phi");
    initializeVectorFloatDecorator(prefix + "_m");
  }

  /**
   * @brief Initialize an integer decorator by name.
   *
   * @param name The name of the integer decorator to initialize.
   */
  void initializeIntDecorator(const std::string& name) {
    intDecorators[name] =
        std::make_unique<SG::AuxElement::Decorator<int>>(name);
  }

  /**
   * @brief Initialize a vector of int decorator by name.
   *
   * @param name The name of the vector ofint decorator to initialize.
   */
  void initializeVectorIntDecorator(const std::string& name) {
    vectorintDecorators[name] =
        std::make_unique<SG::AuxElement::Decorator<std::vector<int>>>(name);
  }

  /**
   * @brief Initialize multiple integer decorators by a list of names.
   *
   * @param names A vector of decorator names.
   */
  void initializeIntDecorator(const std::vector<std::string>& names) {
    for (const auto& name : names) {
      initializeIntDecorator(name);
    }
  }

  /**
   * @brief Retrieve a pointer to an integer decorator by name.
   *
   * @param name The name of the decorator to retrieve.
   * @return Pointer to the requested integer decorator.
   * @throws std::runtime_error if the decorator is not found.
   */
  SG::AuxElement::Decorator<int>* getIntDecorator(
      const std::string& name) const {
    auto it = intDecorators.find(name);
    if (it != intDecorators.end()) {
      return it->second
          .get();  // Use .get() to return the raw pointer from unique_ptr
    } else
      throw std::runtime_error("Decorator with name " + name + " not found.");
  }

  /**
   * @brief Retrieve a pointer to a vector of int decorator by name.
   *
   * @param name The name of the decorator to retrieve.
   * @return Pointer to the requested vector of int decorator.
   * @throws std::runtime_error if the decorator is not found.
   */
  SG::AuxElement::Decorator<std::vector<int>>* getVectorIntDecorator(
      const std::string& name) const {
    auto it = vectorintDecorators.find(name);
    if (it != vectorintDecorators.end()) {
      return it->second
          .get();  // Use .get() to return the raw pointer from unique_ptr
    } else
      throw std::runtime_error("Decorator with name " + name + " not found.");
  }

  /**
   * @brief Retrieve a pointer to a float decorator by name.
   *
   * @param name The name of the decorator to retrieve.
   * @return Pointer to the requested float decorator.
   * @throws std::runtime_error if the decorator is not found.
   */
  SG::AuxElement::Decorator<float>* getFloatDecorator(
      const std::string& name) const {
    auto it = floatDecorators.find(name);
    if (it != floatDecorators.end()) {
      return it->second
          .get();  // Use .get() to return the raw pointer from unique_ptr
    } else
      throw std::runtime_error("Decorator with name " + name + " not found.");
  }

  /**
   * @brief Retrieve a pointer to a vector of float decorator by name.
   *
   * @param name The name of the decorator to retrieve.
   * @return Pointer to the requested vector of float decorator.
   * @throws std::runtime_error if the decorator is not found.
   */
  SG::AuxElement::Decorator<std::vector<float>>* getVectorFloatDecorator(
      const std::string& name) const {
    auto it = vectorfloatDecorators.find(name);
    if (it != vectorfloatDecorators.end()) {
      return it->second
          .get();  // Use .get() to return the raw pointer from unique_ptr
    } else
      throw std::runtime_error("Decorator with name " + name + " not found.");
  }

  /**
   * @brief Template to apply default particle kinematics decoration to a
   * PartonHistory object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateDefault(const std::string& prefix, T* object) {
    FillDefaultParticleInfo(
        *getFloatDecorator(prefix + "_pt"), *getFloatDecorator(prefix + "_eta"),
        *getFloatDecorator(prefix + "_phi"), *getFloatDecorator(prefix + "_m"),
        *getIntDecorator(prefix + "_pdgId"), object);
  }

  /**
   * @brief Template to apply default vector particle kinematics decoration to a
   * PartonHistory object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateVectorDefault(const std::string& prefix, T* object) {
    FillDefaultVectorParticleInfo(*getVectorFloatDecorator(prefix + "_pt"),
                                  *getVectorFloatDecorator(prefix + "_eta"),
                                  *getVectorFloatDecorator(prefix + "_phi"),
                                  *getVectorFloatDecorator(prefix + "_m"),
                                  *getVectorIntDecorator(prefix + "_pdgId"),
                                  object);
  }

  /**
   * @brief Template to apply particle kinematics decoration without PDG ID to
   * an object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateDefaultNoPdgId(const std::string& prefix, T* object) {
    FillDefaultParticleInfo(*getFloatDecorator(prefix + "_pt"),
                            *getFloatDecorator(prefix + "_eta"),
                            *getFloatDecorator(prefix + "_phi"),
                            *getFloatDecorator(prefix + "_m"), object);
  }

  /**
   * @brief Template to apply custom float decoration to an object.
   *
   * @param name The name of the decorator to be used for the custom float
   * value.
   * @param value The custom float value to be used for decoration.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateCustom(const std::string& name, float value, T* object) {
    FillParticleInfo(*getFloatDecorator(name), value, object);
  }

  /**
   * @brief Template to apply custom integer decoration to an object.
   *
   * @param name The name of the decorator to be used for the custom integer
   * value.
   * @param value The custom integer value to be used for decoration.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateCustom(const std::string& name, int value, T* object) {
    FillParticleInfo(*getIntDecorator(name), value, object);
  }

  /**
   * @brief Template to apply particle kinematics decoration to an object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param p The particle vector containing pt, eta, phi, and mass.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateParticle(const std::string& prefix,
                        const ROOT::Math::PtEtaPhiMVector& p, T* object) {
    FillParticleInfo(*getFloatDecorator(prefix + "_pt"),
                     *getFloatDecorator(prefix + "_eta"),
                     *getFloatDecorator(prefix + "_phi"),
                     *getFloatDecorator(prefix + "_m"), p, object);
  }

  /**
   * @brief Template to apply particle kinematics and PDG ID decoration to an
   * object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param p The particle vector containing pt, eta, phi, and mass.
   * @param pdgId The PDG ID of the particle.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateParticle(const std::string& prefix,
                        const ROOT::Math::PtEtaPhiMVector& p, int pdgId,
                        T* object) {
    FillParticleInfo(
        *getFloatDecorator(prefix + "_pt"), *getFloatDecorator(prefix + "_eta"),
        *getFloatDecorator(prefix + "_phi"), *getFloatDecorator(prefix + "_m"),
        *getIntDecorator(prefix + "_pdgId"), p, pdgId, object);
  }

  /**
   * @brief Template to apply vector particle kinematics and PDG ID decoration
   * to an object.
   *
   * @param prefix The prefix to be added to the decorator names.
   * @param p The particle vector containing pt, eta, phi, and mass.
   * @param pdgId The PDG ID of the particle.
   * @param object Pointer to the object to be decorated.
   */
  template <typename T>
  void decorateVectorParticle(
      const std::string& prefix,
      const std::vector<ROOT::Math::PtEtaPhiMVector>& vec_p,
      const std::vector<int>& vec_pdgId, T* object) {
    ROOT::Math::PtEtaPhiMVector p;
    int pdgId;
    for (size_t i = 0; i < vec_p.size(); i++) {
      p = vec_p.at(i);
      pdgId = vec_pdgId.at(i);
      FillVectorParticleInfo(*getVectorFloatDecorator(prefix + "_pt"),
                             *getVectorFloatDecorator(prefix + "_eta"),
                             *getVectorFloatDecorator(prefix + "_phi"),
                             *getVectorFloatDecorator(prefix + "_m"),
                             *getVectorIntDecorator(prefix + "_pdgId"), p,
                             pdgId, object);
    }
  }
};

#endif  // DECORATORHELPERS_H
