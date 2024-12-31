#include "CommandBufferVK.h"

core::graphic::vulkan::CommandBufferVK::CommandBufferVK(const VkCommandPool commandPool, VkCommandBuffer*commandBuffer) {
	this->m_commandPool = commandPool;
	this->m_commandBuffer = commandBuffer;
}

void core::graphic::vulkan::CommandBufferVK::init(const DeviceVK& device, const uint32_t &amountOfImages)
{
	c_commandPool(device);
	init(device, amountOfImages, this->m_commandPool);
}

void core::graphic::vulkan::CommandBufferVK::init(const DeviceVK& device, const uint32_t& amountOfImages, const VkCommandPool& commandPool)
{
	if (this->m_commandPool == VK_NULL_HANDLE)
	{
		this->m_commandPool = commandPool;
	}
	c_commandBuffer(device.getDevice(), amountOfImages);

}

void core::graphic::vulkan::CommandBufferVK::destroy(const VkDevice& device, const uint32_t &amountOfImages)
{
	if (this->m_commandPool != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(device, this->m_commandPool, amountOfImages, this->m_commandBuffer);
		delete[] this->m_commandBuffer;

		vkDestroyCommandPool(device, this->m_commandPool, nullptr);

		this->m_commandBuffer = VK_NULL_HANDLE;
		this->m_commandPool = VK_NULL_HANDLE;
	}
}

void core::graphic::vulkan::CommandBufferVK::b_bufferBeginInfo(const uint32_t& indexOfImage, const VkCommandBufferUsageFlags& flags) const {
	VkCommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = nullptr;
	commandBufferBeginInfo.flags = flags;
	commandBufferBeginInfo.pInheritanceInfo = nullptr;

	CATCH(vkBeginCommandBuffer(this->m_commandBuffer[indexOfImage], &commandBufferBeginInfo));

}

void core::graphic::vulkan::CommandBufferVK::i_bufferBeginInfo(const uint32_t& indexOfImage) const {
	CATCH(vkEndCommandBuffer(this->m_commandBuffer[indexOfImage]));
}

void core::graphic::vulkan::CommandBufferVK::sendCommands(const GameInfo& gameInfo, const PipelineVK& pipeline, const VkFramebuffer* frameBuffer, const uint32_t& amountOfImages, const BufferVK& buffer)
{

	for (size_t i = 0; i < amountOfImages; i++)
	{
		b_bufferBeginInfo(i, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		pipeline.gerRenderPassVK().b_renderPass(gameInfo, frameBuffer[i], this->m_commandBuffer[i]);

		vkCmdBindPipeline(this->m_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

		VkViewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = gameInfo.getWindowScale.width;
		viewport.height = gameInfo.getWindowScale.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(this->m_commandBuffer[i], 0, 1, &viewport);

		VkRect2D scissor;
		scissor.offset = { 0, 0 };
		scissor.extent = { gameInfo.getWindowScale.width, gameInfo.getWindowScale.height };
		vkCmdSetScissor(this->m_commandBuffer[i], 0, 1, &scissor);

		constexpr VkDeviceSize offsets[] = { 0 };
		VkBuffer vertexBuffer = buffer.getVertexBuffer().buffer;
		vkCmdBindVertexBuffers(this->m_commandBuffer[i], 0, 1, &vertexBuffer, offsets);
		vkCmdBindIndexBuffer(this->m_commandBuffer[i], buffer.getIndexBuffer().buffer, 0, VK_INDEX_TYPE_UINT32);

		VkDescriptorSet descriptorSet = const_cast<VkDescriptorSet>(buffer.getDescriptorPool().getDescriptorSet());
		vkCmdBindDescriptorSets(this->m_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

		//vkCmdDraw(this->m_commandBuffer[i], pipeline.getShader().getVertexVertices().size(), 1, 0, 0);

		vkCmdDrawIndexed(this->m_commandBuffer[i], pipeline.getShader().getIndices().size(), 1, 0, 0, 0);

		pipeline.gerRenderPassVK().i_renderPass(this->m_commandBuffer[i]);
		i_bufferBeginInfo(i);
	}

}

void core::graphic::vulkan::CommandBufferVK::c_commandPool(const core::graphic::vulkan::DeviceVK& device)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = nullptr;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = device.getQueueIndex(); //TODO check right queue family

	CATCH(vkCreateCommandPool(device.getDevice(), &commandPoolCreateInfo, nullptr, &this->m_commandPool));
}

void core::graphic::vulkan::CommandBufferVK::c_commandBuffer(const VkDevice& device, const uint32_t &amountOfImages)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = this->m_commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = amountOfImages;

	this->m_commandBuffer = new VkCommandBuffer[amountOfImages];
	CATCH(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, this->m_commandBuffer));
}


VkCommandPool core::graphic::vulkan::CommandBufferVK::getCommandPool() const
{
	return this->m_commandPool;
}

VkCommandBuffer* core::graphic::vulkan::CommandBufferVK::getCommandBuffer() const
{
	return this->m_commandBuffer;
}

