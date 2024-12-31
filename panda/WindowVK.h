#pragma once
#include "VulkanCreator.h"
#include "InstanceVK.h"

namespace core::graphic {
    class WindowVK {
        GLFWwindow *m_window = nullptr;
        VkSurfaceKHR m_surface = nullptr;
        vulkan::InstanceVK m_instanceVK;

    public:
        //Mostly unused
        WindowVK();

        /**
            * Start the creation of the window and the instance.
            *
            * @param GameInfo& The informations, that were given at the start of the program.
            */
        void init(const GameInfo &gameInfo);

        /**
            * Destroy the window of glfw, after shutting down vulkan completely
            */
        void destroy();

        //Getter of <var*>window</var*>
        GLFWwindow *getWindow() const;

        //Getter of <var*>surface</var*>
        VkSurfaceKHR getSurface() const;

        //Getter of <var*>InstanceVK -> instance</var*>
        VkInstance getInstance() const;

        //Setter of <var*>m_window<var*>
        void setWindow(GLFWwindow *window);
    };
}



