#include "InstanceVK.h"

core::graphic::vulkan::InstanceVK::InstanceVK() = default;

void core::graphic::vulkan::InstanceVK::init(const GameInfo &gameInfo)
{
    c_instance(gameInfo);
}

void core::graphic::vulkan::InstanceVK::destroy() 
{
    if (this->m_instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(this->m_instance, nullptr);
        this->m_instance = VK_NULL_HANDLE;
    }
}

void core::graphic::vulkan::InstanceVK::c_instance(const GameInfo& gameInfo)
{
    //Creating the info about the app
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = gameInfo.gameName;
    appInfo.applicationVersion = gameInfo.gameVersion;
    appInfo.pEngineName = "Project Panda";
    appInfo.engineVersion = gameInfo.gameVersion;
    appInfo.apiVersion = VK_API_VERSION_1_2;

    //Validate Layers and extensions
    const std::vector validationLayers = { "VK_LAYER_KHRONOS_validation" };

    //glfw automatically provides which extensions the progamm needs.
    uint32_t amountOfGlfwExtensions;
    const auto glfwExtensions = glfwGetRequiredInstanceExtensions(&amountOfGlfwExtensions);

    //Create the info about the instance
    VkInstanceCreateInfo InstanceCreateInfo;
    InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceCreateInfo.pNext = nullptr;
    InstanceCreateInfo.flags = 0;
    InstanceCreateInfo.pApplicationInfo = &appInfo;
    InstanceCreateInfo.enabledLayerCount = validationLayers.size();
    InstanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    InstanceCreateInfo.enabledExtensionCount = amountOfGlfwExtensions;
    InstanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;

    //Create the instance and catch any errors.
    CATCH(vkCreateInstance(&InstanceCreateInfo, nullptr, &this->m_instance));
}

VkInstance core::graphic::vulkan::InstanceVK::getInstance() const
{
    return this->m_instance;
}