#include "moonvm.h"
#include "masm.h"
#include <stdio.h>

u16 fetch(MoonVM *);
Instruction decode(u16);
void execute(MoonVM *, Instruction);

MoonVM *MoonVM_init()
{
   MoonVM *vm = (MoonVM *)malloc(sizeof(MoonVM));

   vm->sp = 0xFF; /* Bottom of stack. Grows down */
   vm->isHalt = false;

   /* START PROG */
   /* Initialize r4 and r1 */
   vm->mem[0] = 0x4415; /* ld r4, 0x00 */
   vm->mem[1] = 0x4115; /* ld r1, 0x00 */
   vm->mem[2] = 0x4701; /* ld r7, 0x01 */
   vm->mem[3] = 0x4A0F; /* ld rA, 0x0F */

   /* Draw to display segment */
   vm->mem[4] = 0x3400; /* indx 0x400 */
   vm->mem[5] = 0x5BB4; /* st 0xB, 0xB4 */

   /* draw loop */
   vm->mem[6] = 0x1447; /* add r4, r4, r7 */
   vm->mem[7] = 0x1117; /* add r1, r1, r7 */
   vm->mem[8] = 0x7B41; /* spx 0x5, 0x4 0x1 */
   vm->mem[9] = 0x804A; /* cmp r4 rA */
   vm->mem[10] = 0x9206; /* JNE 6 */
   vm->mem[11] = 0x7B41; /* spx 0x5, 0x4 0x1 */

   /* call function */
   vm->mem[12] = 0xA00E; /* call 14 */

   /* Halt Program */
   vm->mem[13] = 0xF000; /* HALT */

   /* fn draw_hline */
   vm->mem[14] = 0x4000; /* ld r0 0x00; counter */
   vm->mem[15] = 0x4120; /* ld r1 0x20; xpos */
   vm->mem[16] = 0x4220; /* ld r2 0x20; ypos */
   vm->mem[17] = 0x4301; /* ld r3 0x01; step */
   vm->mem[18] = 0x4405; /* ld r4 0x05; line_len */
   vm->mem[19] = 0x7312; /* spx 0x3 r1 r2; draw pix */
   vm->mem[20] = 0x1113; /* add r1,r1,r3; incr r1 */
   vm->mem[21] = 0x1003; /* add r0,r0,r3; incr counter */
   vm->mem[22] = 0x8004; /* cmp r4 r0; line_len == counter ? */
   vm->mem[23] = 0x9212; /* JNE 0x12 (18); */
   vm->mem[24] = 0xB000; /* ret */

   /* END PROG */

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
