#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int mult(lua_State *L)
{
   int a = luaL_checkinteger(L, 1);
   int b = luaL_checkinteger(L, 2);
   lua_Integer c = a * b;

   lua_pushinteger(L, c);
   return 1;
}

int main(int argc, char **argv)
{

   lua_State *L = luaL_newstate();
   luaL_openlibs(L);

   lua_pushcfunction(L, mult);
   lua_setglobal(L, "mult");

   // work with api here
   char *code = "print('Hello, MoonCon User!')\nprint(mult(4, 2))";

   if (luaL_dostring(L, code) == LUA_OK)
   {
      lua_pop(L, lua_gettop(L));
   }

   lua_close(L);
   return 0;
}
