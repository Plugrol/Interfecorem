#include "Shader.h"
#include "VulkanVK.h"
#include <fstream>

void core::graphic::vulkan::GLSLVertex::c_inputDescription(std::vector<VkVertexInputAttributeDescription>& descriptions)
{
	VkVertexInputAttributeDescription vertexInputAttributeDescription;
	for (int i = 0; i < 2; i++)
	{
		vertexInputAttributeDescription.location = i;
		vertexInputAttributeDescription.binding = 0;
		if (i == 0)
		{
			vertexInputAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
			vertexInputAttributeDescription.offset = offsetof(GLSLVertex, m_position);
		}
		else
		{
			vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexInputAttributeDescription.offset = offsetof(GLSLVertex, m_color);
		}
		descriptions.resize(i + 1);
		descriptions[i] = vertexInputAttributeDescription;
	}
}

core::graphic::Shader::Shader() { /* Ignored */ }

void core::graphic::Shader::init(const VkDevice& device, const std::string &vertexShaderName, const std::string &fragmentShaderName)
{
	this->m_vertices = 
	{ 
		vulkan::GLSLVertex{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		vulkan::GLSLVertex{{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
		vulkan::GLSLVertex{{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},

		vulkan::GLSLVertex{{ 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	};

	this->m_indices =
	{
		0, 1, 2, 0, 3, 1
	};

	vulkan::GLSLVertex::c_inputDescription(this->m_vertexInputAttributeDescriptions);

	const auto vertexShader = r_shader(vertexShaderName);
	const auto fragmentShader = r_shader(fragmentShaderName);

	c_shaderModule(device, vertexShader, ShaderModule{ VK_NULL_HANDLE,  VK_SHADER_STAGE_VERTEX_BIT });
	c_shaderModule(device, fragmentShader, ShaderModule{ VK_NULL_HANDLE,  VK_SHADER_STAGE_FRAGMENT_BIT });
}

void core::graphic::Shader::destroy(const VkDevice& device)
{
	for (auto &[shaderModule, shaderType] : this->m_shaderModule)
	{
		vkDestroyShaderModule(device, shaderModule, nullptr);
	}
	this->m_shaderModule.clear();

}

VkVertexInputBindingDescription core::graphic::Shader::c_bindingDescription()
{
	VkVertexInputBindingDescription vertexInputDescription;
	vertexInputDescription.binding = 0;
	vertexInputDescription.stride = sizeof(core::graphic::vulkan::GLSLVertex);
	vertexInputDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return vertexInputDescription;
}

void core::graphic::Shader::c_shaderModule(const VkDevice& device, const std::vector<char> &code, ShaderModule shaderModule)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo;
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pNext = nullptr;
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = (uint32_t*)code.data();

	CATCH(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule.shaderModule));

	this->m_shaderModule.resize(this->m_shaderModule.size() + 1);
	this->m_shaderModule[this->m_shaderModule.size() - 1] = shaderModule;
}

std::vector<char> core::graphic::Shader::r_shader(const std::string& fileName)
{
	if (std::ifstream file(fileName, std::ios::binary | std::ios::ate); file)
	{
		const size_t fileSize = file.tellg();
		std::vector<char> fileBuffer(fileSize);
		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);
		file.close();
		return fileBuffer;
	}

	throw std::runtime_error("Cannot read the file " + fileName + " in ");
}

std::vector<core::graphic::ShaderModule> core::graphic::Shader::getShaderModule() const
{
	return this->m_shaderModule;
}

std::vector<core::graphic::vulkan::GLSLVertex> core::graphic::Shader::getVertexVertices() const
{
	return this->m_vertices;
}
std::vector<uint32_t> core::graphic::Shader::getIndices() const
{
	return this->m_indices;
}

std::vector<VkVertexInputAttributeDescription> core::graphic::Shader::getVertexInputAttributeDescriptions() const
{
	return this->m_vertexInputAttributeDescriptions;
}