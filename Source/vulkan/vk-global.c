//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vk-global.h"

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicaldevice;
uint32_t index_queuefamily;
VkPhysicalDeviceProperties properties_physicaldevice;
VkPhysicalDeviceMemoryProperties properties_physicaldevice_memory;
VkSurfaceCapabilitiesKHR capabilities_surface;
VkDevice device;
VkQueue queue;
VkRenderPass renderpass;
VkSwapchainKHR swapchain;
uint32_t size_swapchain;
VkImage* images_swapchain;
VkImageView* imageviews_swapchain;
VkFramebuffer* framebuffers_swapchain;
VkCommandPool commandpool;
VkCommandBuffer commandbuffers[2];
VkDescriptorSetLayout descriptorsetlayout;
VkPipelineLayout pipelinelayout;
VkPipeline pipeline;
VkFence fence;











