
#include "main/core.h"
#include "utils/fileio.h"

int main(int argc, char **argv)
{
    utils::DataLoader loader;
    std::vector<std::string> fields;

    std::cout << "Building particle database" << std::endl;

    // fields = loader.split("abc:def::ghi", ":");
    // for (auto field : fields)
    //     std::cout << field << std::endl;

    // loader.loadMoreData("../../data/edr3");
}