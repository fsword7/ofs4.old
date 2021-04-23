
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
