#pragma once
#include "VulkanCreator.h"
#include "DeviceVK.h"
#include "PipelineVK.h"
#include "BufferVK.h"


namespace core::graphic::vulkan {
	class CommandBufferVK {
		VkCommandPool m_commandPool = VK_NULL_HANDLE;
		VkCommandBuffer *m_commandBuffer = VK_NULL_HANDLE;

		/**
		* Creates a command pool
		*
		* @param device the device, to create the pool.
		*/
		void c_commandPool(const DeviceVK &device);

		/**
		* Creates a commandBuffer
		*
		* @param device the device, to create the buffer.
		* @param amountOfImages the amount of the Images.
		*/
		void c_commandBuffer(const VkDevice &device, const uint32_t &amountOfImages);

	public:

		explicit CommandBufferVK(VkCommandPool commandPool = VK_NULL_HANDLE, VkCommandBuffer* commandBuffer = VK_NULL_HANDLE);

		/**
		* Creates the CommandBuffer, so you can send commands, that will be executed.
		*
		* @param device The device, that is needed to register the buffer.
		* @param amountOfImages
		* @oaram amountOfImages The amount of images, that are in the swapchain.
		*/
		void init(const DeviceVK &device, const uint32_t &amountOfImages);

		/**
		* Creates the CommandBuffer, so you can send commands, that will be executed.
		*
		* @param device The device, that is needed to register the buffer.
		* @param amountOfImages TODO
		* @param commandPool
		* @oaram amountOfImages The amount of images, that are in the swapchain.
		*/
		void init(const DeviceVK &device, const uint32_t &amountOfImages,
		          const VkCommandPool &commandPool);

		/**
		* Destroys the buffer and the pool, to save some resources.
		*
		* @param device The device it is running on.
		* @oaram amountOfImages The amount of images, that are in the swapchain.
		* @param amountOfImages
		*/
		void destroy(const VkDevice &device, const uint32_t &amountOfImages);

		/**
		* After creating the pool and the buffer,
		* this method starts the buffer, so you could send commands.
		*
		* @oaram uint32_t& The amount of images, that are in the swapchain.
		*/
		void b_bufferBeginInfo(const uint32_t &indexOfImage, const VkCommandBufferUsageFlags &flags) const;

		/**
		* After starting the buffer, it should stop listing, to clear some resource.
		* This method disable it.
		*
		* @oaram uint32_t& The amount of images, that are in the swapchain.
		*/
		void i_bufferBeginInfo(const uint32_t &indexOfImage) const;

		/**
		* Send needed commands to vulkan to draw.
		*
		* @param gameInfo The information about the size of the window.
		* @param pipeline The pipeline.
		* @param frameBuffer The frames.
		* @param amountOfImages The amount of images.
		* @param buffer the buffer.
		* //TODO new param c++ doc
		*/
		void sendCommands(const GameInfo &gameInfo, const PipelineVK &pipeline, const VkFramebuffer *frameBuffer,
		                  const uint32_t &amountOfImages, const BufferVK &buffer);

		//Getter of <var>m_commandPool</var>
		[[nodiscard]] VkCommandPool getCommandPool() const;

		//Getter of <var*>m_commandBuffer</var*>
		[[nodiscard]] VkCommandBuffer *getCommandBuffer() const;
	};
}

