
#include "main/core.h"
#include "main/vulkan/context.h"
#include "render/scene.h"

using namespace osd::vk;
using namespace ofs::render;

void Scene::init()
{
    demoPipeline = new PipelineTriangle(&ctx);
    // demoPipeline->init();
}

void Scene::render()
{
    VkCommandBuffer cmdBuffer = ctx.beginRender();

    ///////////////////////
    // Rendering codes here
    ///////////////////////

    ctx.endRender(cmdBuffer);
}