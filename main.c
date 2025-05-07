#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int mult(lua_State *L)
{
   int a = luaL_checkinteger(L, 1);
   int b = luaL_checkinteger(L, 2);
   lua_Integer c = a * b;

   lua_pushinteger(L, c);
   return 1;
}

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static lua_State *L = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

   /* Lua */
   L = luaL_newstate();
   luaL_openlibs(L);

   lua_pushcfunction(L, mult);
   lua_setglobal(L, "mult");


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
   char *code = "print('Hello, MoonCon User!')\nprint(mult(4, 2))";

   if (luaL_dostring(L, code) == LUA_OK)
   {
      lua_pop(L, lua_gettop(L));
   }

   const char *message = "Hello World!";
   int w = 0, h = 0;
   float x, y;
   const float scale = 4.0f;

   /* Center the message and scale it up */
   SDL_GetRenderOutputSize(renderer, &w, &h);
   SDL_SetRenderScale(renderer, scale, scale);
   x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
   y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   /* Draw the message */
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   SDL_RenderClear(renderer);
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   SDL_RenderDebugText(renderer, x, y, message);
   SDL_RenderPresent(renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   lua_close(L);
}
