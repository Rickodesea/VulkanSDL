//Copyright (C) 2023 Alrick Grandison, Algodal™

//standard / installed libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//3rd-party project libraries
#include "SDL2/SDL.h"

//project source
#include "app.h"
#include "global.h"
#include "event/event.h"
#include "vulkan/vk.h"

static void CreateApp()
{
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL");
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(
        NAME " - " DESCRIPTION,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
    );

    if( window == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Create Window");
        exit(EXIT_FAILURE);
    }
}

static void DestroyApp()
{
    SDL_DestroyWindow(window);
}

int Launch()
{
    
    //Create Features
    CreateApp();
    CreateVulkan();

    //Run Main Loop
    RunEventLoop();

    //Destroy Features
    DestroyVulkan();
    DestroyApp();
    

    return EXIT_SUCCESS;
}









