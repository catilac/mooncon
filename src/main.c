#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdint.h>

#include "moon_context.h"
#include "lua.h"
#include "editbox.h"

#define DISPLAY_SIZE 1024

static uint8_t moondisplay[DISPLAY_SIZE];
static SDL_Texture *displayTex = NULL;

static EditBox *editor = NULL;
static TTF_Font *font = NULL;
static SDL_FRect editRect = {400, 0, 400, 600};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   MoonContext *ctx = MoonContextInit();
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon",
                                    800, 600, 0,
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   memset(moondisplay, 0, DISPLAY_SIZE * sizeof(uint8_t));
   displayTex = SDL_CreateTexture(
                  ctx->renderer, 
                  SDL_PIXELFORMAT_RGBA8888,
                  SDL_TEXTUREACCESS_STREAMING,
                  64, 64);

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

   // if (Lua_Call(ctx->L, "update") != 0) {
   //    SDL_Log("ERROR calling update\n");
   // }

   // if (Lua_Call(ctx->L, "draw") != 0)
   // {
   //    const char *error = lua_tostring(ctx->L, -1);
   //    SDL_Log("Error running draw: %s\n", error);
   //    lua_pop(ctx->L, 1); // remove error message
   // }

   // SDL_SetRenderDrawColor(ctx->renderer, 0xCC, 0xCC, 0xCC, 0xFF);
   // SDL_RenderFillRect(ctx->renderer, &editRect);
   // TTF_SetTextColor(editor->text, 0, 0, 0, 255);
   // EditBox_Draw(editor);

   Uint32 *pixels = NULL;
   int pitch;

   if (SDL_LockTexture(displayTex, NULL, (void **) &pixels, &pitch))
   {
      // TODO: get color out of memory
      // write out the pixel data from display memory
      for (int i = 0; i < (64 * 64); i++)
      {
         pixels[i] = SDL_MapRGB(
               SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
               NULL, 44, 25, 120);
      }
      SDL_UnlockTexture(displayTex);
   }

   SDL_RenderTexture(ctx->renderer, displayTex, NULL, NULL);

   SDL_RenderPresent(ctx->renderer);

   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   MoonContext *ctx = (MoonContext *)appstate;
   lua_close(ctx->L);
}
