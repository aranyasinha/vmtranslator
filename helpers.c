#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

//A counter for logical commands
int label_n = 0;

int type_of(char *arg1, char *arg2, char *arg3)
{

  if(arg2 == NULL && arg3 == NULL)
    return 0;
  else if(strcmp(arg1, "push") == 0)
    return 1;
  else if(strcmp(arg1, "pop") == 0)
    return 2;
  else return -1;

}

int typeof_pushpop(char *arg2)
{
  if(strcmp(arg2, "constant") == 0)
    return 0;
  else if(strcmp(arg2, "local") == 0)
    return 1;
  else if(strcmp(arg2, "argument") == 0)
    return 2;
  else if(strcmp(arg2, "static") == 0)
    return 3;
  else if(strcmp(arg2, "temp") == 0)
    return 4;
  else if(strcmp(arg2, "pointer") == 0)
    return 5;
  else if(strcmp(arg2, "this") == 0 )
    return 6;
  else if(strcmp(arg2, "that") == 0)
    return 7;
  else return -1;

}

int typeof_al(char *arg1)
{
  if(strcmp(arg1, "add") == 0)
    return 0;
  else if(strcmp(arg1, "sub") == 0)
    return 1;
  else if(strcmp(arg1, "neg") == 0)
    return 2;
  else if(strcmp(arg1, "eq") == 0)
    return 3;
  else if(strcmp(arg1, "gt") == 0)
    return 4;
  else if(strcmp(arg1, "lt") == 0)
    return 5;
  else if(strcmp(arg1, "and") == 0)
    return 6;
  else if(strcmp(arg1, "or") == 0)
    return 7;
  else if(strcmp(arg1, "not") == 0)
    return 8;
  else return -1;

}

char *set_register(int t, char *arg3)
{
  char *reg = (char *)malloc(sizeof(char) * 5);
  if(t == 1)
    strcpy(reg, "LCL");
  else if(t == 2)
    strcpy(reg, "ARG");
  else if(t == 4)
    strcpy(reg, "5");
  else if(t == 5)
  {
    if(strcmp(arg3, "0") == 0)
      strcpy(reg, "THIS");
    else if(strcmp(arg3, "1") == 0)
      strcpy(reg, "THAT");
    else
    {
      printf("Invalid pointer command.\n");
      return NULL;
    }
  }
  else if(t == 6)
    strcpy(reg, "THIS");
  else if(t == 7)
    strcpy(reg, "THAT");

  return reg;
}

void code_generator(char *command, char *vm_filename)
{
  char command_copy[strlen(command) + 1];
  strcpy(command_copy, command);
  //Getting the three arguments from the commands
  char *arg1, *arg2, *arg3, *save_ptr;
  arg1 = strtok_r(command_copy, "\n ", &save_ptr);
  arg2 = strtok_r(NULL, " ", &save_ptr);
  arg3 = strtok_r(NULL, "\n ", &save_ptr);

  //Determines the type of vm command
  int t = type_of(arg1, arg2, arg3);
  if(t == -1)
  {
    printf("Invalid command type.\n");
    return;
  }

  //Initializing the string which will hold the asm code that will be generated
  char *asm_commands = NULL;
  
  //Opening the asm file
  char *asm_filename = (char *)malloc(sizeof(char) * 20);
  strcpy(asm_filename, strtok(vm_filename, "."));
  strcat(asm_filename, ".asm");
  FILE *asm_file = fopen(asm_filename, "a");
  
  if(t == 0) //a and l command
  {
    asm_commands = code_generator_al(arg1);
  }
  else if(t == 1) //push command
  {
    asm_commands = code_generator_push(arg2, arg3);
  } 
  else asm_commands = NULL;
  
  if(asm_commands == NULL)
  {
    free(asm_filename);
    fclose(asm_file);
    return;
  }

  //The first line will be a comment indicating the vm command that is being translated
  fprintf(asm_file, "//%s", command);
  //The asm code follows
  fprintf(asm_file, "%s", asm_commands);

  free(asm_commands);
  free(asm_filename);

  fclose(asm_file);
}

char *code_generator_push(char *arg2, char *arg3)
{
  int t_push = typeof_pushpop(arg2);
  if(t_push == -1)
  {
    printf("Invalid push command.\n");
    return NULL;
  }

  if(t_push == 0) //const
  {
    char *s = (char *)malloc(sizeof(char) * 50);
    strcpy(s,"@");
    strcat(s,arg3);
    strcat(s,"\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    return s;
  }
  else if(t_push == 3) // static
  {
    //TODO
    return NULL;
  }
  else if(t_push != -1)//lcl, arg, temp, pointer, this and that
  {
    char *s = (char *)malloc(sizeof(char) * 100);

    //Setting the register that will be used
    char *reg = set_register(t_push, arg3);
    
    strcpy(s, "@");
    strcat(s, arg3);
    strcat(s, "\nD=A\n@");
    strcat(s, reg);
    strcat(s, "\nD=D+M\n@addr\nM=D\n@addr\nA=M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    free(reg);
    return s;
  }
  else return NULL;
}

char *code_generator_pop(char *arg2, char *arg3)
{
  int t_pop = typeof_pushpop(arg2);

  if(t_pop == -1)
  {
    printf("Invalid pop command.\n");
    return NULL;
  }

  if(t_pop == 3) //static
  {
    //TODO
    return NULL;
  }
  else if(t_pop != -1) //lcl, arg, temp, pointer, this & that
  {
    char *s = (char *)malloc(sizeof(char) * 100);
    
    //setting the register that will be used
    char *reg = set_register(t_pop, arg3);    

    //TODO
    free(reg);
    return s;
  }
  return NULL; 
}

char *code_generator_al(char *arg1)
{
  int t_al = typeof_al(arg1);
  if(t_al == -1)
  {
    printf("Invalid Arithematic and Logical command.\n");
    return NULL;
  }

  //op[2] holds the operator to be used
  char op[2];
  op[1] = '\0';
  char *s = NULL;

  if(t_al == 0)
    op[0] = '+';
  else if(t_al == 1)
    op[0] = '-';
  else if(t_al == 6)
    op[0] = '&';
  else if(t_al == 7)
    op[0] = '|';
  else if(t_al == 2)
      op[0] = '-';
  else if(t_al == 8)
      op[0] = '!';

  if(t_al == 0 || t_al == 1 || t_al == 6 || t_al == 7)//add, sub, and, or
  {
    s = (char *)malloc(sizeof(char) * 50);
    strcpy(s, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M");
    strcat(s, op);
    strcat(s, "D\n@SP\nM=M+1\n");
    return s;
  }
  else if (t_al == 2 || t_al == 8)//neg, not
  {
    
    s = (char *)malloc(sizeof(char) * 50);
    strcpy(s, "@SP\nAM=M-1\nM=");
    strcat(s, op);
    strcat(s, "M\n@SP\nM=M+1\n");
    return s;
  }
  else if(t_al == 3 || t_al == 4 || t_al == 5)//eq, gt, lt
  {
    char jmp_code[4];
    label_n++;
    char lbl_no[2];
    lbl_no[0] = label_n + 'A';
    lbl_no[1] = '\0';

    if(t_al == 3)
      strcpy(jmp_code, "JEQ");
    else if(t_al == 4)
      strcpy(jmp_code, "JGT");
    else strcpy(jmp_code, "JLT");

    s = (char *)malloc(sizeof(char) * 200);
    strcpy(s, "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@lbl_");
    strcat(s, lbl_no);
    strcat(s,"\nD;");
    strcat(s, jmp_code);
    strcat(s, "\n@SP\nA=M\nM=0\n@lbl_end_");
    strcat(s, lbl_no);
    strcat(s, "\n0;JMP\n(lbl_");
    strcat(s, lbl_no);
    strcat(s, ")\n@SP\nA=M\nM=-1\n(lbl_end_");
    strcat(s, lbl_no);
    strcat(s, ")\n@SP\nM=M+1\n");
    return s;
  }

  return NULL; 
}
