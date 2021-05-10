
#pragma once

#include "main/vulkan/context.h"

namespace osd::vk
{
    class Texture
    {
    public:
        Texture() = default;
        ~Texture() = default;

        bool loadImage(fs::path fname);

        void createTexture(const VkPhysicalDevice &gpu, const VkDevice &device);

    protected:
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t size = width * height;

        std::vector<uint8_t> data;

        VkFormat format = VK_FORMAT_UNDEFINED;
        VkImage image = nullptr;
        VkImageView view = nullptr;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkDeviceMemory memory = nullptr;
        VkSampler sampler = nullptr;
    };
}