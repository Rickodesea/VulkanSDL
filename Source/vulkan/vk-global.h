//Copyright (C) 2023 Alrick Grandison, Algodal™

#ifndef VK_GLOBAL_H
#define VK_GLOBAL_H

#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif


extern VkInstance instance;
extern VkSurfaceKHR surface;
extern VkPhysicalDevice physicaldevice;
extern uint32_t index_queuefamily;
extern VkPhysicalDeviceProperties properties_physicaldevice;
extern VkPhysicalDeviceMemoryProperties properties_physicaldevice_memory;
extern VkSurfaceCapabilitiesKHR capabilities_surface;
extern VkDevice device;
extern VkQueue queue;
extern VkRenderPass renderpass;
extern VkSwapchainKHR swapchain;
extern uint32_t size_swapchain;
extern VkImage* images_swapchain;
extern VkImageView* imageviews_swapchain;
extern VkFramebuffer* framebuffers_swapchain;
extern VkCommandPool commandpool;
extern VkCommandBuffer commandbuffers[2];
#define draw_commandbuffer commandbuffers[0]
#define copy_commandbuffer commandbuffers[1]
extern VkDescriptorSetLayout descriptorsetlayout;
extern VkPipelineLayout pipelinelayout;
extern VkPipeline pipeline;
extern VkFence fence;


#ifdef __cplusplus
}
#endif

#endif //VK_GLOBAL_H










