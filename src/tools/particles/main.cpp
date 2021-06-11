
#include "main/core.h"
#include "data/gaia.h"

using GaiaDataProvider = ofs::data::GaiaDataProvider;

int main(int argc, char **argv)
{
    GaiaDataProvider loader;
    std::vector<std::string> fields;

    std::cout << "Building particle database" << std::endl;

    loader.load("../../data/edr3");
}