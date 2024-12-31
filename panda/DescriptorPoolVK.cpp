#include "DescriptorPoolVK.h"

#include <BufferVK.h>

core::graphic::vulkan::descriptor::DescriptorPoolVK::DescriptorPoolVK(VkDescriptorPool_T const* descriptorPool, VkDescriptorSet_T const* descriptorSet)
{
	this->m_descriptorPool = descriptorPool;
	this->m_descriptorSet = descriptorSet;
}

void core::graphic::vulkan::descriptor::DescriptorPoolVK::init(const VkDevice& device, const VkDescriptorSetLayout* descriptorLayout, const VkBuffer& buffer, const glm::mat4 &mvp)
{
	c_descriptor(device, descriptorLayout, buffer, mvp);
}

void core::graphic::vulkan::descriptor::DescriptorPoolVK::c_descriptor(
	const VkDevice& device,
	const VkDescriptorSetLayout* descriptorLayout,
	const VkBuffer& buffer,
	[[maybe_unused]] const glm::mat4 &mvp)
{
	VkDescriptorPoolSize descriptorPoolSize;
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo descriptorPollCreateInfo;
	descriptorPollCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPollCreateInfo.pNext = nullptr;
	descriptorPollCreateInfo.flags = 0;
	descriptorPollCreateInfo.maxSets = 1;
	descriptorPollCreateInfo.poolSizeCount = 1;
	descriptorPollCreateInfo.pPoolSizes = &descriptorPoolSize;

	VkDescriptorPool nonConstDescriptorPool = VK_NULL_HANDLE; // Temporary non-const variable for creation
	CATCH(vkCreateDescriptorPool(device, &descriptorPollCreateInfo, nullptr, &nonConstDescriptorPool));

	// Assign the created descriptor pool to the const member
	this->m_descriptorPool = nonConstDescriptorPool;

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = nullptr;
	descriptorSetAllocateInfo.descriptorPool = const_cast<VkDescriptorPool>(this->m_descriptorPool);
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = descriptorLayout;

	VkDescriptorSet nonConstDescriptorSet = VK_NULL_HANDLE; // Temporary non-const variable for allocation
	CATCH(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &nonConstDescriptorSet));

	// Assign the allocated descriptor set to the const member
	this->m_descriptorSet = nonConstDescriptorSet;

	VkDescriptorBufferInfo descriptorBufferInfo;
	descriptorBufferInfo.buffer = buffer;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = sizeof(mvp);

	VkWriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = nullptr;
	writeDescriptorSet.dstSet = const_cast<VkDescriptorSet>(this->m_descriptorSet);
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pImageInfo = nullptr;
	writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
	writeDescriptorSet.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}

void core::graphic::vulkan::descriptor::DescriptorPoolVK::destroy(const VkDevice& device, const VkDescriptorSetLayout& descriptorLayout) const {
	vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);

	// Cast away const to destroy the pool, as destruction is a mutable operation
	vkDestroyDescriptorPool(device, const_cast<VkDescriptorPool>(m_descriptorPool), nullptr);
}

const VkDescriptorPool_T *core::graphic::vulkan::descriptor::DescriptorPoolVK::getDescriptorPool() const {
	return this->m_descriptorPool;
}

const VkDescriptorSet_T * core::graphic::vulkan::descriptor::DescriptorPoolVK::getDescriptorSet() const {
	return this->m_descriptorSet;
}
