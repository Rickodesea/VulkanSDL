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
//  filename: VulkanSDLMain.c                                                                                                           //
//  created: 2022-07-31 02:16 PM                                                                                                        //
//                                                                                                                                      //
//  description:                                                                                                                        //
//                                                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer_01.h"

static SDL_Window * WINDOW;
static vsr_Handle_t * RENDERER;

enum {
	WIDTH = 480,
	HEIGHT = 600
};

int main() {

	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Log("SDL Initialized Successfully");

	// Create an application window with the following settings:
	WINDOW = SDL_CreateWindow (
		"Vulkan SDL C - Renderer",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WIDTH,              // width, in pixels
		HEIGHT,             // height, in pixels
		SDL_WINDOW_VULKAN                  // flags
	);

	// Check that the window was successfully created
	if (WINDOW == 0) {
		// In the case that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	vsr_Param_t param = {};
	param.window = WINDOW;
	param.width = WIDTH;
	param.height = HEIGHT;
	param.requestedInstanceExtensionList[0] = "VK_EXT_debug_utils";
	param.validate.useDebug = 1;
	param.validate.useCreationAndDestruction = 1;
	param.requestedValidationLayerList[0] = "VK_LAYER_KHRONOS_validation";
	param.validate.severityFlag = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	param.validate.typeFlag = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	vsr_Result_t result = vsr_create(param);

	if ( result.OK == 0 ) {
		SDL_Log("Vulkan Renderer Created is not Usable!");
		vsr_destroy(result);
		return EXIT_FAILURE;
	}

	RENDERER = result.handle;
	int RUNNING = 1;
	SDL_Event event = {};

	// START THE GAME
	while ( RUNNING ) {
		while ( SDL_PollEvent( &event ) ) {  
			if ( event.type == SDL_QUIT ) {
				RUNNING = 0;
			}
		}
	}


	SDL_Log("***** Unallocating Memory *****");
	vsr_destroy(result);
	SDL_Log("Destroyed Vulkan Renderer");

	SDL_DestroyWindow ( WINDOW );
	SDL_Log("Destroyed Window");

	SDL_Quit();
	SDL_Log("Quit SDL");

	return EXIT_SUCCESS;
}











