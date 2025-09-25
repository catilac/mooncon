#ifndef MOONVM_H
#define MOONVM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_SIZE 2048

#define MEMORY_SIZE 32768 /* 2 << 14 */
#define STACK 8192 /* 2 << 12  */
#define VRAM 16384 /* 2 << 13 */

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef struct _MoonVM
{
   u16  I; // index register
   i8  reg[16]; // how many registers do we want?
   u16 pc; // program counter
   u8  sp; // stack pointer 

   u16 mem[MEMORY_SIZE];
   bool isHalt;
} MoonVM;

MoonVM *MoonVM_init();
void MoonVM_fde_cycle(); /* rename to MoonVM_run_cycle ? */
void MoonVM_compile(MoonVM *vm, const char *input);

#endif
