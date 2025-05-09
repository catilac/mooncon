#include "ilua.h"
#include "moonlang.h"

static char *moon_context_key;

int iLuaInit(MoonContext *ctx)
{
   ctx->L = luaL_newstate();
   luaL_openlibs(ctx->L);

   lua_pushlightuserdata(ctx->L, (void *)&moon_context_key);
   lua_pushlightuserdata(ctx->L, ctx);
   lua_settable(ctx->L, LUA_REGISTRYINDEX);

   lua_pushnumber(ctx->L, 800);
   lua_setglobal(ctx->L, "width");

   lua_pushnumber(ctx->L, 600);
   lua_setglobal(ctx->L, "height");

   lua_pushcfunction(ctx->L, clear);
   lua_setglobal(ctx->L, "clear");

   lua_pushcfunction(ctx->L, text);
   lua_setglobal(ctx->L, "text");

   lua_pushcfunction(ctx->L, rect);
   lua_setglobal(ctx->L, "rect");

   lua_pushcfunction(ctx->L, color);
   lua_setglobal(ctx->L, "color");

   if (luaL_dofile(ctx->L, "./prog.lua") == LUA_OK)
   {
      printf("prog.lua: OK");
   } else { 
      luaL_error(ctx->L, "Error: %s\n", lua_tostring(ctx->L, -1));
   }

   return 0;
}

int iLuaCallFunc(lua_State *L, const char *funcName)
{
   lua_getglobal(L, funcName);
   return lua_pcall(L, 0, 0, 0);
}

MoonContext *getMoonContext(lua_State *L)
{
   lua_pushlightuserdata(L, (void *)&moon_context_key);
   lua_gettable(L, LUA_REGISTRYINDEX);
   return (MoonContext *)lua_touserdata(L, -1);
}

