#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static lua_State *L = NULL;

int clear(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   SDL_SetRenderDrawColor(renderer, r, g, b, 255);
   SDL_RenderClear(renderer);

   return 1;
}

int color(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   SDL_SetRenderDrawColor(renderer, r, g, b, 255);

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

   /* Center the message and scale it up */
   SDL_GetRenderOutputSize(renderer, &w, &h);
   SDL_SetRenderScale(renderer, scale, scale);
   x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(str)) / 2;
   y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   /* Draw the message */
   SDL_RenderDebugText(renderer, x, y, str);

   return 1;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

   /* Lua */
   L = luaL_newstate();
   luaL_openlibs(L);

   lua_pushnumber(L, 800);
   lua_setglobal(L, "width");

   lua_pushnumber(L, 600);
   lua_setglobal(L, "height");

   lua_pushcfunction(L, clear);
   lua_setglobal(L, "clear");

   lua_pushcfunction(L, text);
   lua_setglobal(L, "text");

   lua_pushcfunction(L, color);
   lua_setglobal(L, "color");

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }
   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
   if (event->type == SDL_EVENT_KEY_DOWN ||
         event->type == SDL_EVENT_QUIT) {
      return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
   }
   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
   // work with api here
   char *code = "clear(255, 255, 255)\ncolor(200, 150, 25)\ntext('Hello, MoonCon User!', 1)";

   if (luaL_dostring(L, code) == LUA_OK)
   {
      lua_pop(L, lua_gettop(L));
   }

   SDL_RenderPresent(renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   lua_close(L);
}
