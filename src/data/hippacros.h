
#include "data/data.h"

namespace ofs::data
{
    class HippacrosDataProvider : public DataProvider
    {
            enum Field
            {
                idxType = 0,
                idxHIPnum,
                idxProxy,
                idxRAhms,
                idxDEhms,
                idxVisualMag,
                idxVariableFlag,
                idxrVisualMag,
                idxRAdeg,
                idxDEdeg,
                idxAstroRef,
                idxParallex,
                idxRApm,
                idxDEpm,
                idxeRAdeg,
                idxeDEdeg,
                idxeParallex,
                idxepmRA,
                idxepmDE,
                idxDERAcorr,
                idxPlxRAcorr,
                idxPlxDEcorr,
                idxpmRARAcorr,
                idxpmRADEcorr,
                idxpmRAPlxcorr,
                idxpmDERAcorr,
                idxpmDEDEcorr,
                idxpmDEpmRAcorr,
                idxF1,
                idxF2,
                idxHIP2,
                idxBTMag,
                idxeBTMag,
                idxVTMag,
                idxeVTMag,
                idxmBTMag,
                idxBVmag,
                idxeBVmag,
                idxrBVmag,
                idxVImag,
                idxeVImag,
                idxrVImag,
                idxCombMag,
                idxHPMag,
                idxeHPMag,
                idxHPscat,
                idxeHPmag,
                idxmHPmag,
                idxHPmax,
                idxHPmin,
                idxPeriod,
                idxHVarTYpe,
                idxmoreVar,
                idxMorePhoto,
                idxCCDM,
                idxnCCDM,
                idxNsys,
                idxNcomp,
                idxMultiFlag,
                idxSource,
                idxQual,
                idxmHIP,
                idxTheta,
                idxRho,
                idxeRho,
                idxdHp,
                idxedHp,
                idxSurvey,
                idxChart,
                idxNotes,
                idxHDnum,
                idxBD,
                idxCoD,
                idxCPD,
                idxVIred,
                idxSpectralType,
                idxrSpectralType
            };
            
        public:
            HippacrosDataProvider() = default;
            ~HippacrosDataProvider() = default;

            void load(fs::path file);
        
        protected:
            bool addStar(const std::string &line);

            void loadFile(std::ifstream &inFile);
            void loadMapped(fs::path file, uint32_t fn, uint32_t totalFiles);

        private:
            uint64_t starTotal = 0;
            uint64_t starAccepted = 0;
    };
}