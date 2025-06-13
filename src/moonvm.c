#include "moonvm.h"
#include "masm.h"
#include <stdio.h>

u16 fetch(MoonVM *);
Instruction decode(u16);
void execute(MoonVM *, Instruction);

MoonVM *MoonVM_init()
{
   MoonVM *vm = (MoonVM *)malloc(sizeof(MoonVM));

   vm->isHalt = false;
   // any other initialization goes here
   // add a program here
   vm->mem[0] = 0x4000; // cls ; clear screen
   vm->mem[1] = 0x2400; // ld r4, 0x00
   vm->mem[2] = 0x2100; // ld r1, 0x00
   vm->mem[3] = 0x2701; // ld r7, 0x01
   vm->mem[4] = 0x1447; // add r4, r4, r7
   vm->mem[5] = 0x1117; // add r1, r1, r7
   vm->mem[6] = 0x5A41; // spx 0x5, 0x4 0x1
   vm->mem[7] = 0x6004; // JMP 4
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
   Op op;
   switch ( (bytecode >> 12) & 0xF )
   {
      case 0x00:
         op = NOP;
         break;
      case 0x01:
         op = ADD;
         break;
      case 0x02:
         op = LD;
         break;
      case 0x03:
         op = ST;
         break;
      case 0x04:
         op = CLS;
         break;
      case 0x05:
         op = SPX;
         break;
      case 0x06:
         op = JMP;
         break;
      case 0x0F:
         op = HALT;
         break;
   }

   // TODO: i think the function itself should just look
   // at the bits in the instruction how it wants
   i.op = op;
   i.dest = (bytecode >> 8) & 0xF;
   i.hn = (bytecode >> 4) & 0xF;
   i.ln = bytecode & 0xF;

   return i;
}

void execute(MoonVM *vm, Instruction i)
{
   switch (i.op)
   {
      case NOP:
         break;
      case ADD:
         add(vm, i.dest, i.hn, i.ln); 
         break;
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
      case JMP:
         jmp(vm, i.dest, i.hn, i.ln);
         break;
      case HALT:
         vm->isHalt = true;
         printf("\n\n=========\n\t\tHALT\t\t\n==========\n\n");
         MoonVM_dump(vm);
         break;
   }
   
   return;
}
