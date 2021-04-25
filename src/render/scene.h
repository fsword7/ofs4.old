
#pragma once

namespace ofs::render
{
    class Scene
    {
    public:
        Scene(osd::vk::Context &ctx);
        ~Scene();

        void render();

    private:
        osd::vk::Context &ctx;

    };
}