#pragma once
#include "VulkanCreator.h"
#include "SwapchainVK.h"

namespace core::graphic::vulkan {
	class RenderPassVK {
		VkRenderPass m_renderPass = VK_NULL_HANDLE;

		/**
		* Creates the render pass.
		*
		* @param device The device info. that is needed to register this render pass.
		* @param format the format of the swapchain.
		*/
		void c_renderPass(const VkDevice &device, const VulkanSwapchainFormat &format);

	public:
		//Mostly unused
		RenderPassVK();

		/**
		* Set up the render pass and make it read to begin.
		*
		* @param device The device info. that is needed to register this render pass.
		* @param format the format of the swapchain.
		*/
		void init(const VkDevice &device, const VulkanSwapchainFormat &format);

		/**
		* Destroys the data of this render pass.
		*
		* @param device the device this render pass is running on.
		*/
		void destroy(const VkDevice &device);

		/**
		* This method starts the render pass and allows to send commands.
		*
		* @param gameInfo The information of the screensize, to draw on.
		* @param frameBuffer The frame buffer, which accepts the frames-
		* @param commandBuffer The command buffer to send commands to.
		*/
		void b_renderPass(const GameInfo &gameInfo, const VkFramebuffer &frameBuffer,
		                  const VkCommandBuffer &commandBuffer) const;

		/**
		* Disables the render pass and clear its data.
		*
		* @param commandBuffer The command buffer this render pass is running in.
		*/
		static void i_renderPass(const VkCommandBuffer &commandBuffer);

		//Getter of <var>m_renderPass</var>
		[[nodiscard]] VkRenderPass getRenderPass() const;
	};
}
