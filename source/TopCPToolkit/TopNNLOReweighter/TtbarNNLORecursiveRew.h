#ifndef TTBAR_NNLO_RECURSIVE_REW_H
#define TTBAR_NNLO_RECURSIVE_REW_H

// Include necessary ATLAS Athena headers
#include "AsgTools/AsgTool.h"
#include "PATInterfaces/ISystematicsTool.h"
#include <PathResolver/PathResolver.h>
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/IReentrantSystematicsTool.h"
#include "PATInterfaces/SystematicsCache.h"

// Forward declarations
class TH1;
class TH1F;

// Everything else
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TopNNLOReweighter/NNLORewEnums.h"

// class TtbarNNLORecursiveRew : public virtual asg::AsgTool, public virtual CP::ISystematicsTool {
class TtbarNNLORecursiveRew : public virtual asg::AsgTool, public virtual CP::IReentrantSystematicsTool {
public:
    // Constructor
    TtbarNNLORecursiveRew(const std::string& name);
    virtual ~TtbarNNLORecursiveRew();

    // Initialization method
    virtual StatusCode initialize() override;

    // Systematics methods
    virtual CP::SystematicSet affectingSystematics() const override;
    virtual CP::SystematicSet recommendedSystematics() const override;
    // virtual StatusCode applySystematicVariation(const CP::SystematicSet& systConfig) override;

    // Inline methods
    void SetInputDirectory(const std::string& dirName) { m_dir = dirName; };
    void SetInputSuffix(const std::string& suffix) { m_suffix = suffix; };
    void SetSampleID(int sampleID) { m_sampleId = sampleID; };
    // void SetSystVar(int systVar) { m_systVar = systVar; };
    void SetType(int type) { m_type = type; };
    // void SetRewList(std::vector<std::string> list) { m_rewList = list; };
    // void AddRew(std::string rew) { m_rewList.emplace_back( rew ); };

    // Initialisation of the recursive reweightings
    void SetDefault2D(std::vector<std::string>& rewList);
    void SetDefault3D(std::vector<std::string>& rewList);
    void SetInverted2D(std::vector<std::string>& rewList);
    void SetInverted3D(std::vector<std::string>& rewList, int i);

    // Reweighting functions
    double Reweight(double x, int idx=0);
    double GetSysWeight(const CP::SystematicSet& sys, double Top_pt, double ATop_pt, double TTbar_M, double TTbar_Pt=-1);

private:

    // Member variables
    int m_sampleId;
    NNLORewEnums::SystVar m_systVar;
    int m_type;
    std::string m_dir;
    std::string m_suffix;
    std::map< NNLORewEnums::SystVar, std::vector<std::unique_ptr<TH1>> > m_h_nnlo;
    std::map< NNLORewEnums::SystVar, std::vector<std::unique_ptr<TH1>> > m_h_mc;
    std::map< NNLORewEnums::SystVar, std::vector<std::string> > m_rewMap;

    // Systematics
    StatusCode buildCachedSystematics();
    CP::SystematicsCache<NNLORewEnums::SystVar> m_calibCache {this};


};

#endif // TTBAR_NNLO_RECURSIVE_REW_H
