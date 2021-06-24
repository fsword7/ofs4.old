
#include "main/core.h"
#include "data/hippacros.h"

using namespace ofs::data;


bool HippacrosDataProvider::addStar(const std::string &line)
{
    std::vector<std::string> tokens = split(line, "|");

    // std::cout << tokens[idxHIPnum] << "," << tokens[idxParallex] << "," << 
    //     tokens[idxeParallex] << "," << tokens[idxVisualMag] << "," <<
    //     tokens[idxRAdeg] << "," << tokens[idxDEdeg] << std::endl;
    // std::cout.flush();

    // HIP catalog number
    int hip = std::stoi(tokens[idxHIPnum]);

    if (tokens[idxParallex].empty())
    {
        std::cout << "HIP " << hip << ": Unknown parallex - skipped" << std::endl; 
        return false;
    }

    try
    {
        // Parallex
        double parallax = std::stod(tokens[idxParallex]);
        double plxerr = std::stod(tokens[idxeParallex]);
        double appmag = std::stod(tokens[idxVisualMag]);

        // Ascension/declination
        double ra = std::stod(tokens[idxRAdeg]);
        double de = std::stod(tokens[idxDEdeg]);

        // std::cout << "HIP " << hip << " RA: " << ra << " DE: " << de << " PLX: " << parallax << std::endl;
    }
    catch (std::invalid_argument)
    {
        std::cout << "HIP " << hip << ": can't parse field due to invalud number - skipped" << std::endl;
        return false;
    }

    return true;
}

void HippacrosDataProvider::loadFile(std::ifstream &inFile)
{
    std::string line;
    uint32_t ln = 0;
    while (std::getline(inFile, line))
    {
        if (addStar(line))
            starAccepted++;
        starTotal++;
        ln++;
    }
}

void HippacrosDataProvider::loadMapped(fs::path file, uint32_t fn, uint32_t totalFiles)
{
    if (file.extension() == ".gz")
        return;
    if (file.extension() != ".dat")
        return;
    std::ifstream inFile(file);

    if (!inFile.is_open())
    {
        std::cout << "Can't open file " << file << std::endl;
        return;
    }

    loadFile(inFile);

    inFile.close();
}

void HippacrosDataProvider::load(fs::path file)
{
    if (fs::is_directory(file))
    {
        std::vector<fs::path> files;
        uint32_t numFiles = 0;

        for (const auto &file : fs::recursive_directory_iterator(file))
        {
            files.push_back(file);
            numFiles++;
        }

        uint32_t fn = 0;
        for (const auto &file : files)
        {
            starAccepted = 0;
            starTotal = 0;

            loadMapped(file, ++fn, numFiles);

            std::cout << "File " << file.filename() << ": " << fn << "/" << numFiles
                      << " --> " << starAccepted << "/" << starTotal << " stars added" << std::endl;
        }
    }
    else
    {
        starAccepted = 0;
        starTotal = 0;

        loadMapped(file, -1, -1);

        std::cout << "File " << file.filename() << ": " << " --> "
            << starAccepted << "/" << starTotal << " stars added" << std::endl;
    }
}