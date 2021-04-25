
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

void Context::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool) != VK_SUCCESS)
        throw std::runtime_error("Can't create command pool - aborted!");
}

VkCommandBuffer Context::beginSingleTimeCommands(VkCommandPool cmdPool)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer cmdBuffer;
    if (vkAllocateCommandBuffers(device, &allocInfo, &cmdBuffer) != VK_SUCCESS)
        throw std::runtime_error("Can't create command buffers - aborted!");
    
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS)
        throw std::runtime_error("Can't begin command buffers - aborted!");

    return cmdBuffer;
}

void Context::endSingleTimeCommands(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool)
{
    vkEndCommandBuffer(cmdBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &cmdBuffer;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr) != VK_SUCCESS)
        throw std::runtime_error("Can't submit command buffer to queue - aborted!");
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
}