
#pragma once

#include "main/vulkan/context.h"

namespace osd::vk
{
    class Shader
    {
    public:
        using code_t = std::vector<char>;

        Shader() = default;
        ~Shader() = default;

        bool readShaderFile(fs::path fname, code_t &image);

    private:
        VkShaderModule shader = nullptr;
    };
}