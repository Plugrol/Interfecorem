#pragma once
#include "VulkanCreator.h"
#include "DeviceVK.h"

namespace core::graphic::vulkan {
	struct VulkanSwapchainFormat
	{
		VkColorSpaceKHR colorSpace;
		VkFormat colorFormat;
	};

	class SwapchainVK
	{

		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
		VulkanSwapchainFormat m_format{};

		/**
		* Choose the best format, that the swapchain should use.
		*
		* @param gpu the physical device, to check which format is the best.
		* @param surface the surface to get additional info about the device.
		* @return VulkanSwapchainFormat The best format.
		*/
		VulkanSwapchainFormat s_swapchainFormat(PhysicalDeviceVK gpu, const VkSurfaceKHR &surface);

		/**
		* Choose the best present mode, that the swapchain should use.
		*
		* @param gpu the physical device, to check which format is the best.
		* @param surface the surface to get additional info about the device.
		* @return VkPresentModeKHR The best present mode.
		*/
		static VkPresentModeKHR s_presentMode(PhysicalDeviceVK gpu, const VkSurfaceKHR &surface);

		/**
		* Get the image count, that is supported. 3 is most wanted here.
		*
		* @param gpu the physical device, to check which format is the best.
		* @param surface the surface to get additional info about the device.
		* @return uint32_t It'll just 3 or 2 I guess.
		*/
		static uint32_t s_imageCount(PhysicalDeviceVK gpu, const VkSurfaceKHR &surface);
	public:

		//Mostly unused.
		SwapchainVK();

		//Mostly unused.
		explicit SwapchainVK(VkSwapchainKHR oldSwapchain);

		/**
		* Create the swapchain and initialise everything.
		*
		* @param gameInfo The info about the game, like title and size.
		* @param device The device that has been created.
		* @param surface The created surface, this program is working on.
		*/
		void init(const GameInfo& gameInfo, const DeviceVK& device,  const VkSurfaceKHR& surface);

		/**
		* Destroys the swapchain.
		*
		* @param device The device info that is need to destroy the swapchain.
		*/
		void destroy(const VkDevice& device);

		//Getter of <var*>m_swapchain</var*>
		[[nodiscard]] VkSwapchainKHR getSwapchain() const;

		//Getter of <var*>m_format</var*>
		[[nodiscard]] VulkanSwapchainFormat getFormat() const;
	};
}
