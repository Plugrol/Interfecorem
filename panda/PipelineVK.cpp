#include "PipelineVK.h"
//#include <glm/glm.hpp>

core::graphic::vulkan::PipelineVK::PipelineVK() = default;

void core::graphic::vulkan::PipelineVK::init(const GameInfo &gameInfo, const VkDevice &device,
                                             const VulkanSwapchainFormat &format) {
	c_descriptorLayout(device);

	//Loading the shaders first.
	r_shaders(device);

	c_shaderStageInfo(0); //vertex
	c_shaderStageInfo(1); //fragment

	c_pipeline(gameInfo, device, format);
}

void core::graphic::vulkan::PipelineVK::destroy(const VkDevice &device) {
	if (this->m_pipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(device, this->m_pipeline, nullptr);
		this->m_renderPass.destroy(device);

		vkDestroyPipelineLayout(device, this->m_pipelineLayout, nullptr);
		this->m_shader.destroy(device);

		this->m_pipeline = VK_NULL_HANDLE;
		this->m_pipelineLayout = VK_NULL_HANDLE;
		this->m_shaderStages.clear();
	}
}

void core::graphic::vulkan::PipelineVK::c_descriptorLayout(const VkDevice &device) {
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
	descriptorSetLayoutBinding.binding = 0;
	descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = nullptr;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = 1;
	descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

	CATCH(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptiorSetLayout));
}

void core::graphic::vulkan::PipelineVK::c_pipeline(const GameInfo &gameInfo, const VkDevice &device,
                                                   const VulkanSwapchainFormat &format) {
	//GLSLVertex vertex(glm::vec2(0, 0), glm::vec3(0, 0, 0));
	auto vertexBindingDescription = this->m_shader.c_bindingDescription();
	auto inputDescriptions = this->m_shader.getVertexInputAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.pNext = nullptr;
	vertexInputCreateInfo.flags = 0;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.pVertexBindingDescriptions = &vertexBindingDescription;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = inputDescriptions.size();;
	vertexInputCreateInfo.pVertexAttributeDescriptions = inputDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo assemblyInputCreateInfo;
	assemblyInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyInputCreateInfo.pNext = nullptr;
	assemblyInputCreateInfo.flags = 0;
	assemblyInputCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assemblyInputCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = gameInfo.getWindowScale.width;
	viewport.height = gameInfo.getWindowScale.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = {gameInfo.getWindowScale.width, gameInfo.getWindowScale.height};

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.pNext = nullptr;
	viewportStateCreateInfo.flags = 0;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo;
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCreateInfo.pNext = nullptr;
	rasterizationCreateInfo.flags = 0;
	rasterizationCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationCreateInfo.depthBiasClamp = 0.0f;
	rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;
	rasterizationCreateInfo.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multiSampleStateCreateInfo;
	multiSampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multiSampleStateCreateInfo.pNext = nullptr;
	multiSampleStateCreateInfo.flags = 0;
	multiSampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multiSampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multiSampleStateCreateInfo.minSampleShading = 1.0f;
	multiSampleStateCreateInfo.pSampleMask = nullptr;
	multiSampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multiSampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
	                                      | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo;
	colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCreateInfo.pNext = nullptr;
	colorBlendCreateInfo.flags = 0;
	colorBlendCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendCreateInfo.attachmentCount = 1;
	colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
	colorBlendCreateInfo.blendConstants[0] = 0.0f;
	colorBlendCreateInfo.blendConstants[1] = 0.0f;
	colorBlendCreateInfo.blendConstants[2] = 0.0f;
	colorBlendCreateInfo.blendConstants[3] = 0.0f;

	VkDynamicState dynamicStates[] =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.pNext = nullptr;
	dynamicStateCreateInfo.flags = 0;
	dynamicStateCreateInfo.dynamicStateCount = 2;
	dynamicStateCreateInfo.pDynamicStates = dynamicStates;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &this->m_descriptiorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	CATCH(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));

	this->m_renderPass.init(device, format);

	VkGraphicsPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = nullptr;
	pipelineCreateInfo.flags = 0;
	pipelineCreateInfo.stageCount = this->m_shaderStages.size();
	pipelineCreateInfo.pStages = this->m_shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &assemblyInputCreateInfo;
	pipelineCreateInfo.pTessellationState = nullptr;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multiSampleStateCreateInfo;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
	pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	pipelineCreateInfo.layout = m_pipelineLayout;
	pipelineCreateInfo.renderPass = this->m_renderPass.getRenderPass();
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = 0;

	CATCH(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->m_pipeline));
}

void core::graphic::vulkan::PipelineVK::c_shaderStageInfo(const int &index) {
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.pNext = nullptr;
	shaderStageCreateInfo.flags = 0;
	shaderStageCreateInfo.stage = this->m_shader.getShaderModule()[index].shaderType;
	shaderStageCreateInfo.module = this->m_shader.getShaderModule()[index].shaderModule;
	shaderStageCreateInfo.pName = "main";
	shaderStageCreateInfo.pSpecializationInfo = nullptr;

	this->m_shaderStages.resize(this->m_shaderStages.size() + 1);
	this->m_shaderStages[this->m_shaderStages.size() - 1] = shaderStageCreateInfo;
}


void core::graphic::vulkan::PipelineVK::r_shaders(const VkDevice &device) {
	//I don't think I'm gonna change the names of the shaders...
	this->m_shader.init(device, "vert.spv", "frag.spv");
}


core::graphic::Shader core::graphic::vulkan::PipelineVK::getShader() const {
	return this->m_shader;
}

core::graphic::vulkan::RenderPassVK core::graphic::vulkan::PipelineVK::gerRenderPassVK() const {
	return this->m_renderPass;
}

std::vector<VkPipelineShaderStageCreateInfo> core::graphic::vulkan::PipelineVK::getShaderStages() const {
	return this->m_shaderStages;
}

VkPipelineLayout core::graphic::vulkan::PipelineVK::getPipelineLayout() const {
	return this->m_pipelineLayout;
}

VkPipeline core::graphic::vulkan::PipelineVK::getPipeline() const {
	return this->m_pipeline;
}

VkDescriptorSetLayout core::graphic::vulkan::PipelineVK::getVkDescriptorSetLayout() const {
	return this->m_descriptiorSetLayout;
}
