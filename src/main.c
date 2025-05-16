#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdint.h>
#include <stdlib.h>

#include "moonvm.h"
#include "moon_context.h"
#include "lua.h"
#include "editbox.h"

SDL_Color palette[16] = {
    {7, 3, 4, 255},       // #070304
    {50, 45, 76, 255},    // #322d4c
    {30, 58, 45, 255},    // #1e3a2d
    {61, 47, 91, 255},    // #3d2f5b
    {65, 48, 99, 255},    // #413063
    {59, 66, 62, 255},    // #3b423e
    {29, 73, 63, 255},    // #1c493f
    {81, 105, 109, 255},  // #51696d
    {89, 127, 86, 255},   // #597f56
    {145, 42, 100, 255},  // #912a64
    {201, 68, 110, 255},  // #c9446e
    {229, 73, 99, 255},   // #e54963
    {198, 73, 188, 255},  // #c649bc
    {120, 105, 234, 255}, // #7869ea
    {125, 198, 151, 255}, // #7dc697
    {218, 226, 234, 255}, // #dae2ea
};

// TODO: swap this out and use the VM
static SDL_Texture *displayTex = NULL;

static MoonVM *vm;

static EditBox *editor = NULL;
static TTF_Font *font = NULL;
static SDL_FRect editRect = {400, 0, 400, 600};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   MoonContext *ctx = MoonContextInit();
   if (!ctx)
      return SDL_APP_FAILURE;

   *appstate = ctx;

   vm = MoonVM_init();

   /* Window */
   if (!SDL_CreateWindowAndRenderer("MoonCon",
                                    800, 600, 0,
                                    &ctx->window, &ctx->renderer))
   {
      SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   // initialize display with random values
   for (int i=0;i<DISPLAY_SIZE;i++)
   {
      u8 high = (rand() % 16) << 4;
      u8 low = (rand() % 16);
      //moondisplay[i] = high | low;
      vm->mem[VRAM+i] = high | low;
   }

   displayTex = SDL_CreateTexture(
                  ctx->renderer, 
                  SDL_PIXELFORMAT_RGBA8888,
                  SDL_TEXTUREACCESS_STREAMING,
                  64, 64);
   SDL_SetTextureScaleMode(displayTex, SDL_SCALEMODE_NEAREST);

   TTF_Init();
   font = TTF_OpenFont("fonts/bubbletea.ttf", 24.0f);
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
   /* 
    *
    * RUN A HARDWARE CYCLE
    * * */
   // 1. get next instruction type stuff
   // 2. cpu run the instruction
   // 3. ppu stuff might be the code below:
   if (!vm->isHalt)
      MoonVM_fde_cycle(vm);


   // for (int i=0;i<DISPLAY_SIZE;i++)
   // {
   //    u8 high = (rand() % 16) << 4;
   //    u8 low = (rand() % 16);
   //    moondisplay[i] = high | low;
   // }

   Uint32 *pixels = NULL;
   int pitch;

   if (SDL_LockTexture(displayTex, NULL, (void **) &pixels, &pitch))
   {
      int pixelIdx = 0;
      for (int i = 0; i < DISPLAY_SIZE; i++)
      {
         u16 word = vm->mem[VRAM+i];
         u8 highByte = (word >> 4) & 0x0F;
         u8 lowByte = word & 0x0F;

         SDL_Color c1 = palette[highByte];
         SDL_Color c2 = palette[lowByte];

         pixels[pixelIdx++] = SDL_MapRGB(
               SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
               NULL, c1.r, c1.g, c1.b);

         pixels[pixelIdx++] = SDL_MapRGB(
               SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
               NULL, c2.r, c2.g, c2.b);
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
   free(vm);
   lua_close(ctx->L);
}
