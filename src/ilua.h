#ifndef ILUA_H
#define ILUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "moon_context.h"


int iLuaInit(MoonContext *ctx);
int iLuaCallFunc(lua_State *L, const char *funcName);
MoonContext *getMoonContext(lua_State *L);

#endif
