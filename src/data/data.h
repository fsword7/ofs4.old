
#include "utils/fileio.h"

#pragma once

namespace ofs::data
{
    using namespace utils;

    class DataProvider
    {
        public:
            DataProvider() = default;
            ~DataProvider() = default;

            std::vector<std::string> split(const std::string &line, const std::string &separator);

        protected:
            // FileIO ioFile = nullptr;
    };
}