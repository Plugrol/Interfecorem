#pragma once

#include "VulkanCreator.h"

#include "WindowVK.h"
#include "DeviceVK.h"
#include "SwapchainVK.h"
#include "ImageViewVK.h"
#include "PipelineVK.h"
#include "FrameBufferVK.h"
#include "CommandBufferVK.h"
#include "SemaphoreVK.h"
#include "BufferVK.h"


namespace core::graphic::vulkan {
	class VulkanVK {
		GameInfo *m_gameInfo = nullptr;

	public:
		WindowVK m_windowVK;
		DeviceVK m_deviceVK;
		SwapchainVK m_swapchainVK;
		ImageViewVK m_imageViewVK;
		PipelineVK m_pipeLineVK;
		FrameBufferVK m_frameBufferVK;
		BufferVK m_bufferVK;
		CommandBufferVK m_commandBufferVK;
		SemaphoreVK m_semaphoreVK;

		/**
		* Starts the game and creates everything about vulkan.
		*/
		void start(GameInfo &gameInfo);

		/**
		* Disable all provided things by vulkan.
		*/
		void destroy();

		/**
		* Draw a single frame and give its instructions.
		*/
		void drawFrame();

		/**
		* Recreate the swapchain and justify the content when the window is resized.
		*
		* @param window The new window.
		* @param width The width of the new window.
		* @param height The height of the new window.
		* @param pipeline specifies whether the code should restart the pipeline.
		*/
		void resize(GLFWwindow *window, int width, int height, const bool &pipeline);

		//Getter of <var>m_windowVK -> m_window<var>
		GLFWwindow *getWindow() const;
	};

	static VulkanVK *s_vulkanInstance;
}



