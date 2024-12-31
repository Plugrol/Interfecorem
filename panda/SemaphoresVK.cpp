#include "SemaphoreVK.h"

namespace core::graphic::vulkan {

    SemaphoreVK::SemaphoreVK() = default;

    void SemaphoreVK::init(const VkDevice& device)
    {
        if (m_semaphore != nullptr) {
            throw std::runtime_error("SemaphoreVK has already been initialized.");
        }

        m_semaphore = new VkSemaphore[2];
        c_semaphores(device);
    }

    void SemaphoreVK::destroy(const VkDevice& device)
    {
        if (m_semaphore != nullptr)
        {
            for (int i = 0; i < 2; i++)
            {
                if (m_semaphore[i] != VK_NULL_HANDLE) {
                    vkDestroySemaphore(device, m_semaphore[i], nullptr);
                    m_semaphore[i] = VK_NULL_HANDLE; // Clear after destruction
                }
            }
            delete[] m_semaphore;
            m_semaphore = nullptr; // Reset to nullptr after deletion
        }
    }

    void SemaphoreVK::c_semaphores(const VkDevice& device) const {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        for (int i = 0; i < 2; i++)
        {
            if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_semaphore[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create semaphore.");
            }
        }
    }

    VkSemaphore* SemaphoreVK::getSemaphores() const
    {
        return m_semaphore;
    }

}
