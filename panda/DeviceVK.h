#pragma once
#include "VulkanCreator.h"
#include "PhysicalDeviceVK.h"

namespace core::graphic::vulkan {
	class DeviceVK {
		VkDevice m_device = VK_NULL_HANDLE;
		VkQueue m_queue = VK_NULL_HANDLE;
		PhysicalDeviceVK m_physicalDevice;
		uint32_t m_queueIndex = 0;

		/**
		* @return VkDeviceCreateInfo create the device create info.
		*/
		void c_device();

		/**
		* @return VkPhysicalDevice Get the best physical device.
		*/
		static VkPhysicalDevice s_physicalDevice(const VkInstance &instance);

		/**
		* @return uint32_t Get the index of the best queue family.
		*/
		static uint32_t s_queueFamily(const std::vector<VkQueueFamilyProperties> &queueFamilies);

	public:
		//Mostly unused.
		DeviceVK();

		/**
		* Choose the best physical device and create a virtual device out of it.
		*
		* @param instance The initialized instance of vulkan.
		* @param surface The surface, this program is working on.
		*/
		void init(const VkInstance &instance, const VkSurfaceKHR &surface);

		/**
		* Destroy the device, that was created out of the physical device.
		*/
		void destroy();

		//Getter of <var*>m_device</var*>
		VkDevice getDevice() const;

		//Getter of <var*>m_queue</var*>
		VkQueue getQueue() const;

		//Getter of <var*>m_physicalDevice</var*>
		PhysicalDeviceVK getPhysicalDevice() const;

		//Getter of <var*>m_queueIndex</var*>
		uint32_t getQueueIndex() const;
	};
}

