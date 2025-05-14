#include "moon_context.h"
#include "lua.h"

MoonContext *MoonContextInit()
{
   MoonContext *ctx = (MoonContext *)SDL_calloc(1, sizeof(MoonContext));
   if (!ctx) {
      return NULL;
   }

   Lua_Init(ctx);
   
   return ctx;
}
