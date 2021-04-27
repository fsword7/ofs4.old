
#include "main/core.h"
#include "main/vulkan/context.h"
#include "main/vulkan/pipeline.h"

using namespace osd::vk;

Vertex vtxTriangle[3] =
{
    {{  0.0f, -0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f }},
    {{  0.5f,  0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }},
    {{ -0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f }}
};

uint32_t idxTriangle[3] = { 0, 1, 2 };

void Context::readShaderFile(const std::string &fname, char *&data, uint32_t &size)
{
    std::ifstream file(fname, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("Can't open file - aborted!");

    size = file.tellg();
    data = new char[size];

    file.seekg(0);
    file.read(data, size);
    file.close();
}

void Context::createPipeline()
{
    VkPipelineLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 0;
    layoutInfo.pSetLayouts = nullptr;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline layout - aborted!");

    char *vertShaderData = nullptr;
    uint32_t vertShaderSize = 0;
    readShaderFile("../../shaders/triangle-vert.spv", vertShaderData, vertShaderSize);

    char *fragShaderData = nullptr;
    uint32_t fragShaderSize = 0;
    readShaderFile("../../shaders/triangle-frag.spv", fragShaderData, fragShaderSize);

    VkShaderModuleCreateInfo vertShaderCreateInfo = {};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertShaderCreateInfo.codeSize = vertShaderSize;
    vertShaderCreateInfo.pCode = reinterpret_cast<uint32_t *>(vertShaderData);

    VkShaderModuleCreateInfo fragShaderCreateInfo = {};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragShaderCreateInfo.codeSize = fragShaderSize;
    fragShaderCreateInfo.pCode = reinterpret_cast<uint32_t *>(fragShaderData);

    if (vkCreateShaderModule(device, &vertShaderCreateInfo, nullptr, &vertShader) != VK_SUCCESS)
        throw std::runtime_error("Can't create vertex shader module - aborted!");
    if (vkCreateShaderModule(device, &fragShaderCreateInfo, nullptr, &fragShader) != VK_SUCCESS)
        throw std::runtime_error("Can't create fragment shader module - aborted!");
    
    delete vertShaderData;
    delete fragShaderData;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);

    shaderStages[0] = {};
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pName = "main";
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShader;

    shaderStages[1] = {};
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].pName = "main";
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShader;

    std::vector<VkVertexInputBindingDescription> vtxBindingDescriptions(1);
    vtxBindingDescriptions[0].binding = 0;
    vtxBindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vtxBindingDescriptions[0].stride = sizeof(Vertex);

    std::vector<VkVertexInputAttributeDescription> vtxAtttributeDescriptions(2);
    vtxAtttributeDescriptions[0].binding = 0;
    vtxAtttributeDescriptions[0].location = 0;
    vtxAtttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vtxAtttributeDescriptions[0].offset = offsetof(Vertex, position);

    vtxAtttributeDescriptions[1].binding = 0;
    vtxAtttributeDescriptions[1].location = 1;
    vtxAtttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vtxAtttributeDescriptions[1].offset = offsetof(Vertex, color);

    VkPipelineVertexInputStateCreateInfo inputInfo = {};
    inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vtxBindingDescriptions.size());
    inputInfo.pVertexBindingDescriptions = vtxBindingDescriptions.data();
    inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vtxAtttributeDescriptions.size());
    inputInfo.pVertexAttributeDescriptions = vtxAtttributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo assemblyInfo = {};
    assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    assemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo depth_stencil = {};
    depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachments[2];
    
    colorBlendAttachments[0] = {};
    colorBlendAttachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachments[0].blendEnable = VK_TRUE;
    colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;

    colorBlendAttachments[1] = {};
    colorBlendAttachments[1].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachments[1].blendEnable = VK_TRUE;
    colorBlendAttachments[1].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachments[1].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachments[1].colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachments[1].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachments[1].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachments[1].alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlend = {};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.logicOpEnable = VK_FALSE;
    colorBlend.logicOp = VK_LOGIC_OP_COPY;
    colorBlend.attachmentCount = 2;
    colorBlend.pAttachments = colorBlendAttachments;
    colorBlend.blendConstants[0] = 0.0f;
    colorBlend.blendConstants[1] = 0.0f;
    colorBlend.blendConstants[2] = 0.0f;
    colorBlend.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicInfo.pDynamicStates = dynamicStates.data();
    dynamicInfo.flags = 0;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &inputInfo;
    pipelineInfo.pInputAssemblyState = &assemblyInfo;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depth_stencil;
    pipelineInfo.pColorBlendState = &colorBlend;
    pipelineInfo.pDynamicState = &dynamicInfo;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;

    if (vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        throw std::runtime_error("Can't create graphics pipeline - aborted!");

    VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(gpuDevice, &props);

    vtxSize = sizeof(Vertex) * 3;
    createBuffer(device, props, vtxBuffer, vtxMemory, vtxSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    if (vkMapMemory(device, vtxMemory, 0, vtxSize, 0, &vtxMapped) != VK_SUCCESS)
        throw std::runtime_error("Can't map vertex buffer - aborted!");
    memcpy(vtxMapped, vtxTriangle, vtxSize);

    idxSize = sizeof(uint32_t) * 3;
    createBuffer(device, props, idxBuffer, idxMemory, idxSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    
    if (vkMapMemory(device, idxMemory, 0, idxSize, 0, &idxMapped) != VK_SUCCESS)
        throw std::runtime_error("Can't map index buffer - aborted!");
    memcpy(idxMapped, idxTriangle, idxSize);
}

void Context::cleanupPipeline()
{
    vkUnmapMemory(device, vtxMemory);
    vkDestroyBuffer(device, vtxBuffer, nullptr);
    vkFreeMemory(device, vtxMemory, nullptr);

    vkUnmapMemory(device, idxMemory);
    vkDestroyBuffer(device, idxBuffer, nullptr);
    vkFreeMemory(device, idxMemory, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyShaderModule(device, vertShader, nullptr);
    vkDestroyShaderModule(device, fragShader, nullptr);
}