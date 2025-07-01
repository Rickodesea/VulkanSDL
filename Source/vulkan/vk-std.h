//Copyright (C) 2023 Alrick Grandison, Algodal™

#ifndef VK_STD_H
#define VK_STD_H

#include <vulkan/vulkan.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

void CreateInstance(char* name, uint32_t version);
void DestroyInstance();
void CreateDevice();
void DestroyDevice();
void CreateSurface();
void DestroySurface();
void CreateRenderPass();
void DestroyRenderPass();
void CreateSwapchain();
void DestroySwapchain();
void CreateCommandBuffers();
void DestroyCommandBuffers();
void CreatePipelineLayout();
void DestroyPipelineLayout();
void CreatePipeline();
uint32_t GetNextDrawImageIndex();
VkBool32 RecordCommands(uint32_t index_image);
void SubmitCommands(uint32_t index_image);

#ifdef __cplusplus
}
#endif

#endif //VK_STD_H










