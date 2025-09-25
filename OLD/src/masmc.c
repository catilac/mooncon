/* masmc.c */

#include "masmc.h"
#include "masm.h"
#include "moonvm.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *string_before_comment(char *line)
{
   size_t i;
   for (i = 0; i <= strlen(line); i++)
      if (line[i] == ';')
         break;

   if (i == 0)
      return NULL;

   char *substring = (char *)malloc(i * sizeof(char));
   strncpy(substring, line, i);

   return substring;
}

typedef enum state {
   Ins,
   Dest,
   Hn,
   Ln,
} State;

u8 parse_ins(const char *tok)
{
   if (strcmp(tok, "nop") == 0)
      return (u8)NOP;
   if (strcmp(tok, "add") == 0)
      return (u8)ADD;
   if (strcmp(tok, "sub") == 0)
      return (u8)SUB;
   if (strcmp(tok, "indx") == 0)
      return (u8)INDX;
   if (strcmp(tok, "ld") == 0)
      return (u8)LD;
   if (strcmp(tok, "st") == 0)
      return (u8)ST;
   if (strcmp(tok, "cls") == 0)
      return (u8)CLS;
   if (strcmp(tok, "jmp") == 0)
      return (u8)JMP;
   if (strcmp(tok, "call") == 0)
      return (u8)CALL;
   if (strcmp(tok, "ret") == 0)
      return (u8)RET;
   if (strcmp(tok, "halt") == 0)
      return (u8)HALT;
   if (strcmp(tok, "spx") == 0)
      return (u8)SPX;
   if (strcmp(tok, "cmp") == 0)
      return (u8)CMP;

   printf("\tDEBUG: HM -- %s\n",tok);
   return (u8)HALT;
}

u8 parse_operand(const char *tok)
{
   if (strcmp(tok, "r0") == 0)
      return 0x0;
   if (strcmp(tok, "r1") == 0)
      return 0x1;
   if (strcmp(tok, "r2") == 0)
      return 0x2;
   if (strcmp(tok, "r3") == 0)
      return 0x3;
   if (strcmp(tok, "r4") == 0)
      return 0x4;
   if (strcmp(tok, "r5") == 0)
      return 0x5;
   if (strcmp(tok, "r6") == 0)
      return 0x6;
   if (strcmp(tok, "r7") == 0)
      return 0x7;
   if (strcmp(tok, "r8") == 0)
      return 0x8;
   if (strcmp(tok, "r9") == 0)
      return 0x9;
   if (strcmp(tok, "ra") == 0)
      return 0xA;
   if (strcmp(tok, "rb") == 0)
      return 0xB;
   if (strcmp(tok, "rc") == 0)
      return 0xC;
   if (strcmp(tok, "rd") == 0)
      return 0xD;
   if (strcmp(tok, "re") == 0)
      return 0xE;
   if (strcmp(tok, "rf") == 0)
      return 0xF;

   return strtol(tok, NULL, 16);

}

u16 parse_line(char *line)
{
   State curr_state = Ins;
   u16 bytecode = 0;
   if (strlen(line) == 0)
      return 0;

   char *str = string_before_comment(line);
   if (str == NULL)
      return 0;

   char *tok_save_ptr = NULL;
   char *tok = strtok_r(str, " ,", &tok_save_ptr);
   while (tok)
   {
      switch (curr_state)
      {
         case Ins:
            bytecode |= (parse_ins(tok) << 12);
            break;
         case Dest:
            bytecode |= (parse_operand(tok) << 8);
            break;
         case Hn:
            bytecode |= (parse_operand(tok) << 4);
            break;
         case Ln:
            bytecode |= parse_operand(tok);
            break;
      }
      if (curr_state == Ins && strcmp(tok, "cmp") == 0)
         curr_state++;
      curr_state++;
      tok = strtok_r(NULL, " ,", &tok_save_ptr);

   }

   free(str);
   return bytecode;
}

void compile(MoonVM *vm, const char *input)
{

   char *copy_input = (char *)malloc(strlen(input) + 1);
   strcpy(copy_input, input);
   char *line_save_ptr = NULL;
   char *line = strtok_r(copy_input, "\n", &line_save_ptr);
   u16 pc = 0;
   while (line != NULL)
   {
      u16 instruction = parse_line(line);

      /* TODO: this isn't ideal but will work for now */
      if (instruction > 0)
      {
         vm->mem[pc] = instruction;
         printf("pc(%d) # %04x -- %s\n", pc++, instruction, line);
      }

      line = strtok_r(NULL, "\n", &line_save_ptr);
   }

   free(copy_input);

   return;
}
