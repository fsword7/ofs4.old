
#pragma once

#include "main/coreapp.h"
#include "main/vulkan/context.h"
#include "render/scene.h"

namespace ofs::main
{
    class coreApplication : public ofs::coreApplication
    {
    public:
        coreApplication() = default;
        ~coreApplication() = default;

        void init() override;
        void cleanup() override;
        void run() override;

    private:
        SDL_Window *window = nullptr;

        osd::vk::Context *ctx = nullptr;
        ofs::render::Scene *scene = nullptr;
    };
}