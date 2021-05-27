
#pragma once

#include "main/vulkan/context.h"
#include "main/vulkan/shader.h"

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
        Pipeline(Context *ctx) : ctx(ctx), device(ctx->getLogicalDevice()) {}
        virtual ~Pipeline() = default;

        inline VkPipeline getPipeline() const { return pipeline; }
        
        virtual void init();
        virtual void cleanup();
        void rebuildSwapChain();

        // virtual void recordCommands(const VkCommandBuffer &cmdBuffer);
        virtual void render(VkSemaphore waitSignal);
        virtual void render(const VkCommandBuffer &cmdBuffer);

    protected:
        // void createRenderPass();
        // void createFramebuffers();
        void createPipeline();
        void createDescriptorSet();

    protected:
        Context *ctx = nullptr;

        VkDevice device = nullptr;

        // VkRenderPass renderPass;
        // std::vector<VkFramebuffer> frameBuffers;
        VkPipelineLayout pipelineLayout = nullptr;
        VkPipeline pipeline = nullptr;

        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet descriptorSet;

        std::vector<Descriptor> descriptorInfos;

        VkSubmitInfo submitInfo = {};

        Shader *shader = nullptr;
    };
}