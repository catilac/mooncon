#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "moon_context.h"
#include "lua.h"
#include "editbox.h"

static EditBox *editor = NULL;
static TTF_Font *font = NULL;
static SDL_FRect editRect = {50, 50, 200, 150};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   MoonContext *ctx = MoonContextInit();
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon", 
                                    800, 600, SDL_WINDOW_FULLSCREEN,
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

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
   EditBox_Insert(editor, ctx->program);

   /* Call Init() */
   if (Lua_Call(ctx->L, "init") != 0) {
      SDL_Log("ERROR calling init\n");
      return SDL_APP_FAILURE;
   }

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
   MoonContext *ctx = (MoonContext *)appstate;
   SDL_ConvertEventToRenderCoordinates(ctx->renderer, event);

   switch (event->type)
   {
      case SDL_EVENT_KEY_DOWN:
         if (event->key.key == SDLK_R && (event->key.mod & SDL_KMOD_CTRL))
            Lua_Reload(ctx);
         else
            EditBox_HandleEvent(editor, event);
         break;
      case SDL_EVENT_QUIT:
         return SDL_APP_SUCCESS;
      default:
         EditBox_HandleEvent(editor, event);
         break;
   }

   ctx->program = editor->text->text;

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
   MoonContext *ctx = (MoonContext *)appstate;

   if (Lua_Call(ctx->L, "update") != 0) {
      SDL_Log("ERROR calling update\n");
   }

   if (Lua_Call(ctx->L, "draw") != 0)
   {
      const char *error = lua_tostring(ctx->L, -1);
      SDL_Log("Error running draw: %s\n", error);
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
