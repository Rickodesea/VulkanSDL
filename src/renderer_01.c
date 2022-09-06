//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2022 Alrick Grandison                                                                                                 //
//                                                                                                                                      //
//  This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable             //
//  for any damages arising from the use of this software. Permission is granted to anyone to use this software for any                 //
//  purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:      //
//                                                                                                                                      //
//  1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.                 //
//     If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.  //
//  2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.           //
//  3. This notice may not be removed or altered from any source distribution.                                                          //
//                                                                                                                                      //
//                                                                                                                                      //
//  filename: renderer_01.c                                                                                                       //
//  created: 2022-07-31 02:17 PM                                                                                                        //
//                                                                                                                                      //
//  description: Simple Vulkan Renderer C code for re-use                                                                               //
//                                                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer_01.h"

/*********
   1. Initialize the API
      1. Create a Vulkan Instance to access inner functions of the Vulkan API. 
      2. Pick the best Physical Device from every device that supports Vulkan on your machine. 
      3. Create a Logical Device, Surface, Queue, Command Pool, Semaphores, Fences.
   2. Create Commands - Describe everything that'll be rendered on the current frame 
      in your command buffers.
   3. Initialize Resources
      1. Create a Descriptor Pool, Descriptor Set Layout, Pipeline Layout, Vertex Buffer/Index Buffer 
      2. and send it to GPU Accessible Memory, 
      3. describe our Input Attributes, 
      4. create a Uniform Buffer, Render Pass, Frame Buffers, Shader Modules, and Pipeline State.
   4. Setup Commands for each command buffer to set the GPU state to render the triangles
   5. Render - Use an Update Loop to switch between different frames in your swapchain as well as 
      to poll input devices/window events.
   6. Destroy any data structures once the application is asked to close.
 ********/

struct vsr_TYPE_Internal {
	const char * instanceExtensionList[vsr_MAX_APPLIED_INSTANCE_EXTENSION];
	uint32_t instanceExtensionCount;
	const char * validationLayerList[vsr_MAX_REQUESTED_VALIDATION_LAYER];
	uint32_t validationLayerCount;
};

static void allocate_result_initials(vsr_Result_t * result);
static VkApplicationInfo getApplicationInfo(vsr_Param_t param);
static void create_handle(vsr_Result_t * result, vsr_Param_t param);
static uint32_t getNullTerminatedStringArrayCount(const char * arr[], unsigned int max);
static int map_InstanceExtension(vsr_Result_t * result, vsr_Param_t param);
static VkDebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCreateInfo(vsr_Param_t param);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData, void * pUserData);
static int map_ValidationLayer(vsr_Result_t * result, vsr_Param_t param);
static void create_debug(vsr_Result_t * result, vsr_Param_t param);
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

#define GET_MIN(x,y) (((x) <= (y)) ? (x) : (y))

vsr_Result_t vsr_create( vsr_Param_t param ) {

	// Initialization

	vsr_Result_t result = {};
	allocate_result_initials(&result);

	// Process

	create_handle(&result, param);
	create_debug(&result, param);
	
	return result;
}

void vsr_destroy(vsr_Result_t result) {
	DestroyDebugUtilsMessengerEXT(result.handle->instance, result.handle->debug, 0);
	vkDestroyInstance(result.handle->instance, 0);
	free(result.handle);
	free(result.internal);
}

static void allocate_result_initials(vsr_Result_t * result) {

	// allocate handle
	result->handle = malloc(sizeof(vsr_Handle_t));
	memset(result->handle, 0, sizeof(vsr_Handle_t));

	// allocate internal
	result->internal = malloc(sizeof(vsr_Internal_t));
	memset(result->internal, 0, sizeof(vsr_Internal_t));

}

static void create_handle(vsr_Result_t * result, vsr_Param_t param) {
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VkApplicationInfo applicationInfo = getApplicationInfo(param);
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	if(param.validate.useDebug) {
		if(param.validate.useCreationAndDestruction) {
			VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo = getDebugUtilsMessengerCreateInfo(param);
			instanceCreateInfo.pNext = &DebugUtilsMessengerCreateInfo;
		}

		map_ValidationLayer(result, param);

		instanceCreateInfo.enabledLayerCount = result->internal->validationLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = result->internal->validationLayerList;
	}

	if(map_InstanceExtension(result, param) == 0) return;

	instanceCreateInfo.enabledExtensionCount = result->internal->instanceExtensionCount;
	instanceCreateInfo.ppEnabledExtensionNames = result->internal->instanceExtensionList;

	if (vkCreateInstance(&instanceCreateInfo, 0, &result->handle->instance) != VK_SUCCESS) {

	    SDL_Log("Failed to Create Vulkan Instance");
	    return;

	}

	result->OK = 1; // Renderer is Usable!
}

static VkApplicationInfo getApplicationInfo(vsr_Param_t param) {
	VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = param.applicationName ? param.applicationName : "Vulkan SDL Renderer App";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(param.vulkanVersion.major ? param.vulkanVersion.major : 1, param.vulkanVersion.minor, param.vulkanVersion.patch);
    applicationInfo.pEngineName = param.engineName ? param.engineName : "Vulkan SDL Renderer C Engine";
    applicationInfo.engineVersion =VK_MAKE_VERSION(param.sdlVersion.major > 1 ? param.sdlVersion.major : 2, param.sdlVersion.minor, param.sdlVersion.patch); // SDL version
    applicationInfo.apiVersion = VK_API_VERSION_1_0;
    return applicationInfo;
}

static uint32_t getNullTerminatedStringArrayCount(const char * arr[], unsigned int max) {
	for (unsigned int i = 0; i < max; i++) {
		if(arr[i] == 0) return i;
	}
	return max;
}

static int map_InstanceExtension(vsr_Result_t * result, vsr_Param_t param) {
	// Get All available Vulkan Extensions on this system

	SDL_Log("Vulkan: Searching for Available Instance Extensions for this System ....");

	uint32_t availableInstanceExtensionCount = 0;

	vkEnumerateInstanceExtensionProperties( 0, &availableInstanceExtensionCount, 0 );

	if(availableInstanceExtensionCount == 0) {

		SDL_Log("Vulkan: No Instance Extension found!");
		return 0;

	}

	VkExtensionProperties * availableInstanceExtensionList = malloc(sizeof(VkExtensionProperties) * availableInstanceExtensionCount);

	vkEnumerateInstanceExtensionProperties( 0, &availableInstanceExtensionCount, availableInstanceExtensionList );

	for(uint32_t i = 0; i < availableInstanceExtensionCount; i++) {

		SDL_Log( "Vulkan: %s Instance Extension is AVAILABLE!", availableInstanceExtensionList[i].extensionName );

	}

	SDL_Log("SDL: Searching for Required Vulkan Instance Extensions ....");

	unsigned int SDLRequiredInstanceExtensionCount;

	if(SDL_Vulkan_GetInstanceExtensions(param.window, &SDLRequiredInstanceExtensionCount, 0) != SDL_TRUE) {

		SDL_Log("SDL: SDL_Vulkan_GetInstanceExtensions returned false!");
		return 0;

	}

	if(SDLRequiredInstanceExtensionCount == 0) {

		SDL_Log("SDL: No Required Instance Extension found!");
		return 0;
	}

	const char ** SDLRequiredExtensionList = malloc(sizeof(char *) * SDLRequiredInstanceExtensionCount);

	SDL_Vulkan_GetInstanceExtensions(param.window, &SDLRequiredInstanceExtensionCount, SDLRequiredExtensionList);

	for(unsigned int i = 0; i < SDLRequiredInstanceExtensionCount; i++) {

		SDL_Log("SDL: %s is REQUIRED!", SDLRequiredExtensionList[i]);

	}

	int state_all, state_unit;

	SDL_Log("Vulkan: Mapping Instance Available Extensions and Required Extensions ....");

	state_all = 1;

	for(unsigned int i = 0; i < SDLRequiredInstanceExtensionCount; i++) {
		
		state_unit = 0;
		
		for(uint32_t j = 0; j < availableInstanceExtensionCount; j++) {

			if(strcmp(SDLRequiredExtensionList[i], availableInstanceExtensionList[j].extensionName) == 0) {

				SDL_Log("Vulkan: MAPPED %s!", SDLRequiredExtensionList[i]);
				state_unit = 1; break;
			}
		}

		switch(state_unit){ case 1: continue; }

		state_all = 0;

		SDL_Log("Vulkan: MISSED %s!", SDLRequiredExtensionList[i]);
	}

	if(state_all == 0) {

		if(SDLRequiredExtensionList) free(SDLRequiredExtensionList);
		if(availableInstanceExtensionList) free(availableInstanceExtensionList);

		return 0;

	}

	// Check if additional requested extensions are available

	uint32_t requestedInstanceExtensionCount = getNullTerminatedStringArrayCount(param.requestedInstanceExtensionList, vsr_MAX_REQUESTED_INSTANCE_EXTENSION);

	if(requestedInstanceExtensionCount) {

		SDL_Log("Vulkan: Instance Extensions Requested by App ....");

		for(unsigned int i = 0; i < requestedInstanceExtensionCount; i++) {

			SDL_Log("Vulkan: %s is REQUESTED by App", param.requestedInstanceExtensionList[i]);

		}

		SDL_Log("Vulkan: Mapping Instance Available Extensions and Requested Extensions");

		state_all = 1;

		for(unsigned int i = 0; i < requestedInstanceExtensionCount; i++) {

			state_unit = 0;

			for(uint32_t j = 0; j < availableInstanceExtensionCount; j++) {

				if(strcmp(param.requestedInstanceExtensionList[i], availableInstanceExtensionList[j].extensionName) == 0) {

					SDL_Log("Vulkan: MAPPED %s!", param.requestedInstanceExtensionList[i]);
					state_unit = 1; break;

				}
			}
			switch(state_unit){ case 1: continue; }

			state_all = 0;
			SDL_Log("Vulkan: MISSED %s!", param.requestedInstanceExtensionList[i]);
		}

		if(state_all == 0) {
			if(SDLRequiredExtensionList) free(SDLRequiredExtensionList);
			if(availableInstanceExtensionList) free(availableInstanceExtensionList);

			return 0;
		}
	}

	result->internal->instanceExtensionCount = GET_MIN(SDLRequiredInstanceExtensionCount + requestedInstanceExtensionCount, vsr_MAX_APPLIED_INSTANCE_EXTENSION);
	
	memcpy(result->internal->instanceExtensionList, SDLRequiredExtensionList, sizeof(char *) * SDLRequiredInstanceExtensionCount);

	if(requestedInstanceExtensionCount) {
		const uint32_t remCount = GET_MIN(requestedInstanceExtensionCount, vsr_MAX_APPLIED_INSTANCE_EXTENSION - SDLRequiredInstanceExtensionCount);
		memcpy(&result->internal->instanceExtensionList[SDLRequiredInstanceExtensionCount], param.requestedInstanceExtensionList, sizeof(char *) * remCount);
	}

	free(SDLRequiredExtensionList);
	free(availableInstanceExtensionList);

	return 1;
}

static int map_ValidationLayer(vsr_Result_t * result, vsr_Param_t param) {
	// Get All available Vulkan Extensions on this system

	SDL_Log("Vulkan: Searching for Available Validation Layers for this System ....");

	uint32_t availableValidationLayerCount = 0;

	vkEnumerateInstanceLayerProperties( &availableValidationLayerCount, 0 );

	if(availableValidationLayerCount == 0) {

		SDL_Log("Vulkan: No Validation Layer found!");
		return 1;

	}

	VkLayerProperties * availableValidationLayerList = malloc(sizeof(VkLayerProperties) * availableValidationLayerCount);

	vkEnumerateInstanceLayerProperties( &availableValidationLayerCount, availableValidationLayerList );

	for(uint32_t i = 0; i < availableValidationLayerCount; i++) {

		SDL_Log( "Vulkan: %s Validation Layer is AVAILABLE!", availableValidationLayerList[i].layerName );

	}

	// Check if additional requested extensions are available

	uint32_t requestedValidationLayerCount = getNullTerminatedStringArrayCount(param.requestedValidationLayerList, vsr_MAX_REQUESTED_VALIDATION_LAYER);

	int state_all, state_unit;
	
	if(requestedValidationLayerCount) {

		SDL_Log("Vulkan: Validation Layer Requested by App ....");

		for(unsigned int i = 0; i < requestedValidationLayerCount; i++) {

			SDL_Log("Vulkan: %s is REQUESTED by App", param.requestedValidationLayerList[i]);

		}

		SDL_Log("Vulkan: Mapping Validation Available Layer and Requested Layer");

		state_all = 1;

		for(unsigned int i = 0; i < requestedValidationLayerCount; i++) {

			state_unit = 0;

			for(uint32_t j = 0; j < availableValidationLayerCount; j++) {

				if(strcmp(param.requestedValidationLayerList[i], availableValidationLayerList[j].layerName) == 0) {

					SDL_Log("Vulkan: MAPPED %s!", param.requestedValidationLayerList[i]);
					state_unit = 1; break;

				}
			}
			switch(state_unit){ case 1: continue; }

			state_all = 0;
			SDL_Log("Vulkan: MISSED %s!", param.requestedValidationLayerList[i]);
		}
	}

	if(requestedValidationLayerCount && state_all) {
		result->internal->validationLayerCount = requestedValidationLayerCount;
		memcpy(&result->internal->validationLayerList, param.requestedValidationLayerList, sizeof(char *) * requestedValidationLayerCount);
	}

	free(availableValidationLayerList);

	return 1;
}

static VkDebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCreateInfo(vsr_Param_t param) {
	const int severity = param.validate.severityFlag ?  param.validate.severityFlag : VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	const int type = param.validate.typeFlag ? param.validate.typeFlag : VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	
	VkDebugUtilsMessengerCreateInfoEXT info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.messageSeverity = severity;
    info.messageType = type;
    info.pfnUserCallback = debugCallback;
    
    return info;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData, void * pUserData) {
    fprintf(stderr, "validation layer: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

static void create_debug(vsr_Result_t * result, vsr_Param_t param) {
	if(param.validate.useDebug) {
		VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo = getDebugUtilsMessengerCreateInfo(param);

        if (CreateDebugUtilsMessengerEXT(result->handle->instance, &DebugUtilsMessengerCreateInfo, 0, &result->handle->debug) != VK_SUCCESS) {
            fprintf(stderr, "Vulkan: Failed to set up Debug Util Messenger!");
        }
	}
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != 0) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != 0) {
        func(instance, debugMessenger, pAllocator);
    }
}









