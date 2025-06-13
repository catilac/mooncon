#include "moonvm.h"
#include <stdio.h>

typedef enum Op
{
   NOP,
   ADD, // Add -- add r1, r0, r3
   LD, // load into register -- ld r15, 0xFF
   ST, // store register contents into memory -- st $0xFFFF, r15
   CLS, // clear screen -- cls
   SPX, // set a pixel --  spx 0x4 x y
   JMP, // JMP
   HALT, // halt program
} Op;

typedef struct Instruction
{
   Op op;
   u8 dest;
   u8 hn; // high nibble
   u8 ln; // low nibble
} Instruction;

u16 _fetch(MoonVM *);
Instruction _decode(u16);
void _execute(MoonVM *, Instruction);

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

void _dump(MoonVM *vm)
{
   printf("REGISTERS: \n");
   for (int i = 0; i < 16; i++)
      printf("R[%d] = %d\n", i, vm->reg[i]);
   printf("DISPLAY SEGMENT: \n");
   for (int i = 0; i < 2048; i++)
      printf("%02x ", vm->mem[VRAM+i]);
}

void MoonVM_fde_cycle(MoonVM *vm)
{
   // Fetch - get instruction from memory at current PC
   u16 word = _fetch(vm);
   // Decode - decode instruction
   Instruction i = _decode(word);
   
   // Execute - execute the decoded instruction
   _execute(vm, i);
}

u16 _fetch(MoonVM *vm)
{
   u16 bytecode = vm->mem[vm->pc++];
   printf("pc: %d -- \t", vm->pc);
   return bytecode;
}

Instruction _decode(u16 bytecode)
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

void _add(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   vm->reg[dest] = vm->reg[hn] + vm->reg[ln];
}

void _ld(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   printf("ld r%d 0x%d%d -- ", dest, hn, ln);

   vm->reg[dest] = (hn << 4) | ln;

   printf("%04x\n", vm->reg[dest]);

}

void _st(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   printf("st %04x %02x%02x -- ", dest, hn, ln);
   vm->mem[VRAM + dest] = (hn << 4) | ln;
}

void _cls(MoonVM *vm)
{
   for (int i = 0; i < DISPLAY_SIZE; i++)
      vm->mem[VRAM+i] = 0;
};

void _spx(MoonVM *vm, u8 col, u8 xreg, u8 yreg)
{
   u8  x = vm->reg[xreg];
   u8  y = vm->reg[yreg];
   u16  offset = (x + y * 64)/2;
   u16 byte = vm->mem[VRAM+offset];

   if (x % 2 == 0)
      byte |= (col << 8) & 0xF0; // hb
   else
      byte |= col & 0x0F; // lb
   vm->mem[VRAM + offset] = byte;
}

void _jmp(MoonVM *vm, u8 dest, u8 hn, u8 ln)
{
   u8 address = (hn << 4 & 0xF0) | ln;
   vm->pc = address;
}

void _execute(MoonVM *vm, Instruction i)
{
   printf("DEBUG: Instruction { op=(%d), dest=(%d), hn=(%d), ln=(%d)\n", i.op, i.dest, i.hn, i.ln);
   switch (i.op)
   {
      case NOP:
         break;
      case ADD:
         _add(vm, i.dest, i.hn, i.ln); 
         break;
      case LD:
         _ld(vm, i.dest, i.hn, i.ln);
         break;
      case ST:
         _st(vm, i.dest, i.hn, i.ln);
         break;
      case SPX:
         _spx(vm, i.dest, i.hn, i.ln);
         break;
      case CLS:
         _cls(vm);
         break;
      case JMP:
         _jmp(vm, i.dest, i.hn, i.ln);
         break;
      case HALT:
         vm->isHalt = true;
         printf("\n\n=========\n\t\tHALT\t\t\n==========\n\n");
         _dump(vm);
         break;
   }
   
   return;
}


