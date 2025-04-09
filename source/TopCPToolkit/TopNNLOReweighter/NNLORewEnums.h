#ifndef TopNNLOReweighter_ENUMS_H
#define TopNNLOReweighter_ENUMS_H

namespace NNLORewEnums {

    enum SampleId {
        Nominal = 0,
        PH7new = 1,
        aMCH7 = 2,
        PH7 = 3,
        aMCP8 = 4,
        PP8hdamp = 5,
        PP8muRup = 6,
        PP8muRdown = 7,
        PP8muFup = 8,
        PP8muFdown = 9,
        PP8FSRup = 10,
        PP8FSRdown = 11,
        PP8pthard1 = 12,
        MECoff = -1
    };

    static const std::map<std::string, SampleId> strToSampleId {
        {"Nominal", SampleId::Nominal},
        {"PH7new", SampleId::PH7new},
        {"aMCH7", SampleId::aMCH7},
        {"PH7", SampleId::PH7},
        {"aMCP8", SampleId::aMCP8},
        {"PP8hdamp", SampleId::PP8hdamp},
        {"PP8muRup", SampleId::PP8muRup},
        {"PP8muRdown", SampleId::PP8muRdown},
        {"PP8muFup", SampleId::PP8muFup},
        {"PP8muFdown", SampleId::PP8muFdown},
        {"PP8FSRup", SampleId::PP8FSRup},
        {"PP8FSRdown", SampleId::PP8FSRdown},
        {"PP8pthard1", SampleId::PP8pthard1},
        {"MECoff", SampleId::MECoff}
    };

    inline std::string sampleIdToStr(int testsample) {
        for (const auto& pair : strToSampleId) {
            if (pair.second == testsample) {
                return pair.first;
            }
        }
        return "unknownSampleId";
    }

    enum SystVar {
        ScaleUpTopPt = 1,
        ScaleDownTopPt = -1,
        ScaleUpTTbarM = 2,
        ScaleDownTTbarM = -2,
        ScaleUpTTbarPt = 3,
        ScaleDownTTbarPt = -3,
        PDFUp = 4,
        PDFDown = -4,
        MuRUp = 10,
        MuRDown = -10,
        MuFUp = 20,
        MuFDown = -20,
        MuRUpTopPt = 11,
        MuRDownTopPt = -11,
        MuFUpTopPt = 21,
        MuFDownTopPt = -21,
        MuRUpTTbarM = 12,
        MuRDownTTbarM = -12,
        MuFUpTTbarM = 22,
        MuFDownTTbarM = -22,
        // NoEW = 100, // MISSING!
        QEDPDFChoice = 200,
        Inv2D = 1000,
        Inv3D1 = 1001,
        Inv3D2 = 1002,
        Inv3D3 = 1003,
        Inv3D4 = 1004,
        Inv3D5 = 1005,
        FOvsPSTopPt = 2000,
        FOvsPSTTbarM = 3000,
        NoSyst = 0
    };

    static const std::map<std::string, SystVar> strToSystVar {
        {"scaleUpTopPt", SystVar::ScaleUpTopPt},
        {"scaleDownTopPt", SystVar::ScaleDownTopPt},
        {"scaleUpTTbarM", SystVar::ScaleUpTTbarM},
        {"scaleDownTTbarM", SystVar::ScaleDownTTbarM},
        {"scaleUpTTbarPt", SystVar::ScaleUpTTbarPt},
        {"scaleDownTTbarPt", SystVar::ScaleDownTTbarPt},
        {"PDFUp", SystVar::PDFUp},
        {"PDFDown", SystVar::PDFDown},
        {"muRUp", SystVar::MuRUp},
        {"muRDown", SystVar::MuRDown},
        {"muFUp", SystVar::MuFUp},
        {"muFDown", SystVar::MuFDown},
        {"muRUpTopPt", SystVar::MuRUpTopPt},
        {"muRDownTopPt", SystVar::MuRDownTopPt},
        {"muFUpTopPt", SystVar::MuFUpTopPt},
        {"muFDownTopPt", SystVar::MuFDownTopPt},
        {"muRUpTTbarM", SystVar::MuRUpTTbarM},
        {"muRDownTTbarM", SystVar::MuRDownTTbarM},
        {"muFUpTTbarM", SystVar::MuFUpTTbarM},
        {"muFDownTTbarM", SystVar::MuFDownTTbarM},
        // {"noEW", SystVar::NoEW}, // MISSING!
        {"QEDPDFChoice", SystVar::QEDPDFChoice},
        {"inv", SystVar::Inv2D},
        {"inv1", SystVar::Inv3D1},
        {"inv2", SystVar::Inv3D2},
        {"inv3", SystVar::Inv3D3},
        {"inv4", SystVar::Inv3D4},
        {"inv5", SystVar::Inv3D5},
        {"FOvsPSTopPt", SystVar::FOvsPSTopPt},
        {"FOvsPSTTbarM", SystVar::FOvsPSTTbarM},
        {"NoSyst", SystVar::NoSyst}
    };

    inline std::string systVarToStr(int testsyst) {
        for (const auto& pair : strToSystVar) {
            if (pair.second == testsyst) {
                return pair.first;
            }
        }
        return "unknownSystVar";
    }

    enum RewType {
        Rew2D = 1,
        Rew3D = 2
    };

    static const std::map<std::string, RewType> strToRewType {
        {"2D", RewType::Rew2D},
        {"3D", RewType::Rew3D}
    };

    inline std::string rewTypeToStr(int testrew) {
        for (const auto& pair : strToRewType) {
            if (pair.second == testrew) {
                return pair.first;
            }
        }
        return "unknownRewType";
    }

    template <class T> std::string printEnumOptions(const std::map<std::string, T> &availOpts) {
        std::stringstream sstream;
        for (const auto& elem : availOpts) {
            sstream << elem.first << " ";
        }
        return sstream.str();
    }

} // namespace NNLORewEnums

#endif // TopNNLOReweighter_ENUMS_H