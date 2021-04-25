
#include "main/core.h"
#include "main/vulkan/context.h"
#include "render/scene.h"

using namespace osd::vk;
using namespace ofs::render;

Scene::Scene(Context &context)
: ctx(context)
{
    ctx.initRender();
}

Scene::~Scene()
{
    ctx.cleanupRender();
}

void Scene::render()
{
    ctx.beginRender();

    ///////////////////////
    // Rendering codes here
    ///////////////////////

    ctx.endRender();
}