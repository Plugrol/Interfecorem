#include "VulkanVK.h"

void core::graphic::vulkan::VulkanVK::drawFrame() const {
    const VkSwapchainKHR swapchain = this->m_swapchainVK.getSwapchain();

    uint32_t imageIndex;
    vkAcquireNextImageKHR(this->m_deviceVK.getDevice(), this->m_swapchainVK.getSwapchain(),
                          std::numeric_limits<uint64_t>::max(), this->m_semaphoreVK.getSemaphores()[0], VK_NULL_HANDLE,
                          &imageIndex);

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &this->m_semaphoreVK.getSemaphores()[0];
    VkPipelineStageFlags waitStageMask[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = waitStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(this->m_commandBufferVK.getCommandBuffer()[imageIndex]);
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &(this->m_semaphoreVK.getSemaphores()[1]);

    CATCH(vkQueueSubmit(this->m_deviceVK.getQueue(), 1, &submitInfo, VK_NULL_HANDLE));

    VkPresentInfoKHR presentInfoKHR;
    presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfoKHR.pNext = nullptr;
    presentInfoKHR.waitSemaphoreCount = 1;
    presentInfoKHR.pWaitSemaphores = &(this->m_semaphoreVK.getSemaphores()[1]);
    presentInfoKHR.swapchainCount = 1;
    presentInfoKHR.pSwapchains = &swapchain;
    presentInfoKHR.pImageIndices = &imageIndex;
    presentInfoKHR.pResults = nullptr;

    CATCH(vkQueuePresentKHR(this->m_deviceVK.getQueue(), &presentInfoKHR));
}

void core::graphic::vulkan::VulkanVK::start(GameInfo &gameInfo) {
    s_vulkanInstance = this;
    this->m_gameInfo = &gameInfo;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    std::string title(gameInfo.gameName);
    title
            .append(" build ")
            .append(std::to_string(VK_VERSION_MAJOR(gameInfo.gameVersion)))
            .append(".").append(std::to_string(VK_VERSION_MINOR(gameInfo.gameVersion)))
            .append(".").append(std::to_string(VK_VERSION_PATCH(gameInfo.gameVersion)));

    title.append(" (debug)"); //!!DEBUG

    this->m_windowVK.setWindow((glfwCreateWindow(gameInfo.getWindowScale.width, gameInfo.getWindowScale.height,
                                                 title.c_str(), nullptr, nullptr)));
    glfwSetWindowSizeCallback(this->m_windowVK.getWindow(), [](GLFWwindow *window, int width, int height) {
        s_vulkanInstance->resize(window, width, height, false);
    });

    this->m_windowVK.init(gameInfo);
    this->m_deviceVK.init(this->m_windowVK.getInstance(), this->m_windowVK.getSurface());

    resize(this->m_windowVK.getWindow(), gameInfo.getWindowScale.width, gameInfo.getWindowScale.height, true);

    this->m_semaphoreVK.init(this->m_deviceVK.getDevice());
}

void core::graphic::vulkan::VulkanVK::destroy() {
    vkDeviceWaitIdle(this->m_deviceVK.getDevice());

    this->m_bufferVK.destroy(this->m_deviceVK.getDevice(), this->m_pipeLineVK.getVkDescriptorSetLayout());
    this->m_semaphoreVK.destroy(this->m_deviceVK.getDevice());
    this->m_commandBufferVK.destroy(this->m_deviceVK.getDevice(), this->m_imageViewVK.getAmountOfImages());
    this->m_frameBufferVK.destroy(this->m_deviceVK.getDevice(), this->m_imageViewVK.getAmountOfImages());
    this->m_pipeLineVK.destroy(this->m_deviceVK.getDevice());
    this->m_imageViewVK.destroy(this->m_deviceVK.getDevice());
    this->m_swapchainVK.destroy(this->m_deviceVK.getDevice());

    this->m_deviceVK.destroy();
    this->m_windowVK.destroy();
}

void core::graphic::vulkan::VulkanVK::resize(GLFWwindow *window, int width, int height, const bool &pipeline) {

    const VkSurfaceCapabilitiesKHR surfaceCapabilites = this->m_deviceVK.getPhysicalDevice().r_surfaceCapabilitiesKHR(
        this->m_windowVK.getSurface());
    if (width > surfaceCapabilites.maxImageExtent.width) {
        width = surfaceCapabilites.maxImageExtent.width;
    }

    if (height > surfaceCapabilites.maxImageExtent.height) {
        height = surfaceCapabilites.maxImageExtent.height;
    }

    if (width < 400) {
        width = 400;
    }

    if (height < 400) {
        height = 400;
    }

    glfwSetWindowSize(window, width, height);


    vkDeviceWaitIdle(this->m_deviceVK.getDevice());

    this->m_commandBufferVK.destroy(this->m_deviceVK.getDevice(), this->m_imageViewVK.getAmountOfImages());
    this->m_frameBufferVK.destroy(this->m_deviceVK.getDevice(), this->m_imageViewVK.getAmountOfImages());

    if (pipeline) {
        this->m_pipeLineVK.destroy(this->m_deviceVK.getDevice());
    }

    this->m_imageViewVK.destroy(this->m_deviceVK.getDevice());
    this->m_swapchainVK.destroy(this->m_deviceVK.getDevice());

    this->m_gameInfo->getWindowScale = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    this->m_swapchainVK.init(*this->m_gameInfo, this->m_deviceVK, this->m_windowVK.getSurface());
    this->m_imageViewVK.init(this->m_deviceVK.getDevice(), this->m_swapchainVK);

    if (pipeline) {
        this->m_pipeLineVK.init(*this->m_gameInfo, this->m_deviceVK.getDevice(), this->m_swapchainVK.getFormat());
    }

    this->m_frameBufferVK.init(*this->m_gameInfo, this->m_deviceVK.getDevice(),
                               this->m_pipeLineVK.gerRenderPassVK().getRenderPass(), this->m_imageViewVK.getImageView(),
                               this->m_imageViewVK.getAmountOfImages());
    this->m_commandBufferVK.init(this->m_deviceVK, this->m_imageViewVK.getAmountOfImages());

    if (pipeline) {
        const VkDescriptorSetLayout layout = this->m_pipeLineVK.getVkDescriptorSetLayout();
        this->m_bufferVK.init(this->m_deviceVK, this->m_pipeLineVK.getShader(),
                              this->m_commandBufferVK.getCommandPool(), &layout);
    }


    this->m_commandBufferVK.sendCommands(*this->m_gameInfo, this->m_pipeLineVK, this->m_frameBufferVK.getFrameBuffer(),
                                         this->m_imageViewVK.getAmountOfImages(), this->m_bufferVK);
}

GLFWwindow *core::graphic::vulkan::VulkanVK::getWindow() const {
    return this->m_windowVK.getWindow();
}
