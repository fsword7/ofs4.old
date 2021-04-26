
#pragma once

namespace ofs::render
{
    class Scene
    {
    public:
        Scene(osd::vk::Context &context)
        : ctx(context)
        { }
        
        ~Scene() = default;

        void render();

    private:
        osd::vk::Context &ctx;

    };
}