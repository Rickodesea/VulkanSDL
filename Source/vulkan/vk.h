//Copyright (C) 2023 Alrick Grandison, Algodal™

#ifndef VK_H
#define VK_H

#include <vulkan/vulkan.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#ifdef __cplusplus
extern "C" {
#endif

void CreateVulkan();
void DestroyVulkan();
void DisplayVulkan();

#ifdef __cplusplus
}
#endif

#endif //VK_H










