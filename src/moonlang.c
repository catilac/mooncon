#include "moonlang.h"
#include "lua.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <SDL3/SDL.h>

int clear(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   MoonContext *ctx = Lua_GetCtx(L);

   SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 255);
   SDL_RenderClear(ctx->renderer);

   return 1;
}

int color(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   MoonContext *ctx = Lua_GetCtx(L);

   SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 255);

   return 1;
}

int rect(lua_State *L)
{
   float xpos = lua_tonumber(L, 1);
   float ypos = lua_tonumber(L, 2);
   float size = lua_tonumber(L, 3);

   SDL_FRect r;
   r.x = xpos;
   r.y = ypos;
   r.w = r.h = size * 4.0f; // TODO: Figure out global scale for our coordinate system
                            //
   MoonContext *ctx = Lua_GetCtx(L);

   SDL_RenderFillRect(ctx->renderer, &r);

   return 1;
}

int text(lua_State *L)
{
   const char *str = luaL_checkstring(L, 1);
   if (!str)
   {
      return -1;
   }

   int w = 0, h = 0;
   float x, y;
   const float scale = 4.0f;

   MoonContext *ctx = Lua_GetCtx(L);

   /* Center the message and scale it up */
   SDL_GetRenderOutputSize(ctx->renderer, &w, &h);
   //SDL_SetRenderScale(ctx->renderer, scale, scale);
   x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(str)) / 2;
   y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   /* Draw the message */
   SDL_RenderDebugText(ctx->renderer, x, y, str);

   return 1;
}
