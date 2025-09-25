#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>

char *readFile(const char *filename)
{

   FILE *fp = fopen(filename, "r");
   if (fp == NULL)
   {
      printf("Error opening input file\n!");
      return NULL;
   }

   fseek(fp, 0, SEEK_END);
   long len = ftell(fp);
   rewind(fp);

   char *buffer = (char *)malloc(len);

   if (!buffer)
   {
      fprintf(stderr, "Could not allocate buffer memory!\n");
      fclose(fp);
      return NULL;
   }

   fread(buffer, 1, len, fp);
   fclose(fp);

   return buffer;
}
