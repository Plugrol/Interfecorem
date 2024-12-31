#pragma once
#include "VulkanCreator.h"
#include "SwapchainVK.h"

namespace core::graphic::vulkan {
	class ImageViewVK
	{

		VkImageView* m_imageView = VK_NULL_HANDLE;
		uint32_t m_amountOfImages = 0;

		/**
		* Creating imageViews and save to them in <var*>imageView</var*>
		*
		* @param device Adding the device in order to communicate with vulkan.
		* @param swapchainImages Also adding swapchain to get stored information.
		* @param swapchain
		*/
		void c_swapchain(const VkDevice& device, const VkImage* swapchainImages, const SwapchainVK& swapchain);

		/**
		* Receive the size of images and store them into <var>amountOfImages</var>
		*
		* @param device Adding the device in order to communicate with vulkan.
		* @param swapchain Also adding swapchain to get stored information.
		*/
		void r_images(const VkDevice &device, const SwapchainVK &swapchain);

	public:

		//Mostly unused
		ImageViewVK();

		/**
		* Construct the imageView and fill up every member.
		*
		* @param device Adding the device in order to communicate with vulkan-
		* @param swapchain Also adding swapchain to get stored information.
		*/
		void init(const VkDevice& device, const SwapchainVK& swapchain);

		/**
		* Clear all imageViews, that were created.
		*/
		void destroy(const VkDevice& device);

		[[nodiscard]] VkImageView* getImageView() const;

		[[nodiscard]] uint32_t getAmountOfImages() const;
	};
}
