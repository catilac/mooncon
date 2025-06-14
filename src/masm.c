// masm.c

#include "masm.h"

void add(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->reg[dest] = vm->reg[hn] + vm->reg[ln];
}

void sub(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->reg[dest] = vm->reg[hn] - vm->reg[ln];
}

void indx(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->I = (dest << 12) | (hn << 8) | ln;
}

void ld(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->reg[dest] = (hn << 4) | ln;
}

void st(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->mem[vm->I + dest] = (hn << 4) | ln;
}

void cls(MoonVM *vm)
{
   for (int i = 0; i < DISPLAY_SIZE; i++)
      vm->mem[VRAM+i] = 0;
};

void spx(MoonVM *vm, u8 col, u8 xreg, u8 yreg)
{
   u8  x = vm->reg[xreg];
   u8  y = vm->reg[yreg];
   u16 offset = (x + y * 64)/2;
   u16 byte = vm->mem[VRAM+offset];

   if (x % 2 == 0)
      byte |= (col << 8) & 0xF0; // hb
   else
      byte |= col & 0x0F; // lb
   vm->mem[VRAM + offset] = byte;
}

void cmp(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   if (vm->reg[hn] == vm->reg[ln])
      vm->reg[0xF] = 8; // EQ
   else
      vm->reg[0xF] = 4; // NEQ
}

void jmp(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   // TODO: currently can only jmp 0x00 to 0xFF
   // Need to think through using I register
   u8 address = (hn << 4 & 0xF0) | ln;
   switch (dest)
   {
      case 0: // JMP
         vm->pc = address;
         break;
      case 1: // JEQ
         if (vm->reg[0xF] == 8)
            vm->pc = address;
         break;
      case 2: // JNE
         if (vm->reg[0xF] == 4)
            vm->pc = address;
         break;
      default:
         vm->pc = address;
         break;
   }
}
