#ifndef MOONVM_H
#define MOONVM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_SIZE 2048

#define MEMORY_SIZE 32768
#define RAM 0 
#define VRAM (2 << 13)

typedef uint8_t u8;
typedef uint16_t u16;
typedef struct _MoonVM
{
   u8  acc; // are we using this?
   u8  reg[16]; // how many registers do we want?
   unsigned int pc;

   u16 mem[MEMORY_SIZE];
   bool isHalt;
} MoonVM;

MoonVM *MoonVM_init();
void MoonVM_fde_cycle();

#endif
