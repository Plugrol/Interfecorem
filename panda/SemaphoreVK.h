#pragma once
#include "VulkanCreator.h"

namespace core
{
	namespace graphic
	{
		namespace vulkan
		{
			class SemaphoreVK
			{
			private:
				VkSemaphore* m_semaphore = VK_NULL_HANDLE;

				/**
				* Creates the semaphores.
				* 
				* @param VkDevice& The device, where they are running on.
				*/
				void c_semaphores(const VkDevice& device) const;

			public:

				//Mostly unused-
				SemaphoreVK();

				/**
				* Creates the semaphores.
				* 
				* @param VkDevice& The device, where they are running on.
				*/
				void init(const VkDevice& device);

				/**
				* Clear the semaphores.
				* 
				* @param VkDevice& The device, where they are running on.
				*/
				void destroy(const VkDevice& device);

				//Getter of <var*>m_semaphore</var*>
				VkSemaphore* getSemaphores() const;
			};
		}
	}
}
