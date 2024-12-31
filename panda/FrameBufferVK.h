#pragma once
#include "VulkanCreator.h"

namespace core::graphic::vulkan {

	class FrameBufferVK
	{
		VkFramebuffer* m_frameBuffer = VK_NULL_HANDLE;

		/**
		* Creates the instances of the frameBuffer.
		*
		* @param gameInfo The information about the game.
		* @param device The device this framebuffer will run on.
		* @param renderPass
		* @param imageView
		* @param amountOfImages
		*/
		void c_frameBuffer(const GameInfo& gameInfo, const VkDevice& device, const VkRenderPass& renderPass, const VkImageView* imageView, const size_t& amountOfImages);

	public:
		//Mostly unused
		FrameBufferVK();

		/**
		* Creates the instances of the framebuffer.
		*
		* @param gameInfo The informations about the game.
		* @param device The device this frambuffer will run on.
		* @param renderPass
		* @param imageView
		* @param amountOfImages
		*/
		void init(const GameInfo& gameInfo, const VkDevice& device, const VkRenderPass& renderPass, const VkImageView* imageView, const size_t& amountOfImages);

		/**
		* Destroys the frambuffer and clear its data
		*
		* @param device The device, that is used to render
		* @param amountOfImages The count of the images in the swapchain.
		*/
		void destroy(const VkDevice& device, const size_t& amountOfImages);

		//Getter of <var*>m_frameBuffer</var*>
		[[nodiscard]] VkFramebuffer* getFrameBuffer() const; //Size is always identical to imageview size

	};

}
