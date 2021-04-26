
#include "main/core.h"
#include "main/vulkan/context.h"
#include "main/vulkan/pipeline.h"

using namespace osd::vk;

Vertex vtxTriangle[3] =
{
    {{  0.0f, -0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f }},
    {{  0.5f,  0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }},
    {{ -0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f }}
};

uint32_t indTriangle[3] = { 0, 1, 2 };

void Context::createPipeline()
{
    VkPipelineLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 0;
    layoutInfo.pSetLayouts = nullptr;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline layout - aborted!");

    
}