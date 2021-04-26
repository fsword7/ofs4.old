
#include "main/core.h"
#include "main/vulkan/context.h"
#include "render/scene.h"

using namespace osd::vk;
using namespace ofs::render;

void Scene::render()
{
    ctx.beginRender();

    ///////////////////////
    // Rendering codes here
    ///////////////////////

    ctx.endRender();
}