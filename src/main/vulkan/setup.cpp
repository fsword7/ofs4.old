
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

static std::vector<cchar_t *> validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<cchar_t *> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void Context::init()
{
    createInstance();
    createSurface(instance);
    getPhysicalGPUDevice();
    createLogicalDevice();
}

void Context::cleanup()
{
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void Context::createInstance()
{
    uint32_t extensionCount;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<cchar_t *> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APP_NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "OFS engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledExtensionCount = extensionNames.size();
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("Can't create instance - aborted!");
}

void Context::getPhysicalGPUDevice()
{
    uint32_t gpuCount;
    vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
    if (gpuCount == 0)
        throw std::runtime_error("Can't find existing GPU device - aborted!");
    std::vector<VkPhysicalDevice> devices(gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, devices.data());

    gpuDevice = devices[0];
}


void Context::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(gpuDevice);
    float queuePriority = 1.0f;

    // std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pQueuePriorities = &queuePriority;
    queueInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueInfo.queueCount = 1;

    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    if (vkCreateDevice(gpuDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        throw std::runtime_error("Can't create logical device - aborted!");

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}