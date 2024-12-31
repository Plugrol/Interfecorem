#pragma once
#include "VulkanCreator.h"
#include "RenderPassVK.h"
#include "Shader.h"

namespace core::graphic::vulkan {
	class PipelineVK
	{
		Shader m_shader;
		RenderPassVK m_renderPass;
		std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;

		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
		VkPipeline m_pipeline = VK_NULL_HANDLE;

		VkDescriptorSetLayout m_descriptiorSetLayout = VK_NULL_HANDLE;


		void c_descriptorLayout(const VkDevice& device);

		/**
		* Creates the pipeline.
		*
		* @param gameInfo The info about the screen size.
		* @param device The device, this pipeline should run on.
		* @param format The format of the swapchain, to create the pipeline.
		*/
		void c_pipeline(const GameInfo& gameInfo, const VkDevice& device, const VulkanSwapchainFormat& format);

		/**
		* Creates the stage info from the shader
		*
		* @param index of the shader.
		* 0 = vertex
		* 1 = fragment
		* 2 = coming soon maybe
		*/
		void c_shaderStageInfo(const int &index);

		/**
		* Read the shaders and create them.
		* @param device The device, this shaders should run on.
		*/
		void r_shaders(const VkDevice& device);
	public:

		//Mostly unused
		PipelineVK();

		/**
		* Creates the pipeline and its components.
		*
		* @param gameInfo The info about the screen size.
		* @param device The device, this pipeline should run on.
		* @param format The format of the swap chain, to create the pipeline.
		*/
		void init(const GameInfo& gameInfo, const VkDevice& device, const VulkanSwapchainFormat& format);

		/**
		* Destroys the pipeline and its components
		*
		* @param device The device, where the pipeline is running on.
		*/
		void destroy(const VkDevice& device);

		//Getter of <var>m_shader</var>
		[[nodiscard]] Shader getShader() const;

		//Getter of <var>m_renderPassVK</var>
		[[nodiscard]] RenderPassVK gerRenderPassVK() const;

		//Getter of <var>m_shaderStages</var>
		[[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> getShaderStages() const;

		//Getter of <var>m_pipelineLayout</var>
		[[nodiscard]] VkPipelineLayout getPipelineLayout() const;

		//Getter of <var>m_pipeline</var>
		[[nodiscard]] VkPipeline getPipeline() const;

		//Getter of <var>m_descriptiorSetLayout</var>
		[[nodiscard]] VkDescriptorSetLayout getVkDescriptorSetLayout() const;
	};
}
