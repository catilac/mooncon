#ifndef MOON_CONTEXT_H
#define MOON_CONTEXT_H

#include <lua.h>
#include <SDL3/SDL.h>

typedef struct MoonContext {
   SDL_Window *window;
   SDL_Renderer *renderer;
   lua_State *L;
} MoonContext;


MoonContext *MoonContextInit();

#endif
