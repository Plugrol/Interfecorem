#include "BufferVK.h"

#include "CommandBufferVK.h"

core::graphic::vulkan::BufferVK::BufferVK() = default;

void core::graphic::vulkan::BufferVK::init(const DeviceVK& device, const Shader& shader, const VkCommandPool& commandPool, const VkDescriptorSetLayout* descriptorLayout)
{
	Buffer vertexBuffer{};
	c_buffers(vertexBuffer, shader.getVertexVertices(), device, shader, commandPool, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	Buffer indexBuffer{};
	c_buffers(indexBuffer, shader.getIndices(), device, shader, commandPool, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

	Buffer uniformBuffer{};
	constexpr VkDeviceSize bufferSize = sizeof(this->m_MVP);
	c_buffer(uniformBuffer.buffer, uniformBuffer.bufferMemory, device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	a_buffer(uniformBuffer);

	this->m_descriptorPoolVK.init(device.getDevice(), descriptorLayout, getUniformBuffer().buffer, this->m_MVP);

}

void core::graphic::vulkan::BufferVK::destroy(const VkDevice& device, const VkDescriptorSetLayout& descriptorLayout)
{
	this->m_descriptorPoolVK.destroy(device, descriptorLayout);
	for (auto &[buffer, bufferMemory] : this->m_buffer)
	{
		if (buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, buffer, nullptr);
			vkFreeMemory(device, bufferMemory, nullptr);
		}

	}
	this->m_buffer.clear();
}

template <typename T>
void core::graphic::vulkan::BufferVK::c_buffers(Buffer& buffer, const std::vector<T>& data, const DeviceVK& device, const Shader& shader, const VkCommandPool& commandPool, const VkBufferUsageFlags& usage)
{

	//Staging buffer
	Buffer stagingBuffer{};

	const VkDeviceSize bufferSize = sizeof(T) * data.size();
	c_buffer(stagingBuffer.buffer, stagingBuffer.bufferMemory, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* rawData;
	vkMapMemory(device.getDevice(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &rawData);
	memcpy(rawData, data.data(), bufferSize);
	vkUnmapMemory(device.getDevice(), stagingBuffer.bufferMemory);

	c_buffer(buffer.buffer, buffer.bufferMemory, device, bufferSize, usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	w_buffer(device, commandPool, stagingBuffer.buffer, buffer.buffer, bufferSize);

	this->a_buffer(buffer);

	vkDestroyBuffer(device.getDevice(), stagingBuffer.buffer, nullptr);
	vkFreeMemory(device.getDevice(), stagingBuffer.bufferMemory, nullptr);
}

void core::graphic::vulkan::BufferVK::c_buffer(VkBuffer& buffer, VkDeviceMemory& deviceMemory, const DeviceVK& device, const VkDeviceSize& deviceSize, const VkBufferUsageFlags& bufferUsageFlags, const VkMemoryPropertyFlags& memoryFlags)
{
	VkBufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = nullptr;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = deviceSize;
	bufferCreateInfo.usage = bufferUsageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;

	CATCH(vkCreateBuffer(device.getDevice(), &bufferCreateInfo, nullptr, &buffer));

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device.getDevice(), buffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = r_memoryTypeIndex(device, memoryRequirements.memoryTypeBits, memoryFlags);

	CATCH(vkAllocateMemory(device.getDevice(), &allocateInfo, nullptr, &deviceMemory));

	vkBindBufferMemory(device.getDevice(), buffer, deviceMemory, 0);
}

void core::graphic::vulkan::BufferVK::w_buffer(const DeviceVK& device, const VkCommandPool& commandPool, const VkBuffer& src, const VkBuffer& dst, const VkDeviceSize& size)
{
	CommandBufferVK commandBuffer;
	commandBuffer.init(device, 1, commandPool);
	commandBuffer.b_bufferBeginInfo(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VkBufferCopy bufferCopy;
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = size;
	vkCmdCopyBuffer(*commandBuffer.getCommandBuffer(), src, dst, 1, &bufferCopy);

	commandBuffer.i_bufferBeginInfo(0);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffer.getCommandBuffer();
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	CATCH(vkQueueSubmit(device.getQueue(), 1, &submitInfo, VK_NULL_HANDLE));

	vkQueueWaitIdle(device.getQueue());
	vkFreeCommandBuffers(device.getDevice(), commandBuffer.getCommandPool(), 1, commandBuffer.getCommandBuffer());
	delete[] commandBuffer.getCommandBuffer();
}

uint32_t core::graphic::vulkan::BufferVK::r_memoryTypeIndex(const DeviceVK& device, const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties)
{
	const VkPhysicalDeviceMemoryProperties memoryProperties = device.getPhysicalDevice().r_physicalDeviceMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties))
		{
			return i;
		}
	}

	throw std::runtime_error("No memory type found.");
}

void core::graphic::vulkan::BufferVK::a_buffer(const Buffer& buffer)
{
	this->m_buffer.resize(this->m_buffer.size() + 1);
	this->m_buffer[this->m_buffer.size() - 1] = buffer;
}

core::graphic::vulkan::Buffer core::graphic::vulkan::BufferVK::getVertexBuffer() const
{
	return this->m_buffer[0];
}

core::graphic::vulkan::Buffer core::graphic::vulkan::BufferVK::getIndexBuffer() const
{
	return this->m_buffer[1];
}

core::graphic::vulkan::Buffer core::graphic::vulkan::BufferVK::getUniformBuffer() const
{
	return this->m_buffer[2];
}

core::graphic::vulkan::descriptor::DescriptorPoolVK core::graphic::vulkan::BufferVK::getDescriptorPool() const
{
	return this->m_descriptorPoolVK;
}

glm::mat4 core::graphic::vulkan::BufferVK::getMVP() const
{
	return this->m_MVP;
}

void core::graphic::vulkan::BufferVK::setMVP(const glm::mat4 &mvp)
{
	m_MVP = mvp;
}