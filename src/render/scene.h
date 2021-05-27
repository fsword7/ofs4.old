
#pragma once

#include "render/pipelines/demo.h"

namespace ofs::render
{
    using namespace osd::vk;

    class Scene
    {
    public:
        Scene(osd::vk::Context &context)
        : ctx(context)
        { }
        
        ~Scene() = default;

        void init();
        void render();

    private:
        osd::vk::Context &ctx;

        Pipeline *demoPipeline = nullptr;

    };
}