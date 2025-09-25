#include "moonvm.h"
#include "masm.h"
#include "masmc.h"

#include <stdio.h>

u16 fetch(MoonVM *);
Instruction decode(u16);
void execute(MoonVM *, Instruction);

MoonVM *MoonVM_init()
{
   MoonVM *vm = (MoonVM *)malloc(sizeof(MoonVM));
   vm->sp = 0xFF; /* Bottom of stack. Grows down */
   vm->isHalt = false;
   vm->pc = 0; 
   return vm;
}

void MoonVM_fde_cycle(MoonVM *vm)
{
   // Fetch - get instruction from memory at current PC
   u16 word = fetch(vm);
   // Decode - decode instruction
   Instruction i = decode(word);
   // Execute - execute the decoded instruction
   execute(vm, i);
}

void MoonVM_compile(MoonVM *vm, const char *input)
{
   vm->isHalt = true;
   compile(vm, input);
   vm->pc = 0;
   vm->isHalt = false;
}

void MoonVM_dump(MoonVM *vm)
{
   printf("REGISTERS: \n");
   for (int i = 0; i < 16; i++)
      printf("R[%d] = %d\n", i, vm->reg[i]);
   printf("DISPLAY SEGMENT: \n");
   for (int i = 0; i < 2048; i++)
      printf("%02x ", vm->mem[VRAM+i]);
}

u16 fetch(MoonVM *vm)
{
   return vm->mem[vm->pc++];
}

Instruction decode(u16 bytecode)
{
   Instruction i = {};
   // 0000 0000 0000 0000
   //   OP DDDD r1r1 r2r2
   i.op = (bytecode >> 12) & 0xF;
   i.dest = (bytecode >> 8) & 0xF;
   i.hn = (bytecode >> 4) & 0xF;
   i.ln = bytecode & 0xF;

   return i;
}

void execute(MoonVM *vm, Instruction i)
{
   // TODO: Is there a way to not do this
   switch (i.op)
   {
      case NOP:
         break;
      case ADD:
         add(vm, i.dest, i.hn, i.ln); 
         break;
      case SUB:
         sub(vm, i.dest, i.hn, i.ln);
         break;
      case INDX:
         indx(vm, i.dest, i.hn, i.ln);
      case LD:
         ld(vm, i.dest, i.hn, i.ln);
         break;
      case ST:
         st(vm, i.dest, i.hn, i.ln);
         break;
      case SPX:
         spx(vm, i.dest, i.hn, i.ln);
         break;
      case CLS:
         cls(vm);
         break;
      case CMP:
         cmp(vm, i.dest, i.hn, i.ln);
         break;
      case JMP:
         jmp(vm, i.dest, i.hn, i.ln);
         break;
      case CALL:
         call(vm, i.dest, i.hn, i.ln);
         break;
      case RET:
         ret(vm);
         break;
      case RES_0:
      case RES_1:
      case RES_2:
      case HALT:
         vm->isHalt = true;
         printf("\n\n=========\n\t\tHALT\t\t\n==========\n\n");
         MoonVM_dump(vm);
         break;
   }
   return;
}
