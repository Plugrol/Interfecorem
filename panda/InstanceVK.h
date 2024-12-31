#pragma once
#include "VulkanCreator.h"

namespace core::graphic::vulkan {
    class InstanceVK {
        VkInstance m_instance = VK_NULL_HANDLE;

        /**
          * Create the instance of vulkan.
          *
          * @param gameInfo Information about the version and the name of the game
          */
        void c_instance(const GameInfo &gameInfo);

    public:
        //Mostly unused.
        InstanceVK();

        /**
          * Create an instance of vulkan and save it.
          *
          * @param gameInfo The info of the game. In this case the version is needed.
          */
        void init(const GameInfo &gameInfo);

        /**
          * Remove the instance while shutting down the game
          */
        void destroy();

        //Getter of <var>instance</var>
        [[nodiscard]] VkInstance getInstance() const;
    };
}

