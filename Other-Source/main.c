//Copyright (C) 2023 Alrick Grandison, Algodal™

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"
#include "app.h"

int main()
{
	printf("Military-Collide: Hello World!\n");
	SDL_version version = {};
	SDL_GetVersion(&version);
	printf("(lib) SDL2 version: %d.%d.%d\n", version.major, version.minor, version.patch);
	SDL_VERSION(&version);
	printf("(hdr) SDL2 version: %d.%d.%d\n", version.major, version.minor, version.patch);
	return Launch();
}










