#include "lua.h"
#include "moonlang.h"

static char *moon_context_key;

// TODO: put this somewhere out of the way, maybe just read
// from `prog.lua` or something.
const char *init_program = "\
function init()\n \
\tsize = 15\n \
\tt = 0\n \
end\n \
\n \
function update()\n \
\tt = t + 0.01\n \
\tsize = 15 * math.sin(t);\n \
end\n \
\n \
function draw()\n \
\tclear(55, 55, 225)\n \
\tcolor(200, 150, 25)\n \
\ttext('Hello, MoonCon User!', 1)\n \
\trect(40, 40, size)\n \
\n \
\tcolor(40, 240, 120)\n \
\trect(50 + math.cos(t*5) * 15, 50 + math.sin(t*8) * 24, 10);\n \
end";


int Lua_Init(MoonContext *ctx)
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

   if (luaL_dostring(ctx->L, init_program) == LUA_OK)
   {
      printf("prog.lua: OK");
   } else { 
      luaL_error(ctx->L, "Error: %s\n", lua_tostring(ctx->L, -1));
   }

   return 0;
}

int Lua_Call(lua_State *L, const char *funcName)
{
   lua_getglobal(L, funcName);
   return lua_pcall(L, 0, 0, 0);
}

MoonContext *Lua_GetCtx(lua_State *L)
{
   lua_pushlightuserdata(L, (void *)&moon_context_key);
   lua_gettable(L, LUA_REGISTRYINDEX);
   return (MoonContext *)lua_touserdata(L, -1);
}

