
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

void Context::startRender()
{
    fences.resize(surfaceImages.size());
    for (int idx = 0; idx < surfaceImages.size(); idx++)
    {
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if (vkCreateFence(device, &fenceInfo, nullptr, &fences[idx]) != VK_SUCCESS)
            throw std::runtime_error("Can't create fence - aborted!");
    }

    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &createInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS)
        throw std::runtime_error("Can't create image available semaphore - aborted!");
    if (vkCreateSemaphore(device, &createInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
        throw std::runtime_error("Can't create render finished semaphoore - aborted!");

    graphicsCommandBuffers.resize(surfaceImages.size());
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandBufferCount = static_cast<uint32_t>(graphicsCommandBuffers.size());
    allocInfo.commandPool = cmdPool;

     if (vkAllocateCommandBuffers(device, &allocInfo, graphicsCommandBuffers.data()) != VK_SUCCESS)
        throw std::runtime_error("Can't allocate graphics command buffers - aborted!");
    // beginGraphicsCommandBuffer(graphicsCommandBuffers);

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = &waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphore;
 
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pResults = nullptr;
}

void Context::stopRender()
{
    vkFreeCommandBuffers(device, cmdPool, graphicsCommandBuffers.size(), graphicsCommandBuffers.data());

    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);

    for (int idx = 0; idx < fences.size(); idx++)
        vkDestroyFence(device, fences[idx], nullptr);
}

// void Context::beginGraphicsCommandBuffer(std::vector<VkCommandBuffer> &cmdBuffers)
// {
//     VkCommandBufferBeginInfo beginInfo = {};
//     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//     beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
//     beginInfo.pInheritanceInfo = nullptr;

//     VkClearValue clearValues[3] = {};
//     clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f }; // surface image
//     clearValues[1].color = { 0.0f, 0.0f, 0.0f, 1.0f }; // color image
//     clearValues[2].depthStencil = { 1.0f, 0 }; // depth image

//     VkRenderPassBeginInfo renderInfo = {};
//     renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     renderInfo.renderPass = renderPass;
//     renderInfo.renderArea.offset = { 0, 0 };
//     renderInfo.renderArea.extent = { surfaceImageExtent.width, surfaceImageExtent.height };
//     renderInfo.clearValueCount = 3;
//     renderInfo.pClearValues = clearValues;

//     VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

//     VkViewport viewport = {};
//     viewport.x = 0;
//     viewport.y = 0;
//     viewport.width = surfaceImageExtent.width;
//     viewport.height = surfaceImageExtent.height;
//     viewport.minDepth = 0.0f;
//     viewport.maxDepth = 1.0f;

//     VkRect2D scissor = {};
//     scissor.extent.width = surfaceImageExtent.width;
//     scissor.extent.height = surfaceImageExtent.height;
//     scissor.offset.x = 0;
//     scissor.offset.y = 0;

//     VkDeviceSize offsets[] = { 0 };

//     for (int idx = 0; idx < cmdBuffers.size(); idx++)
//     {
//         vkResetCommandBuffer(cmdBuffers[idx], 0);
//         renderInfo.framebuffer = frameBuffers[idx];

//         vkBeginCommandBuffer(cmdBuffers[idx], &beginInfo);

//         vkCmdBeginRenderPass(cmdBuffers[idx], &renderInfo, VK_SUBPASS_CONTENTS_INLINE);
//         vkCmdSetViewport(cmdBuffers[idx], 0, 1, &viewport);
//         vkCmdSetScissor(cmdBuffers[idx], 0, 1, &scissor);

//         // Draw a demo triangle
//         vkCmdBindPipeline(cmdBuffers[idx], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
//         vkCmdBindVertexBuffers(cmdBuffers[idx], 0, 1, &vtxObject.buffer, offsets);
//         vkCmdBindIndexBuffer(cmdBuffers[idx], idxObject.buffer, 0, VK_INDEX_TYPE_UINT32);
//         vkCmdDrawIndexed(cmdBuffers[idx], idxObject.entries, 1, 0, 0, 0);

//         // Rendering code here...

//         vkCmdEndRenderPass(cmdBuffers[idx]);
//         vkEndCommandBuffer(cmdBuffers[idx]);
//     }
// }

VkCommandBuffer Context::beginRender()
{
    vkWaitForFences(device, 1, &fences[imageIndex], VK_TRUE, UINT32_MAX);

    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
        imageAvailableSemaphore, nullptr, &imageIndex);

    vkQueueWaitIdle(graphicsQueue);
    vkResetFences(device, 1, &fences[imageIndex]);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    VkClearValue clearValues[3] = {};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f }; // surface image
    clearValues[1].color = { 0.0f, 0.0f, 0.0f, 1.0f }; // color image
    clearValues[2].depthStencil = { 1.0f, 0 }; // depth image

    VkRenderPassBeginInfo renderInfo = {};
    renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderInfo.renderPass = renderPass;
    renderInfo.framebuffer = frameBuffers[imageIndex];
    renderInfo.renderArea.offset = { 0, 0 };
    renderInfo.renderArea.extent = { surfaceImageExtent.width, surfaceImageExtent.height };
    renderInfo.clearValueCount = 3;
    renderInfo.pClearValues = clearValues;

    vkBeginCommandBuffer(graphicsCommandBuffers[imageIndex], &beginInfo);
    vkCmdBeginRenderPass(graphicsCommandBuffers[imageIndex], &renderInfo, VK_SUBPASS_CONTENTS_INLINE);

    return graphicsCommandBuffers[imageIndex];
}

void Context::endRender(VkCommandBuffer cmdBuffer)
{
    vkCmdEndRenderPass(cmdBuffer);
    vkEndCommandBuffer(cmdBuffer);
   
    submitInfo.pCommandBuffers = &graphicsCommandBuffers[imageIndex];
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, fences[imageIndex]);
    
    VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
    // Rebuild swap chain resources after window changed.
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
        createSwapChain();
    else if (result != VK_SUCCESS)
        throw std::runtime_error("Can't present swap chain image - aborted!");

    vkDeviceWaitIdle(device);
}

void Context::endRender(VkCommandBuffer cmdBuffer, VkSemaphore signal)
{
    vkCmdEndRenderPass(cmdBuffer);
    vkEndCommandBuffer(cmdBuffer);

    submitInfo.pCommandBuffers = &graphicsCommandBuffers[imageIndex];
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, fences[imageIndex]);
   
    VkSemaphore signals[] = { signal };
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signals;
 
    VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
    
    // Rebuild swap chain resources when window changed.
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        createSwapChain();
    else if (result != VK_SUCCESS)
        throw std::runtime_error("Can't present swap chain image - aborted!");

    vkDeviceWaitIdle(device);  
}