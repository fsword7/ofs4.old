
#include "main/core.h"
#include "main/vulkan/pipeline.h"

using namespace osd::vk;

void Pipeline::init()
{
    // ctx = context;
    // device = ctx->getLogicalDevice();

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
}

void Pipeline::cleanup()
{
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);

    // for (int idx = 0; idx < frameBuffers.size(); idx++)
    //     vkDestroyFramebuffer(device, frameBuffers[idx], nullptr);
    // vkDestroyRenderPass(device, renderPass, nullptr);
}

void Pipeline::rebuildSwapChain()
{
    // vkDestroyRenderPass(device, renderPass, nullptr);
    // for (int idx = 0; idx < frameBuffers.size(); idx++)
    //     vkDestroyFramebuffer(device, frameBuffers[idx], nullptr);

    // createRenderPass();
    // createFramebuffers();
}

void Pipeline::render(VkSemaphore renderSignal)
{
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // submitInfo.waitSemaphoreCount = 1;
    // submitInfo.pWaitSemaphores = waitSignals;
    // submitInfo.pWaitDstStageMask = waitStages;

    VkSemaphore signals[] = { renderSignal };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signals;

    VkResult result = vkQueueSubmit(ctx->getGraphicsQueue(), 1, &submitInfo, nullptr);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Can't submit render command buffer - aborted!");
}

void Pipeline::render(const VkCommandBuffer &cmdBuffer)
{

    // vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

}

// void Pipeline::createRenderPass()
// {
//     VkAttachmentDescription presentAttachment = {};
//     presentAttachment.format = ctx->getSurfaceImageFormat();
//     presentAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//     presentAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//     presentAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//     presentAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//     presentAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     presentAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//     presentAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

//     VkAttachmentReference presentAttachmentRef = {};
//     presentAttachmentRef.attachment = 0;
//     presentAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


//     VkAttachmentDescription colorAttachment = {};
//     colorAttachment.format = ctx->getColorImageFormat();
//     colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//     colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//     colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//     colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//     colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

//     VkAttachmentReference colorAttachmentRef = {};
//     colorAttachmentRef.attachment = 1;
//     colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


//     VkAttachmentDescription depthAttachment = {};
//     depthAttachment.format = ctx->getDepthImageFormat();
//     depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//     depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//     depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//     depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//     depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

//     VkAttachmentReference depthAttachmentRef = {};
//     depthAttachmentRef.attachment = 2;
//     depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


//     std::array<VkAttachmentDescription, 3> attachments =
//         { presentAttachment, colorAttachment, depthAttachment };
//     std::array<VkAttachmentReference, 2> attachmentRefs =
//         { presentAttachmentRef, colorAttachmentRef };

//     VkSubpassDescription subpass = {};
//     subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//     subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentRefs.size());
//     subpass.pColorAttachments = attachmentRefs.data();
//     subpass.pDepthStencilAttachment = &depthAttachmentRef;

//     VkSubpassDependency subpassDependency = {};
//     subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//     subpassDependency.dstSubpass = 0;
//     subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//     subpassDependency.srcAccessMask = 0;
//     subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//     subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
//                                       VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//     subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

//     VkRenderPassCreateInfo createInfo = {};
//     createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//     createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
//     createInfo.pAttachments = attachments.data();
//     createInfo.subpassCount = 1;
//     createInfo.pSubpasses = &subpass;
//     createInfo.dependencyCount = 1;
//     createInfo.pDependencies = &subpassDependency;

//     if (vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) != VK_SUCCESS)
//         throw std::runtime_error("Can't create render pass - aborted!");
// }

// void Pipeline::createFramebuffers()
// {
//     const std::vector<VkImageView> &imageViews = ctx->getSurfaceImageViews();
//     frameBuffers.resize(imageViews.size());

//     for (int idx = 0; idx < imageViews.size(); idx++)
//     {
//         std::array<VkImageView, 3> attachments =
//         {
//             imageViews[idx],
//             ctx->getColorImageView(idx),
//             ctx->getDepthImageView(idx)
//         };

//         VkFramebufferCreateInfo frameInfo =  {};
//         frameInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         frameInfo.renderPass = renderPass;
//         frameInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
//         frameInfo.pAttachments = attachments.data();
//         frameInfo.width = ctx->getSurfaceExtent().width;
//         frameInfo.height = ctx->getSurfaceExtent().height;
//         frameInfo.layers = 1;

//         if (vkCreateFramebuffer(device, &frameInfo, nullptr, &frameBuffers[idx]) != VK_SUCCESS)
//             throw std::runtime_error("Can't create frame buffers - aborted!");
//     }
// }

void Pipeline::createPipeline()
{
    VkPipelineLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // layoutInfo.setLayoutCount = 1;
    // layoutInfo.pSetLayouts = &descriptorSetLayout;
    layoutInfo.setLayoutCount = 0;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;
   
    if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline layout - aborted!");
   
    assert(shader != nullptr);

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shader->getShaderStageInfos().size();
    pipelineInfo.pStages = shader->getShaderStageInfos().data();
    pipelineInfo.pVertexInputState = &shader->getVertexInputInfo();
    pipelineInfo.pInputAssemblyState = &shader->getInputAssemblyInfo();
    pipelineInfo.pViewportState = &shader->getViewportStateInfo();
    pipelineInfo.pRasterizationState = &shader->getRasterizerInfo();
    pipelineInfo.pMultisampleState = &shader->getMultisampleInfo();
    pipelineInfo.pDepthStencilState = &shader->getDepthStencilInfo();
    pipelineInfo.pColorBlendState = &shader->getColorBlendInfo();
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = ctx->getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.flags = 0;

    if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
        throw std::runtime_error("Can't create graphics pipeline - aborted!");

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