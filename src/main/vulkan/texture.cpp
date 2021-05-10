
#include "main/core.h"
#include "main/vulkan/context.h"
#include "main/vulkan/texture.h"

using namespace osd::vk;

bool Texture::loadImage(fs::path fname)
{
    return false;
}

void Texture::createTexture(const VkPhysicalDevice &gpu, const VkDevice &device)
{

}

// ******************************

void Context::createTexture(const VkPhysicalDevice &gpu, const VkDevice &device)
{
    std::vector<uint8_t> imageData;

    VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(gpu, &props);

    txObject.buffer.size = 0;
    txObject.buffer.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    txObject.buffer.mode = VK_SHARING_MODE_EXCLUSIVE;
    txObject.buffer.mpFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    createBuffer(device, props, txObject.buffer.buffer, txObject.buffer.memory,
        txObject.buffer.size, txObject.buffer.usageFlags, txObject.buffer.mode,
        txObject.buffer.mpFlags);

    if (vkMapMemory(device, txObject.buffer.memory,  0, txObject.buffer.size, 0, &txObject.buffer.mapped) != VK_SUCCESS)
        throw std::runtime_error("Can't map GPU space for texture - aborted!");
    memcpy(txObject.buffer.mapped, imageData.data(), txObject.buffer.size);
    vkUnmapMemory(device, txObject.buffer.memory);

    txObject.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    txObject.format = VK_FORMAT_R8G8B8A8_UNORM;

    createImage(txObject.width, txObject.height, txObject.format, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, txObject.image, txObject.memory,
        VK_IMAGE_LAYOUT_UNDEFINED);

    VkCommandBuffer cmdBuffer = beginSingleTimeCommands(cmdPool);

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;


    VkBufferImageCopy bufferCopyRegion = {};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.mipLevel = 0;
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width = txObject.width;
    bufferCopyRegion.imageExtent.height = txObject.height;
    bufferCopyRegion.imageExtent.depth = 1;
    bufferCopyRegion.bufferOffset = 0;

    vkCmdCopyBufferToImage(cmdBuffer, txObject.buffer.buffer, txObject.image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
    vkDestroyBuffer(device, txObject.buffer.buffer, nullptr);

    convertImageLayout(cmdBuffer, txObject.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        txObject.layout, subresourceRange);

    endSingleTimeCommands(cmdBuffer, cmdPool);

    createImageSampler(device, txObject.image, txObject.format,
        txObject.view, txObject.sampler);

    const uint32_t descriptorPoolSizeCount = 1;

    std::vector<VkDescriptorPoolSize> txPoolSizes(1);
    txPoolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    txPoolSizes[0].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(txPoolSizes.size());
    poolInfo.pPoolSizes = txPoolSizes.data();
    poolInfo.maxSets = 1;

    VkDescriptorPool txPool;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &txPool) != VK_SUCCESS)
        throw std::runtime_error("Can't create descriptor pool - aborted!");

    std::vector<VkDescriptorSetLayoutBinding> txLayoutBindings(1);
    txLayoutBindings[0].binding = 0;
    txLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    txLayoutBindings[0].descriptorCount = 1;
    txLayoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(txLayoutBindings.size());
    layoutInfo.pBindings = txLayoutBindings.data();

    VkDescriptorSetLayout txDescriptorSetLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &txDescriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("Can't create descriptor set - aborted!");
    
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = txPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &txDescriptorSetLayout;

    VkDescriptorSet txDescriptorSet;
    if (vkAllocateDescriptorSets(device, &allocInfo, &txDescriptorSet) != VK_SUCCESS)
        throw std::runtime_error("Can't create descriptor set - aborted!");

    VkDescriptorImageInfo descriptorImageInfo;
    descriptorImageInfo.sampler = txObject.sampler;
    descriptorImageInfo.imageView = txObject.view;
    descriptorImageInfo.imageLayout = txObject.layout;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = txDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrite.pNext = nullptr;

    static const int offset = offsetof(VkWriteDescriptorSet, pImageInfo);
    VkDescriptorImageInfo **data = reinterpret_cast<VkDescriptorImageInfo **>(reinterpret_cast<uint8_t *>(&descriptorWrite) + offset);
    *data = &descriptorImageInfo;

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}