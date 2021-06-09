
#pragma once

#include "main/vulkan/context.h"

namespace osd::vk
{
    struct ShaderFiles
    {
        fs::path fname;
        VkShaderStageFlags flags;
    };

    class Shader
    {
    public:
        using code_t = std::vector<char>;

        Shader(Context *context, const fs::path &vertexFile, const fs::path &fragmentFile);
        virtual ~Shader();

        virtual void cleanup();

        void loadShaders(fs::path vertexFile, fs::path fragmentFile);
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
        VkShaderModule createShaderModule(const code_t &code);

        // Virtual function calls
        // virtual void loadShaders(fs::path fname);

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
        VkDevice device = nullptr;

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

        VkShaderModule vertexModule = nullptr;
        VkShaderModule fragmentModule = nullptr;
    };
}