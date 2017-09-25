#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "helpers.h"
#define LINE_SIZE 200

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("Usage: vmtranslator /path/to/file_name.vm");
    return 1;
  }

  //Opening the vm file
  FILE *vm_file = fopen(argv[1], "r");
  if(vm_file == NULL)
  {
    printf("File not found.\n");
    return 2;
  }
  
  //Tracks the command number
  int n = 1;
  
  //Looping through the vm file
  while(!feof(vm_file))
  {
    //reading the current command from the vm file
    char *command = (char *)malloc(sizeof(char) * LINE_SIZE);
    fgets(command, LINE_SIZE, vm_file);
    if(command == NULL)
    {
      printf("File I/O error.\n");
      return 3;
    }
    
    //Ignoring comments and white space
    if(!isalpha(command[0]) || feof(vm_file))
    {
      free(command);
      continue;
    }
    printf("%d %s", n, command);
    //generates and appends assembly code.
    code_generator(command, argv[1]);
    
    free(command);
    n++;
  }

  fclose(vm_file);
  printf( "File translation completed.\n");
  return 0;
}
