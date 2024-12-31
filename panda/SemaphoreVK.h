#pragma once
#include "VulkanCreator.h"

namespace core::graphic::vulkan {
	class SemaphoreVK
	{
		VkSemaphore* m_semaphore = VK_NULL_HANDLE;

		/**
		* Creates the semaphores.
		*
		* @param device The device, where they are running on.
		*/
		void c_semaphores(const VkDevice& device) const;

	public:

		//Mostly unused-
		SemaphoreVK();

		/**
		* Creates the semaphores.
		*
		* @param device The device, where they are running on.
		*/
		void init(const VkDevice& device);

		/**
		* Clear the semaphores.
		*
		* @param device The device, where they are running on.
		*/
		void destroy(const VkDevice& device);

		[[nodiscard]] VkSemaphore* getSemaphore(uint32_t index) const;
	};
}
