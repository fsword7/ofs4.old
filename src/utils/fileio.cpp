
#include "main/core.h"
#include "utils/fileio.h"

using namespace utils;

using std::filesystem::directory_iterator;

void DataLoader::loadFile(fs::path file)
{
    std::ifstream inFile(file);
    std::string line;

    if (!inFile.is_open())
        return;

    while (std::getline(inFile, line))
    {
        std::cout << line << std::endl;
    }

    inFile.close();
}

void DataLoader::loadMoreData(fs::path path)
{
    for (const auto &file : directory_iterator(path))
    {
        std::cout << file.path() << " => " << fs::path(file).extension() << std::endl;
        if (fs::path(file).extension() == ".csv")
            loadFile(fs::path(file));
    }
}

std::vector<std::string> DataLoader::split(const std::string &line, const std::string &delimitor)
{
    std::vector<std::string> fields;
    size_t next, pos = 0;

    while ((next = line.find(delimitor, pos)) != std::string::npos)
    {
        fields.push_back(line.substr(pos, next - pos));
        pos = next+1;
    }
    fields.push_back(line.substr(pos));

    return fields;
}