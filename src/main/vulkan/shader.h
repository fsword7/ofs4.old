
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

        virtual void cleanup();

        bool readShaderFile(fs::path fname, code_t &image);

        const std::vector<VkPipelineShaderStageCreateInfo> &getShaderStageInfos() const { return shaderStageInfos; }
        const VkVertexInputBindingDescription &getVertexBindingDescription() const { return vertexBinding; }
        const std::vector<VkVertexInputAttributeDescription> &getVertexAttributeDescription() const { return vertexAttributes; }
        const VkPipelineVertexInputStateCreateInfo &getVertexInputInfo() const { return inputInfo; }
        const VkPipelineInputAssemblyStateCreateInfo &getInputAssemblyInfo() const { return assemblyInfo; }

        const VkPipelineViewportStateCreateInfo &getViewportStateInfo() const { return viewportInfo; }
        const VkPipelineRasterizationStateCreateInfo &getRasterizerInfo() const { return rasterizerInfo; }
        const VkPipelineMultisampleStateCreateInfo &getMultisampleInfo() const { return multisampleInfo; }
        const VkPipelineDepthStencilStateCreateInfo &getDepthStencilInfo() const { return depthStencilInfo; }
        const VkPipelineColorBlendStateCreateInfo &getColorBlendInfo() const { return colorBlendInfo; }
        const VkPipelineDynamicStateCreateInfo &getDynamicInfo() const { return dynamicInfo; }

    protected:
        // Virtual function calls
        virtual void loadShaders(fs::path fname);

        virtual void createVertexBinding();
        virtual void createVertexAttributes();
        virtual void createVertexInputs();
        virtual void createInputAssembly();
        virtual void createViewportState();
        virtual void createRasterizerState();
        virtual void createMultisampleState();
        virtual void createDepthStencilState();
        virtual void createBlendState();
        virtual void createDynamicState();

    protected:
        Context *ctx = nullptr;

        std::vector<VkShaderModule> shaders;
        VkShaderModule vertexShaderModule = nullptr;
        VkShaderModule fragmentShaderModule = nullptr;

        std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
        VkVertexInputBindingDescription vertexBinding;
        std::vector<VkVertexInputAttributeDescription> vertexAttributes;
        VkPipelineVertexInputStateCreateInfo inputInfo;
        VkPipelineInputAssemblyStateCreateInfo assemblyInfo;

        std::vector<VkViewport> viewports;
        std::vector<VkRect2D> scissors;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineRasterizationStateCreateInfo rasterizerInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        std::vector<VkDynamicState> dynamicStates;
        VkPipelineDynamicStateCreateInfo dynamicInfo;
    };
}