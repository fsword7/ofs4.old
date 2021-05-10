
#include "main/core.h"
#include "main/vulkan/pipeline.h"

using namespace osd::vk;

void Pipeline::init(Context *context)
{
    ctx = context;
    device = ctx->getLogicalDevice();


}

void Pipeline::cleanup()
{
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    for (int idx = 0; idx < frameBuffers.size(); idx++)
        vkDestroyFramebuffer(device, frameBuffers[idx], nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
}

void Pipeline::rebuildSwapChain()
{
    vkDestroyRenderPass(device, renderPass, nullptr);
    for (int idx = 0; idx < frameBuffers.size(); idx++)
        vkDestroyFramebuffer(device, frameBuffers[idx], nullptr);

    createRenderPass();
    createFramebuffers();
}

void Pipeline::createRenderPass()
{
    VkAttachmentDescription presentAttachment = {};
    presentAttachment.format = ctx->getSurfaceImageFormat();
    presentAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    presentAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    presentAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    presentAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    presentAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    presentAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    presentAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference presentAttachmentRef = {};
    presentAttachmentRef.attachment = 0;
    presentAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = ctx->getColorImageFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 1;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = ctx->getDepthImageFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 2;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


    std::array<VkAttachmentDescription, 3> attachments =
        { presentAttachment, colorAttachment, depthAttachment };
    std::array<VkAttachmentReference, 2> attachmentRefs =
        { presentAttachmentRef, colorAttachmentRef };

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentRefs.size());
    subpass.pColorAttachments = attachmentRefs.data();
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;
    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &subpassDependency;

    if (vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) != VK_SUCCESS)
        throw std::runtime_error("Can't create render pass - aborted!");
}

void Pipeline::createFramebuffers()
{
    const std::vector<VkImageView> &imageViews = ctx->getSurfaceImageViews();
    frameBuffers.resize(imageViews.size());

    for (int idx = 0; idx < imageViews.size(); idx++)
    {
        std::array<VkImageView, 3> attachments =
        {
            imageViews[idx],
            ctx->getColorImageView(idx),
            ctx->getDepthImageView(idx)
        };

        VkFramebufferCreateInfo frameInfo =  {};
        frameInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameInfo.renderPass = renderPass;
        frameInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameInfo.pAttachments = attachments.data();
        frameInfo.width = ctx->getSurfaceExtent().width;
        frameInfo.height = ctx->getSurfaceExtent().height;
        frameInfo.layers = 1;

        if (vkCreateFramebuffer(device, &frameInfo, nullptr, &frameBuffers[idx]) != VK_SUCCESS)
            throw std::runtime_error("Can't create frame buffers - aborted!");
    }
}

void Pipeline::createPipeline()
{
    // VkPipelineLayoutCreateInfo layoutInfo = {};
    // layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // layoutInfo.setLayoutCount = 0;
    // layoutInfo.pSetLayouts = nullptr;
    // layoutInfo.pushConstantRangeCount = 0;
    // layoutInfo.pPushConstantRanges = nullptr;

    // if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    //     throw std::runtime_error("Can't create pipeline layout - aborted!");

    // char *vertShaderData = nullptr;
    // uint32_t vertShaderSize = 0;
    // readShaderFile("../../shaders/triangle-vert.spv", vertShaderData, vertShaderSize);

    // char *fragShaderData = nullptr;
    // uint32_t fragShaderSize = 0;
    // readShaderFile("../../shaders/triangle-frag.spv", fragShaderData, fragShaderSize);

    // VkShaderModuleCreateInfo vertShaderCreateInfo = {};
    // vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    // vertShaderCreateInfo.codeSize = vertShaderSize;
    // vertShaderCreateInfo.pCode = reinterpret_cast<uint32_t *>(vertShaderData);

    // VkShaderModuleCreateInfo fragShaderCreateInfo = {};
    // fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    // fragShaderCreateInfo.codeSize = fragShaderSize;
    // fragShaderCreateInfo.pCode = reinterpret_cast<uint32_t *>(fragShaderData);

    // if (vkCreateShaderModule(device, &vertShaderCreateInfo, nullptr, &vertShader) != VK_SUCCESS)
    //     throw std::runtime_error("Can't create vertex shader module - aborted!");
    // if (vkCreateShaderModule(device, &fragShaderCreateInfo, nullptr, &fragShader) != VK_SUCCESS)
    //     throw std::runtime_error("Can't create fragment shader module - aborted!");
    
    // delete vertShaderData;
    // delete fragShaderData;

    // std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);

    // shaderStages[0] = {};
    // shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // shaderStages[0].pName = "main";
    // shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    // shaderStages[0].module = vertShader;

    // shaderStages[1] = {};
    // shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // shaderStages[1].pName = "main";
    // shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    // shaderStages[1].module = fragShader;

    // std::vector<VkVertexInputBindingDescription> vtxBindingDescriptions(1);
    // vtxBindingDescriptions[0].binding = 0;
    // vtxBindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // vtxBindingDescriptions[0].stride = sizeof(Vertex);

    // std::vector<VkVertexInputAttributeDescription> vtxAtttributeDescriptions(3);
    // vtxAtttributeDescriptions[0].binding = 0;
    // vtxAtttributeDescriptions[0].location = 0;
    // vtxAtttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    // vtxAtttributeDescriptions[0].offset = offsetof(Vertex, position);

    // vtxAtttributeDescriptions[1].binding = 0;
    // vtxAtttributeDescriptions[1].location = 1;
    // vtxAtttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    // vtxAtttributeDescriptions[1].offset = offsetof(Vertex, color);

    // vtxAtttributeDescriptions[2].binding = 0;
    // vtxAtttributeDescriptions[2].location = 2;
    // vtxAtttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    // vtxAtttributeDescriptions[2].offset = offsetof(Vertex, texcoord);

    // VkPipelineVertexInputStateCreateInfo inputInfo = {};
    // inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // inputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vtxBindingDescriptions.size());
    // inputInfo.pVertexBindingDescriptions = vtxBindingDescriptions.data();
    // inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vtxAtttributeDescriptions.size());
    // inputInfo.pVertexAttributeDescriptions = vtxAtttributeDescriptions.data();

    // VkPipelineInputAssemblyStateCreateInfo assemblyInfo = {};
    // assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    // assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // assemblyInfo.primitiveRestartEnable = VK_FALSE;

    // VkPipelineViewportStateCreateInfo viewportState = {};
    // viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    // viewportState.viewportCount = 1;
    // viewportState.scissorCount = 1;

    // VkPipelineRasterizationStateCreateInfo rasterizer = {};
    // rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    // rasterizer.depthClampEnable = VK_FALSE;
    // rasterizer.rasterizerDiscardEnable = VK_FALSE;
    // rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    // rasterizer.lineWidth = 1.0f;
    // rasterizer.cullMode = VK_CULL_MODE_NONE;
    // rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    // rasterizer.depthBiasEnable = VK_FALSE;
    // rasterizer.depthBiasConstantFactor = 0.0f;
    // rasterizer.depthBiasClamp = 0.0f;
    // rasterizer.depthBiasSlopeFactor = 0.0f;

    // VkPipelineMultisampleStateCreateInfo multisampling = {};
    // multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    // multisampling.sampleShadingEnable = VK_FALSE;
    // multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    // multisampling.minSampleShading = 1.0f;
    // multisampling.pSampleMask = nullptr;
    // multisampling.alphaToCoverageEnable = VK_FALSE;
    // multisampling.alphaToOneEnable = VK_FALSE;

    // VkPipelineDepthStencilStateCreateInfo depth_stencil = {};
    // depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    // depth_stencil.depthTestEnable = VK_TRUE;
    // depth_stencil.depthWriteEnable = VK_TRUE;
    // depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    // depth_stencil.depthBoundsTestEnable = VK_FALSE;
    // depth_stencil.stencilTestEnable = VK_FALSE;

    // VkPipelineColorBlendAttachmentState colorBlendAttachments[2];
    
    // colorBlendAttachments[0] = {};
    // colorBlendAttachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    //                                           VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // colorBlendAttachments[0].blendEnable = VK_TRUE;
    // colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    // colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;

    // colorBlendAttachments[1] = {};
    // colorBlendAttachments[1].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    //                                           VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // colorBlendAttachments[1].blendEnable = VK_TRUE;
    // colorBlendAttachments[1].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    // colorBlendAttachments[1].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachments[1].colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachments[1].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachments[1].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachments[1].alphaBlendOp = VK_BLEND_OP_ADD;

    // VkPipelineColorBlendStateCreateInfo colorBlend = {};
    // colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // colorBlend.logicOpEnable = VK_FALSE;
    // colorBlend.logicOp = VK_LOGIC_OP_COPY;
    // colorBlend.attachmentCount = 2;
    // colorBlend.pAttachments = colorBlendAttachments;
    // colorBlend.blendConstants[0] = 0.0f;
    // colorBlend.blendConstants[1] = 0.0f;
    // colorBlend.blendConstants[2] = 0.0f;
    // colorBlend.blendConstants[3] = 0.0f;

    // std::vector<VkDynamicState> dynamicStates =
    // {
    //     VK_DYNAMIC_STATE_VIEWPORT,
    //     VK_DYNAMIC_STATE_SCISSOR,
    //     VK_DYNAMIC_STATE_LINE_WIDTH
    // };

    // VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    // dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    // dynamicInfo.pDynamicStates = dynamicStates.data();
    // dynamicInfo.flags = 0;

    // VkGraphicsPipelineCreateInfo pipelineInfo = {};
    // pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    // pipelineInfo.pStages = shaderStages.data();
    // pipelineInfo.pVertexInputState = &inputInfo;
    // pipelineInfo.pInputAssemblyState = &assemblyInfo;
    // pipelineInfo.pViewportState = &viewportState;
    // pipelineInfo.pRasterizationState = &rasterizer;
    // pipelineInfo.pMultisampleState = &multisampling;
    // pipelineInfo.pDepthStencilState = &depth_stencil;
    // pipelineInfo.pColorBlendState = &colorBlend;
    // pipelineInfo.pDynamicState = &dynamicInfo;
    // pipelineInfo.layout = pipelineLayout;
    // pipelineInfo.renderPass = renderPass;
    // pipelineInfo.subpass = 0;
    // pipelineInfo.basePipelineHandle = nullptr;
    // pipelineInfo.basePipelineIndex = -1;
    // pipelineInfo.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;

    // if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    //     throw std::runtime_error("Can't create graphics pipeline - aborted!");
}

void Pipeline::createDescriptorSet()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (Descriptor &descriptor : descriptorInfos)
        bindings.push_back(descriptor.layoutBinding);

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline descriptor set layout - aborted!");

    std::vector<VkDescriptorPoolSize> poolSizes;
    for (VkDescriptorSetLayoutBinding &binding : bindings)
    {
        bool found = false;

        for (VkDescriptorPoolSize &poolSize : poolSizes)
        {
            if (binding.descriptorType == poolSize.type)
            {
                poolSize.descriptorCount += binding.descriptorCount;
                found = true;
                break;
            }
        }

        if (!found)
        {
            VkDescriptorPoolSize poolSize;
            poolSize.type = binding.descriptorType;
            poolSize.descriptorCount = binding.descriptorCount;
            poolSizes.push_back(poolSize);
        }
    }

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 256;

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline descriptor pool - aborted!");

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
        throw std::runtime_error("Can't allocate pipeline descriptor set - aborted!");

    std::vector<VkWriteDescriptorSet> descriptorWrites = {};
    for (Descriptor &descriptor : descriptorInfos)
    {
        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = descriptor.layoutBinding.binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = descriptor.layoutBinding.descriptorType;
        descriptorWrite.descriptorCount = descriptor.layoutBinding.descriptorCount;

        if (descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
            descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
            descriptorWrite.pBufferInfo = descriptor.bufferInfos.data();
        else if (descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                 descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
                 descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER ||
                 descriptor.layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
            descriptorWrite.pImageInfo = descriptor.imageInfos.data();

        descriptorWrites.push_back(descriptorWrite);
    }

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}