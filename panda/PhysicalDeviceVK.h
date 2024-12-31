#pragma once
#include "VulkanCreator.h"

namespace core::graphic::vulkan {
	class PhysicalDeviceVK {

		VkPhysicalDevice_T const *m_device; //The device, this class is about


	public:

		/**
		* Set the Physical device and prepare to be written.
		*
		* @param device The device this class is about.
		*/
		explicit PhysicalDeviceVK(VkPhysicalDevice_T const *device);

		//Mostly unused.
		PhysicalDeviceVK();

		[[nodiscard]] VkPhysicalDevice getDevice() const;

		/**
		* Get the properties of a
		* graphical device
		*
		* @return VkPhysicalDeviceProperties The properties. DELETE AFTER USING
		*/
		[[nodiscard]] VkPhysicalDeviceProperties r_physicalDeviceProperties() const;

		/**
		* Get the features of a
		* graphical device
		*
		* @return VkPhysicalDeviceFeatures The features. DELETE AFTER USING
		*/
		[[nodiscard]] VkPhysicalDeviceFeatures r_physicalDeviceFeatures() const;


		/**
		* Get the memory properties of a
		* graphical device
		*
		* @return VkPhysicalDeviceMemoryProperties The memory properties. DELETE AFTER USING
		*/
		[[nodiscard]] VkPhysicalDeviceMemoryProperties r_physicalDeviceMemoryProperties() const;

		/**
		* Get the family properties.
		*
		* @return Contains all available queue families.
		*/
		[[nodiscard]] std::vector<VkQueueFamilyProperties> r_queueFamilyProperties() const;

		/**
		* Get the surface capabilities
		* of a graphical device
		*
		* @param surface The surface which was created in a further step
		* @return VkSurfaceCapabilitiesKHR The surface capabilities. DELETE AFTER USING
		*/
		[[nodiscard]] VkSurfaceCapabilitiesKHR r_surfaceCapabilitiesKHR(const VkSurfaceKHR &surface) const;

		/**
		* This method returns the supported formats
		* of the graphical device.
		*
		* @param surface The surface which was created in a further step
		* @return std::vector All supported formats.
		*/
		[[nodiscard]] std::vector<VkSurfaceFormatKHR> r_supportedFormats(const VkSurfaceKHR &surface) const;

		/**
		* This method returns all available Present modes.
		*
		* @param surface The surface which was created in a further step
		* @return std::vector The supported present modes.
		*/
		[[nodiscard]] std::vector<VkPresentModeKHR> r_supportedPresentModes(const VkSurfaceKHR &surface) const;

		/**
		* @return VkBool32 whether the physical device supports swap chains.
		*/
		[[nodiscard]] VkBool32 r_physicalDeviceSupportSwapchain(const VkSurfaceKHR &surface) const;

		/**
		* This method does print the debug stats of the graphic card.
		*/
		void debug(const VkSurfaceKHR &surface) const;
	};
}
