#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>

#include "moon_context.h"
#include "ilua.h"
#include "editbox.h"

static EditBox *editor = NULL;
static TTF_Font *font = NULL;
static SDL_Surface *window_surface = NULL;
static SDL_FRect editRect = {50, 50, 200, 150};

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

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   MoonContext *ctx = MoonContextInit();
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon", 
                                    800, 600, 0/*SDL_WINDOW_FULLSCREEN*/,
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   window_surface = SDL_GetWindowSurface(ctx->window);

   TTF_Init();
   font = TTF_OpenFont("fonts/Kobold.ttf", 24.0f);
   if (!font) {
      SDL_Log("Font: %s\n", SDL_GetError());
      return SDL_APP_FAILURE;
   }
   
   editor = EditBox_Create(ctx->window, ctx->renderer,
                           TTF_CreateRendererTextEngine(ctx->renderer),
                           font, &editRect);
   EditBox_SetFocus(editor, true);
   EditBox_Insert(editor, init_program);

   /* Call Init() */
   if (iLuaCallFunc(ctx->L, "init") != 0) {
      printf("ERROR calling init\n");
      return SDL_APP_FAILURE;
   }

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
   switch (event->type)
   {
      case SDL_EVENT_KEY_DOWN:
         EditBox_HandleEvent(editor, event);
         break;
      case SDL_EVENT_QUIT:
         return SDL_APP_SUCCESS;
      default:
         EditBox_HandleEvent(editor, event);
         break;
   }

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
   MoonContext *ctx = (MoonContext *)appstate;

   if (iLuaCallFunc(ctx->L, "update") != 0) {
      printf("ERROR calling update\n");
   }

   if (iLuaCallFunc(ctx->L, "draw") != 0)
   {
      const char *error = lua_tostring(ctx->L, -1);
      fprintf(stderr, "Error running draw: %s\n", error);
      lua_pop(ctx->L, 1); // remove error message
   }

   SDL_RenderRect(ctx->renderer, &editRect);
   TTF_SetTextColor(editor->text, 0, 0, 0, 255);
   EditBox_Draw(editor);

   SDL_RenderPresent(ctx->renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   MoonContext *ctx = (MoonContext *)appstate;
   lua_close(ctx->L);
}
