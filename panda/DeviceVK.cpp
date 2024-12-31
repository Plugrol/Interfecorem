#include "DeviceVK.h"

core::graphic::vulkan::DeviceVK::DeviceVK() = default;

void core::graphic::vulkan::DeviceVK::init(const VkInstance &instance, const VkSurfaceKHR &surface) {
	VkPhysicalDevice_T const* bestGPU = s_physicalDevice(instance);
	this->m_physicalDevice = PhysicalDeviceVK(bestGPU);

	c_device();

	vkGetDeviceQueue(this->m_device, this->m_queueIndex, 0, &this->m_queue);
	this->m_physicalDevice.debug(surface);
}

void core::graphic::vulkan::DeviceVK::destroy() {
	if (this->m_device != VK_NULL_HANDLE) {
		vkDestroyDevice(this->m_device, nullptr);
		this->m_device = VK_NULL_HANDLE;
	}
}

void core::graphic::vulkan::DeviceVK::c_device() {
	VkDeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.pNext = nullptr;
	deviceQueueCreateInfo.flags = 0;
	deviceQueueCreateInfo.queueFamilyIndex = 0;
	deviceQueueCreateInfo.queueCount = 1;
	constexpr float queuePriorityValue = 1.0f;
	deviceQueueCreateInfo.pQueuePriorities = &queuePriorityValue;

	constexpr VkPhysicalDeviceFeatures deviceFeatures = {};

	const std::vector deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
	deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	CATCH(vkCreateDevice(this->m_physicalDevice.getDevice(), &deviceCreateInfo, nullptr, &this->m_device));
}

VkPhysicalDevice core::graphic::vulkan::DeviceVK::s_physicalDevice(const VkInstance &instance) {
	uint32_t amountOfGraphicCards = 0;
	vkEnumeratePhysicalDevices(instance, &amountOfGraphicCards, nullptr);
	auto *physicalDevices = new VkPhysicalDevice[amountOfGraphicCards];

	CATCH(vkEnumeratePhysicalDevices(instance, &amountOfGraphicCards, physicalDevices));

	unsigned int bestGPU = 0;
	uint32_t maxHeap = 0;

	for (unsigned int i = 0; i < amountOfGraphicCards; i++) {
		PhysicalDeviceVK physicalDevice(physicalDevices[i]);

		auto [memoryTypeCount, memoryTypes, memoryHeapCount, memoryHeaps] = physicalDevice.
				r_physicalDeviceMemoryProperties();

		const auto heapsPointer = memoryHeaps;
		auto heaps = std::vector(heapsPointer, heapsPointer + memoryHeapCount);

		for (const auto &[size, flags]: heaps) {
			if (flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
				if (maxHeap < size) {
					maxHeap = size;
					bestGPU = i;
				}
			}
		}
	}

	return physicalDevices[bestGPU];
}

uint32_t core::graphic::vulkan::DeviceVK::s_queueFamily(const std::vector<VkQueueFamilyProperties> &queueFamilies) {
	return 0;
}

VkDevice core::graphic::vulkan::DeviceVK::getDevice() const {
	return this->m_device;
}

VkQueue core::graphic::vulkan::DeviceVK::getQueue() const {
	return this->m_queue;
}

core::graphic::vulkan::PhysicalDeviceVK core::graphic::vulkan::DeviceVK::getPhysicalDevice() const {
	return this->m_physicalDevice;
}

uint32_t core::graphic::vulkan::DeviceVK::getQueueIndex() const {
	return this->m_queueIndex;
}
