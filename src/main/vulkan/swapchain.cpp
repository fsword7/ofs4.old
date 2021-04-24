
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

void Context::cleanupSwapChain(bool remove)
{
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

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    colorImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
    
    // Set up new resouces with new swap chain

}

void Context::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());

    for (int idx = 0; idx < swapChainImages.size(); idx++)
        swapChainImageViews[idx] = createImageView(swapChainImages[idx],
            swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Context::createColorResources()
{
    createImage(swapChainExtent.width, swapChainExtent.height, colorImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
    colorImageView = createImageView(colorImage, colorImageFormat,
        VK_IMAGE_ASPECT_COLOR_BIT);

    convertImageLayout(colorImage, colorImageFormat, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
}

void Context::createDepthResources()
{
    createImage(swapChainExtent.width, swapChainExtent.height, depthImageFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        depthImage, depthImageMemory);
    depthImageView = createImageView(depthImage, depthImageFormat,
        VK_IMAGE_ASPECT_DEPTH_BIT);

    convertImageLayout(depthImage, depthImageFormat, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}