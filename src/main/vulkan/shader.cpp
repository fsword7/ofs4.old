
#include "main/core.h"
#include "main/vulkan/context.h"
#include "main/vulkan/shader.h"

using namespace osd::vk;

bool Shader::readShaderFile(fs::path fname, code_t &code)
{
    std::ifstream file(fname, std::ios::ate|std::ios::binary);

    if (!file.is_open())
        return false;

    off_t size = file.tellg();
    code.resize(size);

    // Loading shader image
    file.seekg(0);
    file.read(code.data(), size);
    file.close();

    return true;
}

void Shader::loadShaders(fs::path fname)
{

}

void Shader::createVertexBinding()
{
    vertexBinding = Vertex::getBindingDescription();
}

void Shader::createVertexAttributes()
{
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    for (VkVertexInputAttributeDescription &description : vertexAttributes)
        vertexAttributes.push_back(description);
}

void Shader::createVertexInputs()
{
    createVertexBinding();
    createVertexAttributes();

    inputInfo = {};
    inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputInfo.vertexBindingDescriptionCount = 1;
    inputInfo.pVertexBindingDescriptions = &vertexBinding;
    inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
    inputInfo.pVertexAttributeDescriptions = vertexAttributes.data();
}

void Shader::createInputAssembly()
{
    assemblyInfo = {};
    assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    assemblyInfo.primitiveRestartEnable = VK_FALSE;
}

void Shader::createViewportState()
{
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = ctx->getSurfaceExtent().width;
    viewport.height = ctx->getSurfaceExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    viewports.push_back(viewport);

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = ctx->getSurfaceExtent();
    scissors.push_back(scissor);

    viewportInfo = {};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = static_cast<uint32_t>(viewports.size());
    viewportInfo.pViewports = viewports.data();
    viewportInfo.scissorCount = static_cast<uint32_t>(scissors.size());
    viewportInfo.pScissors = scissors.data();
}

void Shader::createRasterizerState()
{
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f;
    rasterizerInfo.depthBiasClamp = 0.0f;
    rasterizerInfo.depthBiasSlopeFactor = 0.0f;
}

void Shader::createMultisampleState()
{
    multisampleInfo = {};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = nullptr;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleInfo.alphaToOneEnable = VK_FALSE;
}

void Shader::createDepthStencilState()
{
    depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.depthTestEnable = VK_TRUE;
    depthStencilInfo.depthWriteEnable = VK_TRUE;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilInfo.minDepthBounds = 0.0f;
    depthStencilInfo.maxDepthBounds = 1.0f;
    depthStencilInfo.stencilTestEnable = VK_FALSE;
    depthStencilInfo.front = {};
    depthStencilInfo.back = {};
}

void Shader::createBlendState()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachments.push_back(colorBlendAttachment);

    colorBlendInfo = {};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendInfo.attachmentCount = static_cast<uint32_t>(colorBlendAttachments.size());
    colorBlendInfo.pAttachments = colorBlendAttachments.data();
    colorBlendInfo.blendConstants[0] = 0.0f;
    colorBlendInfo.blendConstants[1] = 0.0f;
    colorBlendInfo.blendConstants[2] = 0.0f;
    colorBlendInfo.blendConstants[3] = 0.0f;
}

void Shader::createDynamicState()
{
    dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

    dynamicInfo = {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicInfo.pDynamicStates = dynamicStates.data();
}
