//Copyright (C) 2023 Alrick Grandison, Algodal™

#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "SDL2/SDL.h"

#define NAME "C SDL2 Vulkan Starter"
#define DESCRIPTION "90s Military RTS Game"

enum {
HEIGHT = 600u,
WIDTH = 12u * HEIGHT / 10u,
TICKS_STEP = 1000u / 60u
};


extern SDL_Window* window;


#ifdef __cplusplus
}
#endif

#endif //GLOBAL_H










