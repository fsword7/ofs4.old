
#include "main/core.h"
#include "data/gaia.h"
#include "data/hippacros.h"

using GaiaDataProvider = ofs::data::GaiaDataProvider;
using HippacrosDataProvider = ofs::data::HippacrosDataProvider;

int main(int argc, char **argv)
{
    GaiaDataProvider gaiaLoader;
    HippacrosDataProvider hipLoader;
    // TychoDataProvider tycLoader;
    std::vector<std::string> fields;

    std::cout << "Building particle database" << std::endl;

    // gaiaLoader.load("../../data/edr3");
    hipLoader.load("../../data/hip/hip_main.dat");
}