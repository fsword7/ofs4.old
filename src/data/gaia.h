
#include "data/data.h"

namespace ofs::data
{
    class GaiaDataProvider : public DataProvider
    {
        public:
            GaiaDataProvider() = default;
            ~GaiaDataProvider() = default;

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