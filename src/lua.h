#ifndef ILUA_H
#define ILUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "moon_context.h"


int Lua_Init(MoonContext *ctx);
int Lua_Call(lua_State *L, const char *funcName);
MoonContext *Lua_GetCtx(lua_State *L);

#endif
