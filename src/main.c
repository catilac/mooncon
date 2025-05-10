#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "moon_context.h"
#include "ilua.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   MoonContext *ctx = MoonContextInit();
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon", 
                                    800, 600, SDL_WINDOW_FULLSCREEN, 
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   /* Call Init() */
   if (iLuaCallFunc(ctx->L, "init") != 0) {
      printf("ERROR calling init\n");
      return SDL_APP_FAILURE;
   }

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
   if (event->type == SDL_EVENT_KEY_DOWN ||
       event->type == SDL_EVENT_QUIT) 
   {
      return SDL_APP_SUCCESS;
   }
   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
   MoonContext *ctx = (MoonContext *)appstate;

   if (iLuaCallFunc(ctx->L, "update") != 0) {
      printf("ERROR calling update\n");
   }

   if (iLuaCallFunc(ctx->L, "draw") != 0)
   {
      const char *error = lua_tostring(ctx->L, -1);
      fprintf(stderr, "Error running draw: %s\n", error);
      lua_pop(ctx->L, 1); // remove error message
   }

   SDL_RenderPresent(ctx->renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   MoonContext *ctx = (MoonContext *)appstate;
   lua_close(ctx->L);
}
