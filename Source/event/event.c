//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "event.h"
#include "global.h"
#include "vulkan/vk.h"

static bool running = true;

static void UpdateEvent()
{
    SDL_Event event = {};

    while( SDL_PollEvent(&event) )
    {
        switch( event.type )
        {
            case SDL_QUIT: running = false; break;
        }
    }
}

static void UpdateLogic()
{
    //
}

static void UpdateVisual()
{
    DisplayVulkan();
}

void RunEventLoop()
{
    Uint64 ticks_delta;
    Uint64 ticks_current;

    Uint64 ticks_elapsed = 0;
    Uint64 ticks_previous = SDL_GetTicks64();
    
    running = true;

    while( running == true )
    {
        ticks_current = SDL_GetTicks64();
        ticks_delta = ticks_current - ticks_previous;
        ticks_previous = ticks_current;
        ticks_elapsed += ticks_delta;

        while( ticks_elapsed >= TICKS_STEP )
        {
            ticks_elapsed -= TICKS_STEP;
            UpdateLogic();
            UpdateVisual();
        }

        UpdateEvent();
    }
}










