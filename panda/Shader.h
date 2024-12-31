#pragma once
#include "VulkanCreator.h"

namespace core::graphic {
	namespace vulkan {
		class GLSLVertex {
		public:
			glm::vec2 m_position;
			glm::vec3 m_color;

			GLSLVertex(const glm::vec2 position, const glm::vec3 color) : m_position(position), m_color(color) {
			}

			static void c_inputDescription(std::vector<VkVertexInputAttributeDescription> &descriptions);
		};
	}

	struct ShaderModule {
		VkShaderModule shaderModule;
		//Do not change to m_ it's a struct you idiot //you(me) did it two times and had to rollback changes :c
		VkShaderStageFlagBits shaderType;
	};

	class Shader {
		std::vector<ShaderModule> m_shaderModule;
		std::vector<vulkan::GLSLVertex> m_vertices;
		std::vector<uint32_t> m_indices;

		//Vertex
		std::vector<VkVertexInputAttributeDescription> m_vertexInputAttributeDescriptions;

		/**
		* Creates the modules of the shader.
		*
		* @param device The information of the device, where the shader will run on.
		* @param code The glsl script, that will be loaded into the module
		* @param shaderModule The description of the type, that shader is about.
		*/
		void c_shaderModule(const VkDevice &device, const std::vector<char> &code, ShaderModule shaderModule);

		/**
		* Loads the content of a file and return it.
		*
		* @param fileName The name of the file.
		*/
		static std::vector<char> r_shader(const std::string &fileName);

	public:
		//Mostly unused
		Shader();

		/**
		* Starts filling the members with the class
		* and setup everything, so the shaders can be used.
		*
		* @param device The information that is needed to set up the shader
		* @param vertexShaderName The name of the file that contains vertex glsl
		* @param fragmentShaderName The name of the file that contains fragment glsl
		*/
		void init(const VkDevice &device, const std::string &vertexShaderName, const std::string &fragmentShaderName);

		/**
		* This method does clear the shaders out of vulkan and clear the vector.
		*
		* @param device The information that is needed to take out the shaders.
		*/
		void destroy(const VkDevice &device);

		static VkVertexInputBindingDescription c_bindingDescription();

		//Getter of <var>m_shaderModule</var>
		[[nodiscard]] std::vector<ShaderModule> getShaderModule() const;

		[[nodiscard]] std::vector<vulkan::GLSLVertex> getVertexVertices() const;

		[[nodiscard]] std::vector<uint32_t> getIndices() const;

		[[nodiscard]] std::vector<VkVertexInputAttributeDescription> getVertexInputAttributeDescriptions() const;
	};
}


