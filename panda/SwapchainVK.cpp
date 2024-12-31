#include "SwapchainVK.h"
#include <stack>

core::graphic::vulkan::SwapchainVK::SwapchainVK() = default;

core::graphic::vulkan::SwapchainVK::SwapchainVK(const VkSwapchainKHR oldSwapchain) {
    this->m_swapchain = oldSwapchain;
}

void core::graphic::vulkan::SwapchainVK::init(const GameInfo &gameInfo, const DeviceVK &device,
                                              const VkSurfaceKHR &surface) {
    this->m_format = s_swapchainFormat(device.getPhysicalDevice(), surface);
    VkPresentModeKHR presentMode = s_presentMode(device.getPhysicalDevice(), surface);

    if (!device.getPhysicalDevice().r_physicalDeviceSupportSwapchain(surface)) {
        std::cerr << "Surface is not supported";
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = s_imageCount(device.getPhysicalDevice(), surface);
    swapchainCreateInfo.imageFormat = this->m_format.colorFormat;
    swapchainCreateInfo.imageColorSpace = this->m_format.colorSpace;
    swapchainCreateInfo.imageExtent = gameInfo.getWindowScale;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = device.getQueueIndex();
    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;

    VkSwapchainKHR swapchain = this->m_swapchain;
    if (this->m_swapchain != VK_NULL_HANDLE) {
        this->destroy(device.getDevice());
    }
    swapchainCreateInfo.oldSwapchain = swapchain;

    CATCH(vkCreateSwapchainKHR(device.getDevice(), &swapchainCreateInfo, nullptr, &this->m_swapchain));
}

void core::graphic::vulkan::SwapchainVK::destroy(const VkDevice &device) {
    if (this->m_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, this->m_swapchain, nullptr);
        this->m_swapchain = VK_NULL_HANDLE;
    }
}

core::graphic::vulkan::VulkanSwapchainFormat core::graphic::vulkan::SwapchainVK::s_swapchainFormat(
    const PhysicalDeviceVK gpu, const VkSurfaceKHR &surface) {
    std::vector<VkSurfaceFormatKHR> availableSurfaceFormats = gpu.r_supportedFormats(surface);

    if (auto[format, colorSpace]{availableSurfaceFormats[0]}; availableSurfaceFormats.size() == 1 && format == VK_FORMAT_UNDEFINED) {
        return VulkanSwapchainFormat{VK_COLORSPACE_SRGB_NONLINEAR_KHR, VK_FORMAT_B8G8R8A8_UNORM};
    }

    for (const auto &[format, colorSpace]: availableSurfaceFormats) {
        if (format == VK_FORMAT_B8G8R8A8_UNORM) {
            return VulkanSwapchainFormat{colorSpace, format};
        }
    }
}

VkPresentModeKHR core::graphic::vulkan::SwapchainVK::s_presentMode(const PhysicalDeviceVK gpu, const VkSurfaceKHR &surface) {
    const std::vector<VkPresentModeKHR> availableModes = gpu.r_supportedPresentModes(surface);
    std::stack<VkPresentModeKHR> preferredModes;
    preferredModes.push(VK_PRESENT_MODE_IMMEDIATE_KHR);
    preferredModes.push(VK_PRESENT_MODE_FIFO_RELAXED_KHR);
    preferredModes.push(VK_PRESENT_MODE_FIFO_KHR);
    preferredModes.push(VK_PRESENT_MODE_MAILBOX_KHR);

    while (!preferredModes.empty()) {
        VkPresentModeKHR mode{preferredModes.top()};

        for (const auto& availableMode : availableModes) {
            if (availableMode == mode) {
                return mode;
            }
        }


        preferredModes.pop();
    }
}

uint32_t core::graphic::vulkan::SwapchainVK::s_imageCount(PhysicalDeviceVK gpu, const VkSurfaceKHR &surface) {
    const VkSurfaceCapabilitiesKHR surfaceCapabilitiesKHR = gpu.r_surfaceCapabilitiesKHR(surface);
    uint32_t minImageCount{surfaceCapabilitiesKHR.minImageCount + 1};

    const uint32_t maxImageCount{
        surfaceCapabilitiesKHR.maxImageCount
    };

    if (maxImageCount > 0 && minImageCount > maxImageCount) {
        minImageCount = maxImageCount;
    }

    return minImageCount;
}

VkSwapchainKHR core::graphic::vulkan::SwapchainVK::getSwapchain() const {
    return this->m_swapchain;
}

core::graphic::vulkan::VulkanSwapchainFormat core::graphic::vulkan::SwapchainVK::getFormat() const {
    return this->m_format;
}
