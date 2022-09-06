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
//  filename: renderer_01.h                                                                                                       //
//  created: 2022-07-31 02:17 PM                                                                                                        //
//                                                                                                                                      //
//  description: Simple Vulkan Renderer C code for re-use                                                                               //
//                                                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ___H_VULKANSDLRENDERER___
#define ___H_VULKANSDLRENDERER___

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif


#define vsr_MAX_REQUESTED_INSTANCE_EXTENSION 16
#define vsr_MAX_REQUESTED_VALIDATION_LAYER 16
#define vsr_MAX_APPLIED_INSTANCE_EXTENSION (vsr_MAX_REQUESTED_INSTANCE_EXTENSION * 2)


typedef struct vsr_TYPE_Result vsr_Result_t;
typedef struct vsr_TYPE_Param vsr_Param_t;
typedef struct vsr_TYPE_Handle vsr_Handle_t;
typedef struct vsr_TYPE_Internal vsr_Internal_t;

struct vsr_TYPE_Result {
	int OK; /// whether or not the renderer is useable
	vsr_Handle_t * handle; /// handle to the created renderer
	vsr_Internal_t * internal; /// internal use only
};

struct vsr_TYPE_Param {
	SDL_Window * window;  /// SDL Window Object

	unsigned int width; /// renderer width
	unsigned int height; /// renderer height

	const char * applicationName;
	const char * engineName;

	const char * requestedInstanceExtensionList[vsr_MAX_REQUESTED_INSTANCE_EXTENSION];
	const char * requestedValidationLayerList[vsr_MAX_REQUESTED_VALIDATION_LAYER];

	struct Version_s {
		char major;
		char minor;
		char patch;
	} vulkanVersion, sdlVersion;

	struct {
		int useDebug;
		int useCreationAndDestruction; //whether or not to use this validation
		int severityFlag;
		int typeFlag;
	} validate;
};

struct vsr_TYPE_Handle {
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug;
};

vsr_Result_t vsr_create( vsr_Param_t param );
void vsr_destroy( vsr_Result_t result );


#ifdef __cplusplus
}
#endif

#endif //___H_VULKANSDLRENDERER___










