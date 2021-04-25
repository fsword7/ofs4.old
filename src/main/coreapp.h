
#pragma once

namespace ofs
{
    class coreApplication
    {
    public:
        coreApplication() = default;
        virtual ~coreApplication() = default;

        // Virtual function calls
        virtual void init() = 0;
        virtual void cleanup() = 0;
        virtual void run() = 0;
    };
}