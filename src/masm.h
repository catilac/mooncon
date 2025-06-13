#ifndef _MASM_H
#define _MASM_H

#include "moonvm.h"

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

void add(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void ld(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void st(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void cls(MoonVM *vm);
void spx(MoonVM *vm, u8 col, u8 xreg, u8 yreg);
void jmp(MoonVM *vm, u8 dest, u8 hn, u8 ln);


//void cmp(MoonVM *vm, u8 dest, u8 hn, u8 ln);
//void indx(MoonVM *vm, u8 dest, u8 hn, u8 ln);
//void sub(MoonVM *vm, u8 dest, u8 hn, u8 ln);

//void call(MoonVM *vm, u8 dest, u8 hn, u8 ln);

// Stack
// void push(MoonVM *vm, u8 dest, u8 hn, u8 ln);
// void pop(MoonVM *vm, u8 dest, u8 hn, u8 ln);


#endif
