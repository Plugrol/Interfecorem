#include "PhysicalDeviceVK.h"

#include <DescriptorPoolVK.h>

core::graphic::vulkan::PhysicalDeviceVK::PhysicalDeviceVK(VkPhysicalDevice_T const* device)
{
    this->m_device = device;
}

core::graphic::vulkan::PhysicalDeviceVK::PhysicalDeviceVK()
{
    this->m_device = VK_NULL_HANDLE;
}

VkPhysicalDevice core::graphic::vulkan::PhysicalDeviceVK::getDevice() const {
    return const_cast<VkPhysicalDevice>(this->m_device);
}

VkPhysicalDeviceProperties core::graphic::vulkan::PhysicalDeviceVK::r_physicalDeviceProperties() const
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(this->getDevice(), &deviceProperties);
    return deviceProperties;
}

VkPhysicalDeviceFeatures core::graphic::vulkan::PhysicalDeviceVK::r_physicalDeviceFeatures() const
{
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(this->getDevice(), &deviceFeatures);
    return deviceFeatures;
}

VkPhysicalDeviceMemoryProperties core::graphic::vulkan::PhysicalDeviceVK::r_physicalDeviceMemoryProperties() const
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(this->getDevice(), &deviceMemoryProperties);
    return deviceMemoryProperties;
}

std::vector<VkQueueFamilyProperties> core::graphic::vulkan::PhysicalDeviceVK::r_queueFamilyProperties() const
{
    uint32_t queueFamilyCount = NULL;
    vkGetPhysicalDeviceQueueFamilyProperties(this->getDevice(), &queueFamilyCount, nullptr);
    auto* familyProperties = new VkQueueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(this->getDevice(), &queueFamilyCount, familyProperties);


    std::vector queueFamilies(familyProperties, familyProperties + queueFamilyCount);
    delete familyProperties;
    return queueFamilies;
}

VkSurfaceCapabilitiesKHR core::graphic::vulkan::PhysicalDeviceVK::r_surfaceCapabilitiesKHR(const VkSurfaceKHR &surface) const
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->getDevice(), surface, &surfaceCapabilities);
    return surfaceCapabilities;
}

std::vector<VkSurfaceFormatKHR> core::graphic::vulkan::PhysicalDeviceVK::r_supportedFormats(const VkSurfaceKHR &surface) const
{
    uint32_t amountOfFormats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(this->getDevice(), surface, &amountOfFormats, nullptr);
    auto* surfaceFormatsProperties = new VkSurfaceFormatKHR[amountOfFormats];
    vkGetPhysicalDeviceSurfaceFormatsKHR(this->getDevice(), surface, &amountOfFormats, surfaceFormatsProperties);

    std::vector surfaceFormats(surfaceFormatsProperties, surfaceFormatsProperties + amountOfFormats);
    delete surfaceFormatsProperties;
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> core::graphic::vulkan::PhysicalDeviceVK::r_supportedPresentModes(const VkSurfaceKHR &surface) const
{
    uint32_t amountOfPresentationModes = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(this->getDevice(), surface, &amountOfPresentationModes, nullptr);

    // Use a vector directly to manage memory.
    std::vector<VkPresentModeKHR> presentModes(amountOfPresentationModes);
    vkGetPhysicalDeviceSurfacePresentModesKHR(this->getDevice(), surface, &amountOfPresentationModes, presentModes.data());

    return presentModes;
}

VkBool32 core::graphic::vulkan::PhysicalDeviceVK::r_physicalDeviceSupportSwapchain(const VkSurfaceKHR &surface) const
{
    VkBool32 surfaceSupport = false;
    CATCH(vkGetPhysicalDeviceSurfaceSupportKHR(this->getDevice(), 0, surface, &surfaceSupport));
    return surfaceSupport;
}

void core::graphic::vulkan::PhysicalDeviceVK::debug(const VkSurfaceKHR &surface) const {
    uint32_t amountOfLayers = 0;
    vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
    auto* layers = new VkLayerProperties[amountOfLayers];
    vkEnumerateInstanceLayerProperties(&amountOfLayers, layers);

    std::cout << "----- Start getting layer info -----";
    std::cout << "> Amount " << amountOfLayers << std::endl;

    for (unsigned int i = 0; i < amountOfLayers; i++) {
        std::cout << std::endl;
        std::cout << "Layer           #" << i << std::endl;
        std::cout << "Name:           " << layers[i].layerName << std::endl;
        std::cout << "Spec. version:  " << layers[i].specVersion << std::endl;
        std::cout << "Impl. version:  " << layers[i].implementationVersion << std::endl;
        std::cout << "Description:    " << layers[i].description << std::endl;
    }
    std::cout << "----- End getting layer info -----";

    std::cout << std::endl << std::endl;

    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
    auto* extensions = new VkExtensionProperties[amountOfExtensions];
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensions);

    std::cout << "----- Start getting extension info -----";
    std::cout << "> Amount " << amountOfExtensions << std::endl;

    for (unsigned int i = 0; i < amountOfExtensions; i++) {
        std::cout << std::endl;
        std::cout << "Extension       #" << i << std::endl;
        std::cout << "Name:           " << extensions[i].extensionName << std::endl;
        std::cout << "Sepc. version:  " << extensions[i].specVersion << std::endl;
    }
    std::cout << "----- End getting extension info -----";

    std::cout << std::endl << std::endl;

    delete[] layers;
    delete[] extensions;

    std::cout << "----- Start getting graphic info -----" << std::endl;

    auto [apiVersion, driverVersion, vendorID, deviceID, deviceType, deviceName, pipelineCacheUUID, limits, sparseProperties] = r_physicalDeviceProperties();
    std::cout << "> Device Properties" << std::endl;
    std::cout << "apiVersion                                                : " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << std::endl;
    std::cout << "driverVersion                                             : " << VK_VERSION_MAJOR(driverVersion) << "." << VK_VERSION_MINOR(driverVersion) << "." << VK_VERSION_PATCH(driverVersion) << std::endl;
    std::cout << "vendorID                                                  : " << vendorID << std::endl;
    std::cout << "deviceID                                                  : " << deviceID << std::endl;
    std::cout << "deviceType                                                : (" << deviceType << ") called (";
    switch (deviceType)
    {
    case 0: std::cout << "VK_PHYSICAL_DEVICE_TYPE_OTHER";
        break;
    case 1: std::cout << "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
        break;
    case 2: std::cout << "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
        break;
    case 3: std::cout << "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
        break;
    case 4: std::cout << "VK_PHYSICAL_DEVICE_TYPE_CPU";
        break;
    default:
            std::cout << "UNKNOWN";
    }
    std::cout << ")" << std::endl;

    std::cout << "deviceName                                                : " << deviceName << std::endl;
    std::cout << "pipelineCacheUUID                                         : " << pipelineCacheUUID << std::endl << std::endl;

    std::cout << "> Limits" << std::endl;
    std::cout << "maxImageDimension1D                                       : " << limits.maxImageDimension1D << std::endl;
    std::cout << "maxImageDimension2D                                       : " << limits.maxImageDimension2D << std::endl;
    std::cout << "maxImageDimension3D                                       : " << limits.maxImageDimension3D << std::endl;
    std::cout << "maxImageDimensionCube                                     : " << limits.maxImageDimensionCube << std::endl;
    std::cout << "maxImageArrayLayers                                       : " << limits.maxImageArrayLayers << std::endl;
    std::cout << "maxTexelBufferElements                                    : " << limits.maxTexelBufferElements << std::endl;
    std::cout << "maxUniformBufferRange                                     : " << limits.maxUniformBufferRange << std::endl;
    std::cout << "maxStorageBufferRange                                     : " << limits.maxStorageBufferRange << std::endl;
    std::cout << "maxPushConstantsSize                                      : " << limits.maxPushConstantsSize << std::endl;
    std::cout << "maxMemoryAllocationCount                                  : " << limits.maxMemoryAllocationCount << std::endl;
    std::cout << "maxSamplerAllocationCount                                 : " << limits.maxSamplerAllocationCount << std::endl;
    std::cout << "bufferImageGranularity                                    : " << limits.bufferImageGranularity << std::endl;
    std::cout << "sparseAddressSpaceSize                                    : " << limits.sparseAddressSpaceSize << std::endl;
    std::cout << "maxBoundDescriptorSets                                    : " << limits.maxBoundDescriptorSets << std::endl;
    std::cout << "maxPerStageDescriptorSamplers                             : " << limits.maxPerStageDescriptorSamplers << std::endl;
    std::cout << "maxPerStageDescriptorUniformBuffers                       : " << limits.maxPerStageDescriptorUniformBuffers << std::endl;
    std::cout << "maxPerStageDescriptorStorageBuffers                       : " << limits.maxPerStageDescriptorStorageBuffers << std::endl;
    std::cout << "maxPerStageDescriptorSampledImages                        : " << limits.maxPerStageDescriptorSampledImages << std::endl;
    std::cout << "maxPerStageDescriptorStorageImages                        : " << limits.maxPerStageDescriptorStorageImages << std::endl;
    std::cout << "maxPerStageDescriptorInputAttachments                     : " << limits.maxPerStageDescriptorInputAttachments << std::endl;
    std::cout << "maxPerStageResources                                      : " << limits.maxPerStageResources << std::endl;
    std::cout << "maxDescriptorSetSamplers                                  : " << limits.maxDescriptorSetSamplers << std::endl;
    std::cout << "maxDescriptorSetUniformBuffers                            : " << limits.maxDescriptorSetUniformBuffers << std::endl;
    std::cout << "maxDescriptorSetUniformBuffersDynamic                     : " << limits.maxDescriptorSetUniformBuffersDynamic << std::endl;
    std::cout << "maxDescriptorSetStorageBuffers                            : " << limits.maxDescriptorSetStorageBuffers << std::endl;
    std::cout << "maxDescriptorSetStorageBuffersDynamic                     : " << limits.maxDescriptorSetStorageBuffersDynamic << std::endl;
    std::cout << "maxDescriptorSetSampledImages                             : " << limits.maxDescriptorSetSampledImages << std::endl;
    std::cout << "maxDescriptorSetStorageImages                             : " << limits.maxDescriptorSetStorageImages << std::endl;
    std::cout << "maxDescriptorSetInputAttachments                          : " << limits.maxDescriptorSetInputAttachments << std::endl;
    std::cout << "maxVertexInputAttributes                                  : " << limits.maxVertexInputAttributes << std::endl;
    std::cout << "maxVertexInputBindings                                    : " << limits.maxVertexInputBindings << std::endl;
    std::cout << "maxVertexInputAttributeOffset                             : " << limits.maxVertexInputAttributeOffset << std::endl;
    std::cout << "maxVertexInputBindingStride                               : " << limits.maxVertexInputBindingStride << std::endl;
    std::cout << "maxVertexOutputComponents                                 : " << limits.maxVertexOutputComponents << std::endl;
    std::cout << "maxTessellationGenerationLevel                            : " << limits.maxTessellationGenerationLevel << std::endl;
    std::cout << "maxTessellationPatchSize                                  : " << limits.maxTessellationPatchSize << std::endl;
    std::cout << "maxTessellationControlPerVertexInputComponents            : " << limits.maxTessellationControlPerVertexInputComponents << std::endl;
    std::cout << "maxTessellationControlPerVertexOutputComponents           : " << limits.maxTessellationControlPerVertexOutputComponents << std::endl;
    std::cout << "maxTessellationControlPerPatchOutputComponents            : " << limits.maxTessellationControlPerPatchOutputComponents << std::endl;
    std::cout << "maxTessellationControlTotalOutputComponents               : " << limits.maxTessellationControlTotalOutputComponents << std::endl;
    std::cout << "maxTessellationEvaluationInputComponents                  : " << limits.maxTessellationEvaluationInputComponents << std::endl;
    std::cout << "maxTessellationEvaluationOutputComponents                 : " << limits.maxTessellationEvaluationOutputComponents << std::endl;
    std::cout << "maxGeometryShaderInvocations                              : " << limits.maxGeometryShaderInvocations << std::endl;
    std::cout << "maxGeometryInputComponents                                : " << limits.maxGeometryInputComponents << std::endl;
    std::cout << "maxGeometryOutputComponents                               : " << limits.maxGeometryOutputComponents << std::endl;
    std::cout << "maxGeometryOutputVertices                                 : " << limits.maxGeometryOutputVertices << std::endl;
    std::cout << "maxGeometryTotalOutputComponents                          : " << limits.maxGeometryTotalOutputComponents << std::endl;
    std::cout << "maxFragmentInputComponents                                : " << limits.maxFragmentInputComponents << std::endl;
    std::cout << "maxFragmentOutputAttachments                              : " << limits.maxFragmentOutputAttachments << std::endl;
    std::cout << "maxFragmentDualSrcAttachments                             : " << limits.maxFragmentDualSrcAttachments << std::endl;
    std::cout << "maxFragmentCombinedOutputResources                        : " << limits.maxFragmentCombinedOutputResources << std::endl;
    std::cout << "maxComputeSharedMemorySize                                : " << limits.maxComputeSharedMemorySize << std::endl;
    std::cout << "maxComputeWorkGroupCount1                                 : " << limits.maxComputeWorkGroupCount[0] << std::endl;
    std::cout << "maxComputeWorkGroupCount2                                 : " << limits.maxComputeWorkGroupCount[1] << std::endl;
    std::cout << "maxComputeWorkGroupCount3                                 : " << limits.maxComputeWorkGroupCount[2] << std::endl;
    std::cout << "maxComputeWorkGroupInvocations                            : " << limits.maxComputeWorkGroupInvocations << std::endl;
    std::cout << "maxComputeWorkGroupSize1                                  : " << limits.maxComputeWorkGroupSize[0] << std::endl;
    std::cout << "maxComputeWorkGroupSize2                                  : " << limits.maxComputeWorkGroupSize[1] << std::endl;
    std::cout << "maxComputeWorkGroupSize3                                  : " << limits.maxComputeWorkGroupSize[2] << std::endl;
    std::cout << "subPixelPrecisionBits                                     : " << limits.subPixelPrecisionBits << std::endl;
    std::cout << "subTexelPrecisionBits                                     : " << limits.subTexelPrecisionBits << std::endl;
    std::cout << "mipmapPrecisionBits                                       : " << limits.mipmapPrecisionBits << std::endl;
    std::cout << "maxDrawIndexedIndexValue                                  : " << limits.maxDrawIndexedIndexValue << std::endl;
    std::cout << "maxDrawIndirectCount                                      : " << limits.maxDrawIndirectCount << std::endl;
    std::cout << "maxSamplerLodBias                                         : " << limits.maxSamplerLodBias << std::endl;
    std::cout << "maxSamplerAnisotropy                                      : " << limits.maxSamplerAnisotropy << std::endl;
    std::cout << "maxViewports                                              : " << limits.maxViewports << std::endl;
    std::cout << "maxViewportDimensions1                                    : " << limits.maxViewportDimensions[0] << std::endl;
    std::cout << "maxViewportDimensions2                                    : " << limits.maxViewportDimensions[1] << std::endl;
    std::cout << "viewportBoundsRange1                                      : " << limits.viewportBoundsRange[0] << std::endl;
    std::cout << "viewportBoundsRange2                                      : " << limits.viewportBoundsRange[1] << std::endl;
    std::cout << "viewportSubPixelBits                                      : " << limits.viewportSubPixelBits << std::endl;
    std::cout << "minMemoryMapAlignment                                     : " << limits.minMemoryMapAlignment << std::endl;
    std::cout << "minTexelBufferOffsetAlignment                             : " << limits.minTexelBufferOffsetAlignment << std::endl;
    std::cout << "minUniformBufferOffsetAlignment                           : " << limits.minUniformBufferOffsetAlignment << std::endl;
    std::cout << "minStorageBufferOffsetAlignment                           : " << limits.minStorageBufferOffsetAlignment << std::endl;
    std::cout << "minTexelOffset                                            : " << limits.minTexelOffset << std::endl;
    std::cout << "maxTexelOffset                                            : " << limits.maxTexelOffset << std::endl;
    std::cout << "minTexelGatherOffset                                      : " << limits.minTexelGatherOffset << std::endl;
    std::cout << "maxTexelGatherOffset                                      : " << limits.maxTexelGatherOffset << std::endl;
    std::cout << "minInterpolationOffset                                    : " << limits.minInterpolationOffset << std::endl;
    std::cout << "maxInterpolationOffset                                    : " << limits.maxInterpolationOffset << std::endl;
    std::cout << "subPixelInterpolationOffsetBits                           : " << limits.subPixelInterpolationOffsetBits << std::endl;
    std::cout << "maxFramebufferWidth                                       : " << limits.maxFramebufferWidth << std::endl;
    std::cout << "maxFramebufferHeight                                      : " << limits.maxFramebufferHeight << std::endl;
    std::cout << "maxFramebufferLayers                                      : " << limits.maxFramebufferLayers << std::endl;
    std::cout << "framebufferColorSampleCounts                              : " << limits.framebufferColorSampleCounts << std::endl;
    std::cout << "framebufferDepthSampleCounts                              : " << limits.framebufferDepthSampleCounts << std::endl;
    std::cout << "framebufferStencilSampleCounts                            : " << limits.framebufferStencilSampleCounts << std::endl;
    std::cout << "framebufferNoAttachmentsSampleCounts                      : " << limits.framebufferNoAttachmentsSampleCounts << std::endl;
    std::cout << "maxColorAttachments                                       : " << limits.maxColorAttachments << std::endl;
    std::cout << "sampledImageColorSampleCounts                             : " << limits.sampledImageColorSampleCounts << std::endl;
    std::cout << "sampledImageIntegerSampleCounts                           : " << limits.sampledImageIntegerSampleCounts << std::endl;
    std::cout << "sampledImageDepthSampleCounts                             : " << limits.sampledImageDepthSampleCounts << std::endl;
    std::cout << "sampledImageStencilSampleCounts                           : " << limits.sampledImageStencilSampleCounts << std::endl;
    std::cout << "storageImageSampleCounts                                  : " << limits.storageImageSampleCounts << std::endl;
    std::cout << "maxSampleMaskWords                                        : " << limits.maxSampleMaskWords << std::endl;
    std::cout << "timestampComputeAndGraphics                               : " << limits.timestampComputeAndGraphics << std::endl;
    std::cout << "timestampPeriod                                           : " << limits.timestampPeriod << std::endl;
    std::cout << "maxClipDistances                                          : " << limits.maxClipDistances << std::endl;
    std::cout << "maxCullDistances                                          : " << limits.maxCullDistances << std::endl;
    std::cout << "maxCombinedClipAndCullDistances                           : " << limits.maxCombinedClipAndCullDistances << std::endl;
    std::cout << "discreteQueuePriorities                                   : " << limits.discreteQueuePriorities << std::endl;
    std::cout << "pointSizeRange1                                           : " << limits.pointSizeRange[0] << std::endl;
    std::cout << "pointSizeRange2                                           : " << limits.pointSizeRange[1] << std::endl;
    std::cout << "lineWidthRange1                                           : " << limits.lineWidthRange[0] << std::endl;
    std::cout << "lineWidthRange2                                           : " << limits.lineWidthRange[1] << std::endl;
    std::cout << "pointSizeGranularity                                      : " << limits.pointSizeGranularity << std::endl;
    std::cout << "lineWidthGranularity                                      : " << limits.lineWidthGranularity << std::endl;
    std::cout << "strictLines                                               : " << limits.strictLines << std::endl;
    std::cout << "standardSampleLocations                                   : " << limits.standardSampleLocations << std::endl;
    std::cout << "optimalBufferCopyOffsetAlignment                          : " << limits.optimalBufferCopyOffsetAlignment << std::endl;
    std::cout << "optimalBufferCopyRowPitchAlignment                        : " << limits.optimalBufferCopyRowPitchAlignment << std::endl;
    std::cout << "nonCoherentAtomSize                                       : " << limits.nonCoherentAtomSize << std::endl << std::endl;

    std::cout << "> Sparse Properties" << std::endl;
    std::cout << "residencyStandard2DBlockShape                             : " << sparseProperties.residencyStandard2DBlockShape << std::endl;
    std::cout << "residencyStandard2DMultisampleBlockShape                  : " << sparseProperties.residencyStandard2DMultisampleBlockShape << std::endl;
    std::cout << "residencyStandard3DBlockShape                             : " << sparseProperties.residencyStandard3DBlockShape << std::endl;
    std::cout << "residencyAlignedMipSize                                   : " << sparseProperties.residencyAlignedMipSize << std::endl;
    std::cout << "residencyNonResidentStrict                                : " << sparseProperties.residencyNonResidentStrict << std::endl << std::endl;

    VkPhysicalDeviceFeatures deviceFeatures = r_physicalDeviceFeatures();
    std::cout << "> Device Features." << std::endl;
    std::cout << "robustBufferAccess                                        : " << deviceFeatures.robustBufferAccess << std::endl;
    std::cout << "fullDrawIndexUint32                                       : " << deviceFeatures.fullDrawIndexUint32 << std::endl;
    std::cout << "imageCubeArray                                            : " << deviceFeatures.imageCubeArray << std::endl;
    std::cout << "independentBlend                                          : " << deviceFeatures.independentBlend << std::endl;
    std::cout << "geometryShader                                            : " << deviceFeatures.geometryShader << std::endl;
    std::cout << "tessellationShader                                        : " << deviceFeatures.tessellationShader << std::endl;
    std::cout << "sampleRateShading                                         : " << deviceFeatures.sampleRateShading << std::endl;
    std::cout << "dualSrcBlend                                              : " << deviceFeatures.dualSrcBlend << std::endl;
    std::cout << "logicOp                                                   : " << deviceFeatures.logicOp << std::endl;
    std::cout << "multiDrawIndirect                                         : " << deviceFeatures.multiDrawIndirect << std::endl;
    std::cout << "drawIndirectFirstInstance                                 : " << deviceFeatures.drawIndirectFirstInstance << std::endl;
    std::cout << "depthClamp                                                : " << deviceFeatures.depthClamp << std::endl;
    std::cout << "depthBiasClamp                                            : " << deviceFeatures.depthBiasClamp << std::endl;
    std::cout << "fillModeNonSolid                                          : " << deviceFeatures.fillModeNonSolid << std::endl;
    std::cout << "depthBounds                                               : " << deviceFeatures.depthBounds << std::endl;
    std::cout << "wideLines                                                 : " << deviceFeatures.wideLines << std::endl;
    std::cout << "largePoints                                               : " << deviceFeatures.largePoints << std::endl;
    std::cout << "alphaToOne                                                : " << deviceFeatures.alphaToOne << std::endl;
    std::cout << "multiViewport                                             : " << deviceFeatures.multiViewport << std::endl;
    std::cout << "samplerAnisotropy                                         : " << deviceFeatures.samplerAnisotropy << std::endl;
    std::cout << "textureCompressionETC2                                    : " << deviceFeatures.textureCompressionETC2 << std::endl;
    std::cout << "textureCompressionASTC_LDR                                : " << deviceFeatures.textureCompressionASTC_LDR << std::endl;
    std::cout << "textureCompressionBC                                      : " << deviceFeatures.textureCompressionBC << std::endl;
    std::cout << "occlusionQueryPrecise                                     : " << deviceFeatures.occlusionQueryPrecise << std::endl;
    std::cout << "pipelineStatisticsQuery                                   : " << deviceFeatures.pipelineStatisticsQuery << std::endl;
    std::cout << "vertexPipelineStoresAndAtomics                            : " << deviceFeatures.vertexPipelineStoresAndAtomics << std::endl;
    std::cout << "fragmentStoresAndAtomics                                  : " << deviceFeatures.fragmentStoresAndAtomics << std::endl;
    std::cout << "shaderTessellationAndGeometryPointSize                    : " << deviceFeatures.shaderTessellationAndGeometryPointSize << std::endl;
    std::cout << "shaderImageGatherExtended                                 : " << deviceFeatures.shaderImageGatherExtended << std::endl;
    std::cout << "shaderStorageImageExtendedFormats                         : " << deviceFeatures.shaderStorageImageExtendedFormats << std::endl;
    std::cout << "shaderStorageImageMultisample                             : " << deviceFeatures.shaderStorageImageMultisample << std::endl;
    std::cout << "shaderStorageImageReadWithoutFormat                       : " << deviceFeatures.shaderStorageImageReadWithoutFormat << std::endl;
    std::cout << "shaderStorageImageWriteWithoutFormat                      : " << deviceFeatures.shaderStorageImageWriteWithoutFormat << std::endl;
    std::cout << "shaderUniformBufferArrayDynamicIndexing                   : " << deviceFeatures.shaderUniformBufferArrayDynamicIndexing << std::endl;
    std::cout << "shaderSampledImageArrayDynamicIndexing                    : " << deviceFeatures.shaderSampledImageArrayDynamicIndexing << std::endl;
    std::cout << "shaderStorageBufferArrayDynamicIndexing                   : " << deviceFeatures.shaderStorageBufferArrayDynamicIndexing << std::endl;
    std::cout << "shaderStorageImageArrayDynamicIndexing                    : " << deviceFeatures.shaderStorageImageArrayDynamicIndexing << std::endl;
    std::cout << "shaderClipDistance                                        : " << deviceFeatures.shaderClipDistance << std::endl;
    std::cout << "shaderCullDistance                                        : " << deviceFeatures.shaderCullDistance << std::endl;
    std::cout << "shaderFloat64                                             : " << deviceFeatures.shaderFloat64 << std::endl;
    std::cout << "shaderInt64                                               : " << deviceFeatures.shaderInt64 << std::endl;
    std::cout << "shaderInt16                                               : " << deviceFeatures.shaderInt16 << std::endl;
    std::cout << "shaderResourceResidency                                   : " << deviceFeatures.shaderResourceResidency << std::endl;
    std::cout << "shaderResourceMinLod                                      : " << deviceFeatures.shaderResourceMinLod << std::endl;
    std::cout << "sparseBinding                                             : " << deviceFeatures.sparseBinding << std::endl;
    std::cout << "sparseResidencyBuffer                                     : " << deviceFeatures.sparseResidencyBuffer << std::endl;
    std::cout << "sparseResidencyImage2D                                    : " << deviceFeatures.sparseResidencyImage2D << std::endl;
    std::cout << "sparseResidencyImage3D                                    : " << deviceFeatures.sparseResidencyImage3D << std::endl;
    std::cout << "sparseResidency2Samples                                   : " << deviceFeatures.sparseResidency2Samples << std::endl;
    std::cout << "sparseResidency4Samples                                   : " << deviceFeatures.sparseResidency4Samples << std::endl;
    std::cout << "sparseResidency8Samples                                   : " << deviceFeatures.sparseResidency8Samples << std::endl;
    std::cout << "sparseResidency16Samples                                  : " << deviceFeatures.sparseResidency16Samples << std::endl;
    std::cout << "sparseResidencyAliased                                    : " << deviceFeatures.sparseResidencyAliased << std::endl;
    std::cout << "variableMultisampleRate                                   : " << deviceFeatures.variableMultisampleRate << std::endl;
    std::cout << "inheritedQueries                                          : " << deviceFeatures.inheritedQueries << std::endl << std::endl;

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = r_physicalDeviceMemoryProperties();
    std::cout << "> Device Memory Properties" << std::endl;
    std::cout << "memoryTypeCount                                           : " << deviceMemoryProperties.memoryTypeCount << std::endl;
    std::cout << "memoryTypes.heapIndex                                     : " << deviceMemoryProperties.memoryTypes->heapIndex << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT           : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT           : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_HOST_COHERENT_BIT          : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_HOST_CACHED_BIT            : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT       : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_PROTECTED_BIT              : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD    : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD    : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) != 0) << std::endl;
    std::cout << "memoryTypes.VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM         : " << ((deviceMemoryProperties.memoryTypes->propertyFlags & VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM) != 0) << std::endl;
    std::cout << "memoryHeapCount                                           : " << deviceMemoryProperties.memoryHeapCount << std::endl;
    std::cout << "memoryHeaps.VK_MEMORY_HEAP_DEVICE_LOCAL_BIT               : " << ((deviceMemoryProperties.memoryHeaps->flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0) << std::endl;
    std::cout << "memoryHeaps.VK_MEMORY_HEAP_MULTI_INSTANCE_BIT             : " << ((deviceMemoryProperties.memoryHeaps->flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) != 0) << std::endl;
    std::cout << "memoryHeaps.VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR         : " << ((deviceMemoryProperties.memoryHeaps->flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR) != 0) << std::endl;
    std::cout << "memoryHeaps.VK_MEMORY_HEAP_FLAG_BITS_MAX_ENUM             : " << ((deviceMemoryProperties.memoryHeaps->flags & VK_MEMORY_HEAP_FLAG_BITS_MAX_ENUM) != 0) << std::endl;
    std::cout << "memoryHeaps.size                                          : " << deviceMemoryProperties.memoryHeaps->size << std::endl << std::endl;

    std::vector<VkQueueFamilyProperties> queueFamilyProperties = r_queueFamilyProperties();
    std::cout << "> Amount of Queue Families " << queueFamilyProperties.size() << std::endl;
    for (int i = 0; i < queueFamilyProperties.size(); i++)
    {
        std::cout << "  > Queue Family #" << i << std::endl;
        std::cout << "  VK_QUEUE_GRAPHICS_BIT                                   : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << std::endl;
        std::cout << "  VK_QUEUE_COMPUTE_BIT                                    : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
        std::cout << "  VK_QUEUE_TRANSFER_BIT                                   : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
        std::cout << "  VK_QUEUE_SPARSE_BINDING_BIT                             : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;
        std::cout << "  VK_QUEUE_PROTECTED_BIT                                  : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_PROTECTED_BIT) != 0) << std::endl;
        std::cout << "  VK_QUEUE_FLAG_BITS_MAX_ENUM                             : " << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_FLAG_BITS_MAX_ENUM) != 0) << std::endl;
        std::cout << "  queueCount                                              : " << queueFamilyProperties[i].queueCount << std::endl;
        std::cout << "  timestampValidBits                                      : " << queueFamilyProperties[i].timestampValidBits << std::endl;
        std::cout << "  minImageTransferGranularity.depth                       : " << queueFamilyProperties[i].minImageTransferGranularity.depth << std::endl;
        std::cout << "  minImageTransferGranularity.height                      : " << queueFamilyProperties[i].minImageTransferGranularity.height << std::endl;
        std::cout << "  minImageTransferGranularity.width                       : " << queueFamilyProperties[i].minImageTransferGranularity.width << std::endl << std::endl;
    }
    queueFamilyProperties.clear();

    VkSurfaceCapabilitiesKHR surfaceCapabilities = r_surfaceCapabilitiesKHR(surface);
    std::cout << "> SurfaceCapabilities" << std::endl;
    std::cout << "minImageCount                                             : " << surfaceCapabilities.minImageCount << std::endl;
    std::cout << "maxImageCount                                             : " << surfaceCapabilities.maxImageCount << std::endl;
    std::cout << "currentExtent                                             : " << surfaceCapabilities.currentExtent.width << " / " << surfaceCapabilities.currentExtent.height << std::endl;
    std::cout << "minImageExtent                                            : " << surfaceCapabilities.minImageExtent.width << " / " << surfaceCapabilities.minImageExtent.height << std::endl;
    std::cout << "maxImageExtent                                            : " << surfaceCapabilities.maxImageExtent.width << " / " << surfaceCapabilities.maxImageExtent.height << std::endl;
    std::cout << "maxImageArrayLayers                                       : " << surfaceCapabilities.maxImageArrayLayers << std::endl;
    std::cout << "supportedTransforms                                       : " << surfaceCapabilities.supportedTransforms << std::endl;
    std::cout << "currentTransform                                          : " << surfaceCapabilities.currentTransform << std::endl;
    std::cout << "supportedCompositeAlpha                                   : " << surfaceCapabilities.supportedCompositeAlpha << std::endl;
    std::cout << "supportedUsageFlags                                       : " << surfaceCapabilities.supportedUsageFlags << std::endl << std::endl;


    std::vector<VkSurfaceFormatKHR> surfaceFormatKHR = r_supportedFormats(surface);
    std::cout << "> Amount of Formats " << surfaceFormatKHR.size() << std::endl;
    for (auto & i : surfaceFormatKHR)
    {
        std::cout << "Format                                                    : " << i.format << std::endl;
    }
    surfaceFormatKHR.clear();

    std::vector<VkPresentModeKHR> presentModeKHR = r_supportedPresentModes(surface);
    std::cout << std::endl;
    std::cout << "> Amount of Presentation Modes " << presentModeKHR.size() << std::endl;
    for (auto & i : presentModeKHR)
    {
        std::cout << "Supported presentation mode                               : " << i << std::endl;
    }
    presentModeKHR.clear();

    std::cout << std::endl << "----- End getting graphic info -----";

    //To write that output took way to long :(
    //It would be beneath my dignity to give how long it took me.
}
