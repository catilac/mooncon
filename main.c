#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static const char moon_context_key;

typedef struct MoonContext {
   SDL_Window *window;
   SDL_Renderer *renderer;
   lua_State *L;
} MoonContext;

MoonContext *getMoonContext(lua_State *L)
{
   lua_pushlightuserdata(L, (void *)&moon_context_key);
   lua_gettable(L, LUA_REGISTRYINDEX);
   return lua_touserdata(L, -1);
}

int clear(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   MoonContext *ctx = getMoonContext(L);

   SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 255);
   SDL_RenderClear(ctx->renderer);

   return 1;
}

int color(lua_State *L)
{
   uint8_t r = luaL_checkinteger(L, 1);
   uint8_t g = luaL_checkinteger(L, 2);
   uint8_t b = luaL_checkinteger(L, 3);

   MoonContext *ctx = getMoonContext(L);

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
   MoonContext *ctx = getMoonContext(L);

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

   MoonContext *ctx = getMoonContext(L);

   /* Center the message and scale it up */
   SDL_GetRenderOutputSize(ctx->renderer, &w, &h);
   SDL_SetRenderScale(ctx->renderer, scale, scale);
   x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(str)) / 2;
   y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   /* Draw the message */
   SDL_RenderDebugText(ctx->renderer, x, y, str);

   return 1;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

   MoonContext *ctx = (MoonContext *)SDL_calloc(1, sizeof(MoonContext));
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   /* Lua */
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

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon", 
                                    800, 600, SDL_WINDOW_FULLSCREEN, 
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   /* Call Init() */
   lua_getglobal(ctx->L, "init");
   if (lua_pcall(ctx->L, 0, 0, 0) != 0) {
      printf("ERROR calling init\n");
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
   MoonContext *ctx = (MoonContext *)appstate;

   lua_getglobal(ctx->L, "update");
   if (lua_pcall(ctx->L, 0, 0, 0) != 0) {
      printf("ERROR calling update\n");
   }
   lua_getglobal(ctx->L, "draw");
   if (lua_pcall(ctx->L, 0, 0, 0) != 0) {
      const char *error = lua_tostring(ctx->L, -1);
      fprintf(stderr, "Error running draw: %s\n", error);
      lua_pop(ctx->L, 1); // remove error message
   }

   SDL_RenderPresent(ctx->renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   MoonContext *ctx = (MoonContext *)appstate;
   lua_close(ctx->L);
}
