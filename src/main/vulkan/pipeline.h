
#pragma once

#include "main/vulkan/context.h"

namespace osd::vk
{
    class Pipeline
    {
    public:
        Pipeline() = default;
        virtual ~Pipeline() = default;

    protected:
        VkPipelineLayout pipelineLayout = nullptr;
    };
}