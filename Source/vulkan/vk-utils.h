#ifndef VK_UTILS_H
#define VK_UTILS_H

#include <vulkan/vulkan.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#ifdef __cplusplus
extern "C" {
#endif


enum {
DEFAULT_SWAPCHAIN_SIZE  = 4, ///< number of swapchain images
};
#define DEFAULT_SURFACE_FORMAT VK_FORMAT_R8G8B8_UNORM
#define DEFAULT_SAMPLE_COUNT   VK_SAMPLE_COUNT_8_BIT
#define DEFAULT_PRESENT_MODE   VK_PRESENT_MODE_MAILBOX_KHR
#define VERTEX_SHADER_PATH   "./shader.vert.spv"
#define FRAGMENT_SHADER_PATH "./shader.frag.spv"

#define DEFINE_INSTANCE_EXTENSIONS(W, N, B, S, ...) const char* B[S] = {}; uint32_t N = 0; do { \
    SDL_Vulkan_GetInstanceExtensions(W, &N, 0); \
    N = (N < S) ? N : S; \
    SDL_bool result = SDL_Vulkan_GetInstanceExtensions(W, &N, B); \
    if(result == SDL_FALSE) \
    { \
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get Vulkan Instance Extensions"); \
        exit(EXIT_FAILURE); \
    } \
    const uint32_t num_args = sizeof((char*[]){__VA_ARGS__})/sizeof(char*); \
    if(num_args > 0) { \
        char** new_B = (char**)&B[N]; \
        char** array = (char*[]){__VA_ARGS__}; \
        memcpy(new_B, array, num_args * sizeof(char*)); \
        N += num_args; \
    } \
} while(0);

#define DEFINE_ENUMERATE(T, F, P, N, B, S) T B[S] = {}; uint32_t N = 0; do { \
    F(P, &N, 0); \
    N = (N < S) ? N : S; \
    F(P, &N, B); \
} while(0);

#define DEFINE_ENUMERATE_2(T, F, P, Q, N, B, S) T B[S] = {}; uint32_t N = 0; do { \
    F(P, Q, &N, 0); \
    N = (N < S) ? N : S; \
    F(P, Q, &N, B); \
} while(0);

#define RETURN_VK_RESULT( R, M, ... ) do{ \
    if(ConfirmVkSuccess(R) == false)\
    {SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", M); return __VA_ARGS__;} \
}while(0);

#define EXIT_VK_RESULT( R, M ) do{ \
    if(ConfirmVkSuccess(R) == false)\
    {SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", M); exit (EXIT_FAILURE);} \
}while(0);

VkSurfaceFormatKHR GetAvailableSurfaceFormat(VkFormat requested);
uint32_t GetPhysicalDeviceScore(VkPhysicalDeviceLimits limits);
VkDebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerInfo();
VkPresentModeKHR GetAvailablePresentMode(VkPresentModeKHR requested);
uint32_t GetAvailableSwapchainSize(uint32_t requested);
VkShaderModule CreateShaderModule(const char* path);
bool ConfirmVkSuccess(VkResult result);
bool ConfirmVkResult(VkResult result);
const char* GetVkResultString(VkResult result);
VkFence CreateFence(void);
VkSemaphore CreateSemaphore(void);

#ifdef __cplusplus
}
#endif

#endif //VK_UTILS_H