//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "app.h"

int main()
{
	printf("Military-Collide: Hello World!\n");
	int version = 0;
	version = SDL_GetVersion();
	printf("(lib) SDL3 link version: %d\n", version);
	return Launch();
}

//There is a segment fault that needs to fix










