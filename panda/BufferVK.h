#pragma once
#include "VulkanCreator.h"
#include "DeviceVK.h"
#include "Shader.h"
#include "DescriptorPoolVK.h"
#include <cstring>

//TODO c++ doc

namespace core::graphic::vulkan {
	struct Buffer {
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
	};

	class BufferVK {
		glm::mat4 m_MVP{};
		std::vector<Buffer> m_buffer;
		descriptor::DescriptorPoolVK m_descriptorPoolVK;

		template<typename T>
		void c_buffers(Buffer &buffer, const std::vector<T> &data, const DeviceVK &device, const Shader &shader,
		               const VkCommandPool &commandPool, const VkBufferUsageFlags &usage);

		static void c_buffer(VkBuffer &buffer, VkDeviceMemory &deviceMemory, const DeviceVK &device,
		                     const VkDeviceSize &deviceSize, const VkBufferUsageFlags &bufferUsageFlags,
		                     const VkMemoryPropertyFlags &memoryFlags);

		static void w_buffer(const DeviceVK &device, const VkCommandPool &commandPool, const VkBuffer &src,
		                     const VkBuffer &dst, const VkDeviceSize &size);

		void a_buffer(const Buffer &buffer);

		static uint32_t r_memoryTypeIndex(const DeviceVK &device, const uint32_t &typeFilter,
		                                  const VkMemoryPropertyFlags &properties);

	public:
		BufferVK();

		void init(const DeviceVK &device, const Shader &shader, const VkCommandPool &commandPool,
		          const VkDescriptorSetLayout *descriptorLayout);

		void destroy(const VkDevice &device, const VkDescriptorSetLayout &descriptorLayout);

		[[nodiscard]] Buffer getVertexBuffer() const;

		[[nodiscard]] Buffer getIndexBuffer() const;

		[[nodiscard]] Buffer getUniformBuffer() const;

		[[nodiscard]] descriptor::DescriptorPoolVK getDescriptorPool() const;

		[[nodiscard]] glm::mat4 getMVP() const;

		void setMVP(const glm::mat4 &mvp);
	};
}
