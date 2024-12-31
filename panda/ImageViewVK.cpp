#include "ImageViewVK.h"

core::graphic::vulkan::ImageViewVK::ImageViewVK() = default;

void core::graphic::vulkan::ImageViewVK::init(const VkDevice& device, const SwapchainVK& swapchain)
{
	r_images(device, swapchain);
}

void core::graphic::vulkan::ImageViewVK::destroy(const VkDevice& device)
{
	if (this->m_imageView != VK_NULL_HANDLE)
	{
		for (int i = 0; i < this->m_amountOfImages; i++)
		{
			vkDestroyImageView(device, this->m_imageView[i], nullptr);
		}
		delete[] this->m_imageView;
		this->m_imageView = VK_NULL_HANDLE;
	}
}

void core::graphic::vulkan::ImageViewVK::c_swapchain(const VkDevice& device, const VkImage* swapchainImages, const SwapchainVK& swapchain)
{

	this->m_imageView = new VkImageView[this->m_amountOfImages];
	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;

	//In loop

	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = swapchain.getFormat().colorFormat;

	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (int i = 0; i < this->m_amountOfImages; i++)
	{

		imageViewCreateInfo.image = swapchainImages[i];
		CATCH(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_imageView[i]));
	}

	delete swapchainImages;
}

void core::graphic::vulkan::ImageViewVK::r_images(const VkDevice& device, const SwapchainVK& swapchain)
{
	vkGetSwapchainImagesKHR(device, swapchain.getSwapchain(), &this->m_amountOfImages, nullptr);
	auto* swapchainImages = new VkImage[this->m_amountOfImages];
	CATCH(vkGetSwapchainImagesKHR(device, swapchain.getSwapchain(), &this->m_amountOfImages, swapchainImages));

	c_swapchain(device, swapchainImages, swapchain);
}

VkImageView* core::graphic::vulkan::ImageViewVK::getImageView() const
{
	return this->m_imageView;
}

uint32_t core::graphic::vulkan::ImageViewVK::getAmountOfImages() const
{
	return this->m_amountOfImages;
}
