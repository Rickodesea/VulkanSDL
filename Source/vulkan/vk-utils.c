#include "vk-utils.h"
#include "vk-global.h"
#include "SDL2/SDL.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData
) 
{
    (void)(messageSeverity);
    (void)(messageType);
    (void)(pUserData);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", pCallbackData->pMessage);
    return VK_FALSE;
}


VkDebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerInfo() 
{
    VkDebugUtilsMessengerCreateInfoEXT info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info.pfnUserCallback = CallbackDebug;
    return info;
}

uint32_t GetPhysicalDeviceScore(VkPhysicalDeviceLimits limits) 
{
    uint32_t score = 0;
    score += limits.maxImageDimension1D;
    score += limits.maxImageDimension2D;
    score += limits.maxImageDimension3D;
    score += limits.maxImageDimensionCube;
    score += limits.maxImageArrayLayers;
    score += limits.maxTexelBufferElements;
    score += limits.maxUniformBufferRange;
    score += limits.maxStorageBufferRange;
    score += limits.maxPushConstantsSize;
    score += limits.maxMemoryAllocationCount;
    score += limits.maxSamplerAllocationCount;
    score += limits.maxBoundDescriptorSets;
    score += limits.maxPerStageDescriptorSamplers;
    score += limits.maxPerStageDescriptorUniformBuffers;
    score += limits.maxPerStageDescriptorStorageBuffers;
    score += limits.maxPerStageDescriptorSampledImages;
    score += limits.maxPerStageDescriptorStorageImages;
    score += limits.maxPerStageDescriptorInputAttachments;
    score += limits.maxPerStageResources;
    score += limits.maxDescriptorSetSamplers;
    score += limits.maxDescriptorSetUniformBuffers;
    score += limits.maxDescriptorSetUniformBuffersDynamic;
    score += limits.maxDescriptorSetStorageBuffers;
    score += limits.maxDescriptorSetStorageBuffersDynamic;
    score += limits.maxDescriptorSetSampledImages;
    score += limits.maxDescriptorSetStorageImages;
    score += limits.maxDescriptorSetInputAttachments;
    score += limits.maxVertexInputAttributes;
    score += limits.maxVertexInputBindings;
    score += limits.maxVertexInputAttributeOffset;
    score += limits.maxVertexInputBindingStride;
    score += limits.maxVertexOutputComponents;
    score += limits.maxTessellationGenerationLevel;
    score += limits.maxTessellationPatchSize;
    score += limits.maxTessellationControlPerVertexInputComponents;
    score += limits.maxTessellationControlPerVertexOutputComponents;
    score += limits.maxTessellationControlPerPatchOutputComponents;
    score += limits.maxTessellationControlTotalOutputComponents;
    score += limits.maxTessellationEvaluationInputComponents;
    score += limits.maxTessellationEvaluationOutputComponents;
    score += limits.maxGeometryShaderInvocations;
    score += limits.maxGeometryInputComponents;
    score += limits.maxGeometryOutputComponents;
    score += limits.maxGeometryOutputVertices;
    score += limits.maxGeometryTotalOutputComponents;
    score += limits.maxFragmentInputComponents;
    score += limits.maxFragmentOutputAttachments;
    score += limits.maxFragmentDualSrcAttachments;
    score += limits.maxFragmentCombinedOutputResources;
    score += limits.maxComputeSharedMemorySize;
    score += limits.maxComputeWorkGroupInvocations;
    score += limits.maxDrawIndexedIndexValue;
    score += limits.maxDrawIndirectCount;
    score += limits.maxSamplerLodBias;
    score += limits.maxSamplerAnisotropy;
    score += limits.maxViewports;
    score += limits.maxTexelOffset;
    score += limits.maxTexelGatherOffset;
    score += limits.maxInterpolationOffset;
    score += limits.maxFramebufferWidth;
    score += limits.maxFramebufferHeight;
    score += limits.maxFramebufferLayers;
    score += limits.framebufferColorSampleCounts;
    score += limits.framebufferDepthSampleCounts;
    score += limits.framebufferStencilSampleCounts;
    score += limits.framebufferNoAttachmentsSampleCounts;
    score += limits.maxColorAttachments;
    score += limits.sampledImageColorSampleCounts;
    score += limits.sampledImageIntegerSampleCounts;
    score += limits.sampledImageDepthSampleCounts;
    score += limits.sampledImageStencilSampleCounts;
    score += limits.storageImageSampleCounts;
    score += limits.maxSampleMaskWords;
    score += limits.maxClipDistances;
    score += limits.maxCullDistances;
    score += limits.maxCombinedClipAndCullDistances;

    return score;
}

VkSurfaceFormatKHR GetAvailableSurfaceFormat(VkFormat requested) 
{
    DEFINE_ENUMERATE_2(
        VkSurfaceFormatKHR,
        vkGetPhysicalDeviceSurfaceFormatsKHR,
        physicaldevice,
        surface,
        formats_count,
        formats,
        64
    )
    ;
    for (uint32_t i = 0; i < formats_count; i++)
    {
        if (requested == formats[i].format)
            return formats[i];
    }

    return formats[0];
}

uint32_t GetAvailableSwapchainSize(uint32_t requested) 
{
    if (capabilities_surface.maxImageCount == 0) {
        return requested;
    }

    if (requested <= capabilities_surface.maxImageCount) {
        return requested;
    }

    return capabilities_surface.minImageCount;
}

VkPresentModeKHR GetAvailablePresentMode(VkPresentModeKHR requested) 
{
    DEFINE_ENUMERATE_2(
        VkPresentModeKHR,
        vkGetPhysicalDeviceSurfacePresentModesKHR,
        physicaldevice,
        surface,
        modes_count,
        modes,
        4
    );
    for (uint32_t i = 0; i < modes_count; i++) {
        if (requested == modes[i])
            return requested;
    }

    return modes[0];
}

VkShaderModule CreateShaderModule(const char* path) 
{
    VkShaderModule shadermodule = VK_NULL_HANDLE;

    SDL_RWops* io = SDL_RWFromFile(path, "rb");

    if(io) {
        const uint32_t size = SDL_RWseek (io, 0, RW_SEEK_END) ;
        SDL_RWseek (io, 0, RW_SEEK_SET) ;
        uint32_t* buffer = calloc(size / sizeof(uint32_t) + 1, sizeof(uint32_t));
        memset(buffer, 0, size);
        SDL_RWread(io, buffer, size, 1);

        VkShaderModuleCreateInfo info_create = {};
        info_create.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        info_create.codeSize = size;
        info_create.pCode = buffer;

        if(ConfirmVkResult(vkCreateShaderModule(device, &info_create, 0, &shadermodule)) == 0) {
            SDL_Log("Failed Vulkan ShaderModule Creation");
            exit(EXIT_FAILURE);
        }
    }

    return shadermodule;
}

/*
void CreateBufferMemory(
    VkDeviceSize size, 
    VkBufferUsageFlags usage, 
    VkMemoryPropertyFlags properties,
    VkBuffer* pBuffer, ///< required
    VkDeviceMemory* pMemory, ///< optional
    VkDeviceSize* pSize ///< required
) 
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(ConfirmVkResult(vkCreateBuffer(device, &bufferCreateInfo, 0, pBuffer)) == 0) {
        SDL_Log("Failed Vulkan Buffer Creation");
        exit(EXIT_FAILURE);
    }

    VkMemoryRequirements memRequirements = {};
    vkGetBufferMemoryRequirements(device, *pBuffer, &memRequirements);
    *pSize = memRequirements.size;

    if(pMemory) {
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = get_memory_index(memRequirements.memoryTypeBits, properties);

        if(ConfirmVkResult(vkAllocateMemory(device, &allocInfo, 0, pMemory)) == 0) {
            SDL_Log("Failed Vulkan Memory Creation");
            exit(EXIT_FAILURE);
        }

        vkBindBufferMemory(device, *pBuffer, *pMemory, 0);
    }
}

static int32_t get_memory_index(uint32_t requested, VkMemoryPropertyFlags required) 
{
    const uint32_t memoryCount = properties_physicaldevice_memory.memoryTypeCount;
    for (uint32_t memoryIndex = 0; memoryIndex < memoryCount; ++memoryIndex) {
        const uint32_t memoryTypeBits = (1 << memoryIndex);
        const VkBool32 isRequiredMemoryType = requested & memoryTypeBits;

        const VkMemoryPropertyFlags properties =
            properties_physicaldevice_memory.memoryTypes[memoryIndex].propertyFlags;
        const VkBool32 hasrequired =
            (properties & required) == required;

        if (isRequiredMemoryType && hasrequired)
            return memoryIndex;
    }

    return 0;
}

void CreateImageMemory(
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling, 
    VkImageUsageFlags usage, 
    VkSampleCountFlagBits sampleCount,
    VkMemoryPropertyFlags properties,
    VkImageLayout imageLayout,
    VkImage* pImage, ///< required
    VkDeviceMemory* pMemory, ///< optional
    VkDeviceSize* pSize ///< required
) 
{
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = format;
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.initialLayout = imageLayout;
    imageCreateInfo.usage = usage;
    imageCreateInfo.samples = sampleCount;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(ConfirmVkResult(vkCreateImage(device, &imageCreateInfo, 0, pImage)) == 0) {
        SDL_Log("Failed Vulkan Image Creation");
        exit(EXIT_FAILURE);
    }

    VkMemoryRequirements memRequirements = {};
    vkGetImageMemoryRequirements(device, *pImage, &memRequirements);
    *pSize = memRequirements.size;

    if(pMemory) {
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = get_memory_index(memRequirements.memoryTypeBits, properties);

        if(ConfirmVkResult(vkAllocateMemory(device, &allocInfo, 0, pMemory)) == 0) {
            SDL_Log("Failed Vulkan Memory Creation");
            exit(EXIT_FAILURE);
        }

        vkBindImageMemory(device, *pImage, *pMemory, 0);
    }
}


VkImageView vx_create_imageview(
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspectFlags
) 
{
    VkImageViewCreateInfo imageViewInfo = {};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = format;
    imageViewInfo.subresourceRange.aspectMask = aspectFlags;
    imageViewInfo.subresourceRange.baseMipLevel = 0;
    imageViewInfo.subresourceRange.baseArrayLayer = 0;
    imageViewInfo.subresourceRange.layerCount = 1;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.image = image;

    VkImageView view = VK_NULL_HANDLE;
    
    if(ConfirmVkResult(vkCreateImageView(device, &imageViewInfo, 0, &view)) == 0) {
        SDL_Log("Failed Vulkan ImageView Creation");
        exit(EXIT_FAILURE);
    }

    return view;
}

VkSampler vx_create_sampler() 
{
    VkSamplerCreateInfo sampleCreateInfo = {};
    sampleCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampleCreateInfo.magFilter = VK_FILTER_LINEAR;
    sampleCreateInfo.minFilter = VK_FILTER_LINEAR;
    sampleCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampleCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampleCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampleCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampleCreateInfo.mipLodBias = 0.0f;
    sampleCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    sampleCreateInfo.minLod = 0.0f;
    sampleCreateInfo.maxAnisotropy = 1.0;
    sampleCreateInfo.anisotropyEnable = VK_FALSE;
    sampleCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    VkSampler sampler = VK_NULL_HANDLE;
    
    if( ConfirmVkResult(vkCreateSampler(device, &sampleCreateInfo, 0, &sampler)) == 0 ) {
        SDL_Log("Failed Vulkan Sampler Creation");
        exit(EXIT_FAILURE);
    }

    return sampler;
}

void vx_map_memory(
    VkDeviceMemory memory,
    VkDeviceSize size,
    void** pmapped
) 
{
    vkMapMemory(
        device,
        memory,
        0,
        size,
        0,
        pmapped
    );
}
*/

bool ConfirmVkResult(VkResult result) 
{
    if ( 
        result == VK_SUCCESS  ||
        result == VK_NOT_READY  ||
        result == VK_TIMEOUT  ||
        result == VK_EVENT_SET  ||
        result == VK_EVENT_RESET  ||
        result == VK_INCOMPLETE  ||
        result == VK_SUBOPTIMAL_KHR
    ) {
        return true;
    }
    return false;
}

bool ConfirmVkSuccess(VkResult result) 
{
    if ( 
        result == VK_SUCCESS
    ) {
        return true;
    }
    return false;
}

const char* GetVkResultString(VkResult result)
{
    switch(result)
    {
        case VK_SUCCESS: return "Success";
        case VK_TIMEOUT: return "Timeout";
        case VK_INCOMPLETE: return "Incomplete";
        case VK_NOT_READY: return "Not Ready";
        case VK_EVENT_SET: return "Event Set";
        case VK_EVENT_RESET: return "Event Reset";
        case VK_SUBOPTIMAL_KHR: return "Suboptimal KHR";
    }
    return "Unknown";
}

VkFence CreateFence(void) 
{
    VkFence fence = VK_NULL_HANDLE;
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    EXIT_VK_RESULT(vkCreateFence(device, &info, 0, &fence), "Failed to create Vulkan Fence");
    return fence;
}

VkSemaphore CreateSemaphore(void) 
{
    VkSemaphore semaphore = VK_NULL_HANDLE;
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    EXIT_VK_RESULT(vkCreateSemaphore(device, &info, 0, &semaphore), "Failed to create Vulkan Semaphore");
    return semaphore;
}


