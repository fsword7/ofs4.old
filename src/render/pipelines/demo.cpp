
#include "main/core.h"
#include "render/pipelines/demo.h"

using namespace ofs::render;

uint32_t vtxObjectSize = 3;
Vertex vtxTriangle[3] =
{
    {{  0.0f, -0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f }},
    {{  0.5f,  0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }},
    {{ -0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f }}
};

uint32_t idxObjectSize = 3;
uint32_t idxTriangle[3] = { 0, 1, 2 };

// uint32_t vtxObjectSize = 4;
// Vertex vtxTriangle[4] =
// {
//     {{ -0.5f, -0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
//     {{ -0.5f,  0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},
//     {{  0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
//     {{  0.5f, -0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }}
// };

// uint32_t idxObjectSize = 6;
// uint32_t idxTriangle[6] = { 0, 1, 2,  0, 2, 3 };

void PipelineTriangle::init()
{
    VkPhysicalDevice gpuDevice = ctx->getPhysicalDevice();

    // createDescriptorSet();
    createPipeline();
 
    VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(gpuDevice, &props);

    vtxObject.size = sizeof(Vertex) * vtxObjectSize;
    vtxObject.entries = vtxObjectSize;
    ctx->createBuffer(device, props, vtxObject.buffer, vtxObject.memory, vtxObject.size,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    if (vkMapMemory(device, vtxObject.memory, 0, vtxObject.size, 0, &vtxObject.mapped) != VK_SUCCESS)
        throw std::runtime_error("Can't map vertex buffer - aborted!");
    memcpy(vtxObject.mapped, vtxTriangle, vtxObject.size);

    idxObject.size = sizeof(uint32_t) * idxObjectSize;
    idxObject.entries = idxObjectSize;
    ctx->createBuffer(device, props, idxObject.buffer, idxObject.memory, idxObject.size,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    
    if (vkMapMemory(device, idxObject.memory, 0, idxObject.size, 0, &idxObject.mapped) != VK_SUCCESS)
        throw std::runtime_error("Can't map index buffer - aborted!");
    memcpy(idxObject.mapped, idxTriangle, idxObject.size);
}

void PipelineTriangle::cleanup()
{
    vkUnmapMemory(device, vtxObject.memory);
    vkDestroyBuffer(device, vtxObject.buffer, nullptr);
    vkFreeMemory(device, vtxObject.memory, nullptr);

    vkUnmapMemory(device, idxObject.memory);
    vkDestroyBuffer(device, idxObject.buffer, nullptr);
    vkFreeMemory(device, idxObject.memory, nullptr);

    // vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    // vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}


void PipelineTriangle::render(const VkCommandBuffer &cmdBuffer)
{
    VkRect2D scissor = {};
    scissor.extent = ctx->getSurfaceExtent();
    scissor.offset = { 0, 0 };
 
    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = ctx->getSurfaceExtent().width;
    viewport.height = ctx->getSurfaceExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    VkDeviceSize offsets[] = { 0 };

    // vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

    // Draw a demo triangle
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vtxObject.buffer, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, idxObject.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, idxObject.entries, 1, 0, 0, 0);
}