
#include "main/core.h"
#include "data/gaia.h"

using namespace ofs::data;


bool GaiaDataProvider::addStar(const std::string &line)
{
    std::vector<std::string> tokens = split(line, ",");

    if (tokens[9].empty())
        return false;

    // Solution ID
    uint32_t solutionid = std::stof(tokens[2]);

    // Parallex
    double parallax = std::stod(tokens[9]);
    double plxerr = std::stod(tokens[10]);
    // double appmag = std::stod(tokens[0]);

    return true;
}

void GaiaDataProvider::loadFile(std::ifstream &inFile)
{
    std::string line;
    uint32_t ln = 0;
    while (std::getline(inFile, line))
    {
        if (ln > 0) // skip header line
        {
            if (addStar(line))
                starAccepted++;
            starTotal++;
        }
        ln++;
    }
}

void GaiaDataProvider::loadMapped(fs::path file, uint32_t fn, uint32_t totalFiles)
{
    if (file.extension() == ".gz")
        return;
    if (file.extension() != ".csv")
        return;
    std::ifstream inFile(file);

    if (!inFile.is_open())
        return;
    loadFile(inFile);

    inFile.close();
}

void GaiaDataProvider::load(fs::path file)
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
}