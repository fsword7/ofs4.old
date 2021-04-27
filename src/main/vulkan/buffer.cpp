
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

uint32_t Context::findMemoryType(const VkPhysicalDeviceMemoryProperties &props, uint32_t typeFilter,
    VkMemoryPropertyFlags mpFlags)
{
    for (uint32_t idx = 0; idx < props.memoryTypeCount; idx++)
    {
        if (typeFilter & (1 << idx) && (props.memoryTypes[idx].propertyFlags & mpFlags) == mpFlags)
            return idx;
    }

    assert(0 && "No available memory properties");
    return -1;
}

void Context::createBuffer(const VkDevice &device, const VkPhysicalDeviceMemoryProperties &props,
    VkBuffer &buffer, VkDeviceMemory &memory, VkDeviceSize size, VkBufferUsageFlags usageFlags,
    VkSharingMode sharingMode, VkMemoryPropertyFlags mpFlags)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = sharingMode;
    bufferInfo.size = size;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        throw std::runtime_error("Can't create buffer - aborted!");

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(device, buffer, &requirements);

    uint32_t type = findMemoryType(props, requirements.memoryTypeBits, mpFlags);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = type;

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("Can't allocate buffer memory - aborted!");

    vkBindBufferMemory(device, buffer, memory, 0);
}