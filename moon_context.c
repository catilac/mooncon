#include "moon_context.h"
#include "ilua.h"

MoonContext *MoonContextInit()
{
   MoonContext *ctx = (MoonContext *)SDL_calloc(1, sizeof(MoonContext));
   if (!ctx) {
      return NULL;
   }

   iLuaInit(ctx);
   
   return ctx;
}
