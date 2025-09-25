#include "moon_context.h"
#include "lua.h"

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

MoonContext *MoonContextInit()
{
   MoonContext *ctx = (MoonContext *)SDL_calloc(1, sizeof(MoonContext));
   if (!ctx) {
      return NULL;
   }

   ctx->program = init_program;

   Lua_Init(ctx);
   
   return ctx;
}
