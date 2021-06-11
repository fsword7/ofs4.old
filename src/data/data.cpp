
#include "main/core.h"
#include "data/data.h"

using namespace ofs::data;

std::vector<std::string> DataProvider::split(const std::string &line, const std::string &delimitor)
{
    std::vector<std::string> fields;
    size_t next, pos = 0;

    while ((next = line.find(delimitor, pos)) != std::string::npos)
    {
        fields.push_back(line.substr(pos, next - pos));
        pos = next + 1;
    }
    fields.push_back(line.substr(pos));

    return fields;
}