
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

uint32_t Context::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags mpFlags, VkDeviceSize memSize)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(gpuDevice, &memProperties);

    for (uint32_t idx = 0; idx < memProperties.memoryHeapCount; idx++)
    {
        if ((typeFilter & (1 << idx)) &&
            ((memProperties.memoryTypes[idx].propertyFlags & mpFlags) == mpFlags) &&
             (memProperties.memoryHeaps[memProperties.memoryTypes[idx].heapIndex].size >= memSize))
            return idx;
    }

    throw std::runtime_error("Can't find suitable memory type - aborted!");
}

void Context::createImage(uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags mpFlags,
    VkImage &image, VkDeviceMemory &imageMemory)
{
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usageFlags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
        throw std::runtime_error("Can't create image - aborted!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
        mpFlags, memRequirements.size);
    
    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        throw std::runtime_error("Can't allocate image memory - aborted!");

    vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView Context::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        throw std::runtime_error("Can't create image view - aborted!");

    return imageView;
}

void Context::convertImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	// VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	// VkImageMemoryBarrier barrier = {};
	// barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	// barrier.oldLayout = oldLayout;
	// barrier.newLayout = newLayout;
	// barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	// barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	// barrier.image = image;
	// barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	// barrier.subresourceRange.baseMipLevel = 0;
	// barrier.subresourceRange.levelCount = 1;
	// barrier.subresourceRange.baseArrayLayer = 0;
	// barrier.subresourceRange.layerCount = 1;

	// if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
	// 	newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	// {
	// 	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	// 	if (hasStencilComponent(format))
	// 		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	// }

	// VkPipelineStageFlags srcStage, dstStage;

	// // Set up source properties
	// if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
	// {
	// 	barrier.srcAccessMask = 0;
	// 	srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
	//               	  	  	    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
	//               	  	  	    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// }
	// else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL)
	// {
	// 	barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT |
	// 							VK_ACCESS_SHADER_WRITE_BIT;
	// 	srcStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
	// }
	// else
	// 	throw std::runtime_error("Unsupported source layout transition!");

	// // Set up destination properties
	// if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
	// 							VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
	// 							VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// }
	// else if (newLayout == VK_IMAGE_LAYOUT_GENERAL)
	// {
	// 	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT |
	// 							VK_ACCESS_SHADER_WRITE_BIT;
	// 	dstStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
	// }
	// else
	// 	throw std::runtime_error("Unsupported destination layout transition!");

	// vkCmdPipelineBarrier(commandBuffer,
	// 	srcStage, dstStage, 0,
	// 	0, nullptr, 0, nullptr,
	// 	1, &barrier);

	// endSingleTimeCommands(commandBuffer);
}
