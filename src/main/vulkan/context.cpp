
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

QueueFamilyIndices Context::findQueueFamilies(VkPhysicalDevice gpuDevie)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount;

    assert(surface != nullptr);
    assert(gpuDevice != nullptr);

    vkGetPhysicalDeviceQueueFamilyProperties(gpuDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(gpuDevice, &queueFamilyCount, queueFamilies.data());

    int idx = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = idx;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(gpuDevice, idx, surface, &presentSupport);
        if (presentSupport == true)
            indices.presentFamily = idx;
        if (indices.isComplete())
            return indices;
        idx++;
    }

    return indices;
}

SwapChainSupportDetails Context::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabiliies);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface,
        &formatCount, nullptr);
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface,
            &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
        &presentModeCount, nullptr);
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
            &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR Context::chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
        return { VK_FORMAT_B8G8R8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

    for (const auto &format : availableFormats)
    {
        if (format.format == VK_FORMAT_B8G8R8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return availableFormats[0];
}

VkPresentModeKHR Context::chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &presentMode : availablePresentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentMode;
        else if (presentMode == VK_PRESENT_MODE_FIFO_KHR)
            mode = presentMode;
    }

    return mode;
}

VkExtent2D Context::chooseSwapChainExtent(const VkSurfaceCapabilitiesKHR capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;

    int width, height;
    getWindowSize(width, height);

    VkExtent2D acutalExtent =
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    acutalExtent.width = std::max(capabilities.currentExtent.width,
        std::min(capabilities.currentExtent.width, acutalExtent.width));
    acutalExtent.height = std::max(capabilities.currentExtent.height,
        std::min(capabilities.currentExtent.height, acutalExtent.height));
    
    return acutalExtent;
}
