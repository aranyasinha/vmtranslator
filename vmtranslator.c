#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"
#define LINE_SIZE 200

int translate_file(char *vm_filename);
int translate_directory(char *directoryname);
void copy_file(char *dest, char *src);

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    fprintf(stderr, "Usage: vmtranslator /path/to/file_name.vm OR\
                     vmtranslator /path/to/directory\n");
    return 1;
  }
  
  if(strstr(argv[1], ".vm") != NULL)
    translate_file(argv[1]);
  else translate_directory(argv[1]);

return 0;
}

int translate_file(char *vm_filename)
{
  //Opening the vm file
  FILE *vm_file = fopen(vm_filename, "r");
  
  if(vm_file == NULL)
  {
    fprintf(stderr, "Error: File not found.\n");
    return 2;
  }
  
  //Tracking the command number
  int n = 1;
  
  //Looping through the vm file
  while(!feof(vm_file))
  {
    //reading the current command from the vm file
    char *command = (char *)malloc(sizeof(char *) * LINE_SIZE);
    fgets(command, LINE_SIZE, vm_file);
    
    if(command == NULL)
    {
      fprintf(stderr, "Error: Could not read from file.\n");
      return 3;
    }
    
    //Ignoring comments and white space
    if(!isalpha(command[0]) || feof(vm_file))
    {
      free(command);
      continue;
    }

    fprintf(stdout, "%d %s", n, command);
  
    //generating and appending assembly code.
    code_generator(command, vm_filename);
    
    if(command != NULL)
      free(command);
    n++;
  }

  fclose(vm_file);
  return 0;
}

int translate_directory(char *directoryname)
{
  //Opening the directory
  DIR *dir = opendir(directoryname);
  struct dirent *entry_pointer;

  if(dir == NULL)
  {
    fprintf(stderr, "Error: Could not open directory\n");
    return 4;
  }

  //getcwd() returns the full path
  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd)) == NULL)
  {
    fprintf(stderr, "Error: Can't get current directory\n");
    return 5;
  }
  
  char *asm_filename = strrchr(cwd, '/');
  asm_filename++; // /DirectoryName to DirectoryName
  strcat(asm_filename, ".asm");

  //Looking for .vm file, translating it, copying it to 
  //the main .asm file and deleting it.
  while((entry_pointer = readdir(dir) ))
  {
    if(entry_pointer->d_type == DT_REG && 
       strstr(entry_pointer->d_name,".vm") != NULL)
    {
      translate_file(entry_pointer->d_name);
      char *translated_asm = (char *)malloc(sizeof(char) * 20);
      strcpy(translated_asm, strtok(entry_pointer->d_name, "."));
      strcat(translated_asm, ".asm");
      copy_file(asm_filename, translated_asm);
      remove(translated_asm);
    }
  }
  
  return 0;
}

void copy_file(char *dest, char *src)
{
 FILE *dest_p, *src_p;

 int a; //For copying

 dest_p = fopen(dest, "a");
 src_p = fopen(src, "r");

 while( (a = fgetc(src_p)) != EOF)
   fputc(a, dest_p);

 fclose(dest_p);
 fclose(src_p);
}
