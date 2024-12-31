#include "WindowVK.h"

core::graphic::WindowVK::WindowVK() = default;

void core::graphic::WindowVK::init(const GameInfo& gameInfo)
{
    //Create Vulkan instance
    this->m_instanceVK = core::graphic::vulkan::InstanceVK();
    this->m_instanceVK.init(gameInfo);

    CATCH(glfwCreateWindowSurface(this->m_instanceVK.getInstance(), this->m_window, nullptr, &this->m_surface));
}

void core::graphic::WindowVK::destroy() 
{

    if (this->m_window != nullptr)
    {
        //First vulkan, then the window.
        vkDestroySurfaceKHR(getInstance(), this->m_surface, nullptr);
        this->m_instanceVK.destroy();

        //Finally shutdown the window.
        glfwDestroyWindow(this->m_window);
        glfwTerminate();

        this->m_window = nullptr;
    }
}

GLFWwindow* core::graphic::WindowVK::getWindow() const
{
    return this->m_window;
}

VkSurfaceKHR core::graphic::WindowVK::getSurface() const
{
    return this->m_surface;
}

VkInstance core::graphic::WindowVK::getInstance() const
{
    return this->m_instanceVK.getInstance();
}

void core::graphic::WindowVK::setWindow(GLFWwindow* window)
{
    this->m_window = window;
}
