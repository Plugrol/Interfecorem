#include "FrameBufferVK.h"

core::graphic::vulkan::FrameBufferVK::FrameBufferVK() = default;

void core::graphic::vulkan::FrameBufferVK::destroy(const VkDevice &device, const size_t& amountOfImages)
{
	if (this->m_frameBuffer != VK_NULL_HANDLE)
	{
		for (size_t i = 0; i < amountOfImages; i++)
		{
			vkDestroyFramebuffer(device, this->m_frameBuffer[i], nullptr);
		}
		delete[] this->m_frameBuffer;
		this->m_frameBuffer = VK_NULL_HANDLE;
	}
}

void core::graphic::vulkan::FrameBufferVK::init(const GameInfo& gameInfo, const VkDevice &device, const VkRenderPass& renderPass, const VkImageView* imageView, const size_t& amountOfImages)
{
	c_frameBuffer(gameInfo, device, renderPass, imageView, amountOfImages);
}

void core::graphic::vulkan::FrameBufferVK::c_frameBuffer(const GameInfo& gameInfo, const VkDevice &device, const VkRenderPass& renderPass, const VkImageView* imageView, const size_t& amountOfImages)
{
	this->m_frameBuffer = new VkFramebuffer[amountOfImages];
	for (size_t i = 0; i < amountOfImages; i++)
	{
		VkFramebufferCreateInfo frameBufferCreateInfo;
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = nullptr;
		frameBufferCreateInfo.flags = 0;
		frameBufferCreateInfo.renderPass = renderPass;
		frameBufferCreateInfo.attachmentCount = 1;
		frameBufferCreateInfo.pAttachments = &(imageView[i]);
		frameBufferCreateInfo.width = gameInfo.getWindowScale.width;
		frameBufferCreateInfo.height = gameInfo.getWindowScale.height;
		frameBufferCreateInfo.layers = 1;

		CATCH(vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &(this->m_frameBuffer[i])))
	}
}

VkFramebuffer* core::graphic::vulkan::FrameBufferVK::getFrameBuffer() const
{
	return this->m_frameBuffer;
}