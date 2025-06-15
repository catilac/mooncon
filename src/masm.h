#ifndef _MASM_H
#define _MASM_H

#include "moonvm.h"

/* 
 * We have 15 operations 0x0 -> 0xF.
 * One is Reserved
 */
typedef enum Op
{
   NOP,
   ADD,
   SUB,
   INDX,
   LD,
   ST,
   CLS,
   SPX,
   CMP,
   JMP,
   CALL,
   RET,
   RES_0, /* Reserved */
   RES_1, /* Reserved */
   RES_2, /* Reserved */
   HALT,
} Op;

/* 
 * We are only using 4 bits of
 * the u8 defined for `hn` and `ln`
 */
typedef struct Instruction
{
   Op op;
   u8 dest;
   u8 hn; /* high nibble */
   u8 ln; /* low nibble */
} Instruction;

void add(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void sub(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void indx(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void ld(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void st(MoonVM *vm, u8 dest, u8 hn, u8 ln);

void cls(MoonVM *vm);
void spx(MoonVM *vm, u8 col, u8 xreg, u8 yreg);

void cmp(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void jmp(MoonVM *vm, u8 dest, u8 hn, u8 ln);

void call(MoonVM *vm, u8 dest, u8 hn, u8 ln);
void ret(MoonVM *vm);

#endif
