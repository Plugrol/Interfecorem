#pragma once
#include "VulkanCreator.h"

namespace core::graphic::vulkan::descriptor {
	class DescriptorPoolVK
	{
		const VkDescriptorPool_T* m_descriptorPool{};
		const VkDescriptorSet_T* m_descriptorSet{};

		void c_descriptor(
			VkDevice_T *const&device,
			VkDescriptorSetLayout_T *const*descriptorLayout,
			VkBuffer_T *const
			&buffer,
			const glm::mat4 &mvp
		);

	public:

		explicit DescriptorPoolVK(VkDescriptorPool_T const* descriptorPool = VK_NULL_HANDLE, VkDescriptorSet_T const* descriptorSet = VK_NULL_HANDLE);

		void init(VkDevice_T *const&device, VkDescriptorSetLayout_T *const*descriptorLayout, VkBuffer_T *const
		          &buffer, const glm::mat4 &mvp);
					
		void destroy(const VkDevice& device, const VkDescriptorSetLayout& descriptorLayout) const;

		[[nodiscard]] const VkDescriptorPool_T* getDescriptorPool() const;
		[[nodiscard]] const VkDescriptorSet_T* getDescriptorSet() const;
	};
}
