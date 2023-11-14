//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vk.h"
#include "vk-global.h"
#include "vk-std.h"
#include "vk-utils.h"
#include "global.h"

void CreateVulkan()
{
    CreateInstance(NAME, VK_MAKE_API_VERSION(0, 1, 0, 0));
    CreateSurface();
    CreateDevice();
    CreateRenderPass();
    CreateSwapchain();
    CreateCommandBuffers();
    CreatePipelineLayout();
    CreatePipeline();
    //CreateMemory();
    //CreateSignals();
    fence = CreateFence();
}

void DestroyVulkan()
{
    vkDeviceWaitIdle(device);

    vkDestroyFence(device, fence, 0);

    //DestroySignals();
    //DestroyMemory();
    vkDestroyPipeline(device, pipeline, 0);
    DestroyPipelineLayout();
    DestroyCommandBuffers();
    DestroySwapchain();
    DestroyRenderPass();
    DestroyDevice();
    DestroySurface();
    DestroyInstance();
}

void DisplayVulkan()
{
    uint32_t index = GetNextDrawImageIndex();
    RecordCommands(index);
    SubmitCommands(index);
    vkWaitForFences(device, 1, &fence, 1, UINT64_MAX);
    vkResetFences(device, 1, &fence);
}










