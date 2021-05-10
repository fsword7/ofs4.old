
#pragma once

#include "main/vulkan/context.h"

namespace osd::vk
{
    class Pipeline
    {
    protected:
        struct Descriptor
        {
            std::vector<VkDescriptorBufferInfo> bufferInfos;
            std::vector<VkDescriptorImageInfo> imageInfos;
            VkDescriptorSetLayoutBinding layoutBinding;
        };

    public:
        Pipeline() = default;
        virtual ~Pipeline() = default;

        inline VkPipeline getPipeline() const { return pipeline; }
        
        void init(Context *context);
        void cleanup();
        void rebuildSwapChain();

    protected:
        void createRenderPass();
        void createFramebuffers();
        void createPipeline();
        void createDescriptorSet();

    protected:
        Context *ctx = nullptr;

        VkDevice device = nullptr;

        VkRenderPass renderPass;
        std::vector<VkFramebuffer> frameBuffers;
        VkPipelineLayout pipelineLayout = nullptr;
        VkPipeline pipeline = nullptr;

        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet descriptorSet;

        std::vector<Descriptor> descriptorInfos;
    };
}