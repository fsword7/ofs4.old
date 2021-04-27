
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

void Context::cleanupSwapChain(bool remove)
{
    stopRender();
    cleanupPipeline();
    
    for (int idx = 0; idx < frameBuffers.size(); idx++)
    {
        // Release all color attachment images
        vkDestroyImageView(device, frameAttachments[idx].color.view, nullptr);
        vkDestroyImage(device, frameAttachments[idx].color.image, nullptr);
        vkFreeMemory(device, frameAttachments[idx].color.memory, nullptr);

        // Release all depth attachment images
        vkDestroyImageView(device, frameAttachments[idx].depth.view, nullptr);
        vkDestroyImage(device, frameAttachments[idx].depth.image, nullptr);
        vkFreeMemory(device, frameAttachments[idx].depth.memory, nullptr);

        // Release frame buffer
        vkDestroyFramebuffer(device, frameBuffers[idx], nullptr);
    }

    vkDestroyRenderPass(device, renderPass, nullptr);

    // Release all depth images
    // vkDestroyImageView(device, depthImageView, nullptr);
    // vkDestroyImage(device, depthImage, nullptr);
    // vkFreeMemory(device, depthImageMemory, nullptr);

    // Release all color images
    // vkDestroyImageView(device, colorImageView, nullptr);
    // vkDestroyImage(device, colorImage, nullptr);
    // vkFreeMemory(device, colorImageMemory, nullptr);

    // Release all surface images
    for (int idx = 0; idx < surfaceImageViews.size(); idx++)
        vkDestroyImageView(device, surfaceImageViews[idx], nullptr);

    if (remove == true)
        vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void Context::createSwapChain()
{
    // Save current swap chain for window changing
    VkSwapchainKHR oldSwapChain = swapChain;

    SwapChainSupportDetails details = querySwapChainSupport(gpuDevice, surface);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapChainSurfaceFormat(details.formats);
    VkPresentModeKHR presentMode = chooseSwapChainPresentMode(details.presentModes);
    VkExtent2D extent = chooseSwapChainExtent(details.capabiliies);

    uint32_t imageCount = details.capabiliies.minImageCount + 1;
    if (details.capabiliies.maxImageCount > 0 &&
        imageCount > details.capabiliies.maxImageCount)
        imageCount = details.capabiliies.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = details.capabiliies.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapChain;

    uint32_t queueFamilies[] =
    { 
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily.value() != indices.presentFamily.value())
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilies;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
        throw std::runtime_error("Can't create swap chain - aborted!");

    if (oldSwapChain != nullptr)
    {
        vkDeviceWaitIdle(device);
        vkDestroySwapchainKHR(device, oldSwapChain, nullptr);

        // Clean old resouces up
        cleanupSwapChain(false);
    }

    // Gather surface image spaces for swap chain process
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    surfaceImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, surfaceImages.data());

    surfaceImageExtent = extent;
    surfaceImageFormat = surfaceFormat.format;
    colorImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
    
    // Set up new resouces with new swap chain
    createImageViews();
    // createColorResources();
    // createDepthResources();
    createRenderPass();
    createFrameBuffers();
    createPipeline();
    startRender();
}

void Context::createImageViews()
{
    surfaceImageViews.resize(surfaceImages.size());

    for (int idx = 0; idx < surfaceImages.size(); idx++)
        surfaceImageViews[idx] = createImageView(surfaceImages[idx],
            surfaceImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

// void Context::createColorResources()
// {
//     createImage(surfaceImageExtent.width, surfaceImageExtent.height, colorImageFormat,
//         VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT |
//         VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
//         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
//         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
//     colorImageView = createImageView(colorImage, colorImageFormat,
//         VK_IMAGE_ASPECT_COLOR_BIT);
//
//     convertImageLayout(colorImage, colorImageFormat, VK_IMAGE_LAYOUT_UNDEFINED,
//         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
// }

VkFormat Context::findDepthFormat()
{
    return findSupportedFormats( { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

// void Context::createDepthResources()
// {
//     depthImageFormat = findDepthFormat();
//
//     createImage(surfaceImageExtent.width, surfaceImageExtent.height, depthImageFormat,
//         VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
//         VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//         depthImage, depthImageMemory);
//     depthImageView = createImageView(depthImage, depthImageFormat,
//         VK_IMAGE_ASPECT_DEPTH_BIT);
//
//     convertImageLayout(depthImage, depthImageFormat, VK_IMAGE_LAYOUT_UNDEFINED,
//         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
// }

void Context::createRenderPass()
{
    VkAttachmentDescription presentAttachment = {};
    presentAttachment.format = surfaceImageFormat;
    presentAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    presentAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    presentAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    presentAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    presentAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    presentAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    presentAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference presentAttachmentRef = {};
    presentAttachmentRef.attachment = 0;
    presentAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = colorImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 1;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 2;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


    std::array<VkAttachmentDescription, 3> attachments =
        { presentAttachment, colorAttachment, depthAttachment };
    std::array<VkAttachmentReference, 2> attachmentRefs =
        { presentAttachmentRef, colorAttachmentRef };

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentRefs.size());
    subpass.pColorAttachments = attachmentRefs.data();
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;
    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &subpassDependency;

    if (vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) != VK_SUCCESS)
        throw std::runtime_error("Can't create render pass - aborted!");
}

void Context::createFrameBuffers()
{   
    frameBuffers.resize(surfaceImages.size());
    frameAttachments.resize(surfaceImages.size());
    VkFormat depthFormat = findDepthFormat();

    for (int idx = 0; idx < surfaceImages.size(); idx++)
    {
        createAttachmentImage(colorImageFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            frameAttachments[idx].color);

        createAttachmentImage(depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT, frameAttachments[idx].depth);

        std::vector<VkImageView> attachments =
        {
            surfaceImageViews[idx],
            frameAttachments[idx].color.view,
            frameAttachments[idx].depth.view
        };

        VkFramebufferCreateInfo frameInfo = {};
        frameInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameInfo.renderPass = renderPass;
        frameInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameInfo.pAttachments = attachments.data();
        frameInfo.width = static_cast<uint32_t>(surfaceImageExtent.width);
        frameInfo.height = static_cast<uint32_t>(surfaceImageExtent.height);
        frameInfo.layers = 1;

        if (vkCreateFramebuffer(device, &frameInfo, nullptr, &frameBuffers[idx]) != VK_SUCCESS)
            throw std::runtime_error("Can't create frame buffer - aborted!");
    }
}
