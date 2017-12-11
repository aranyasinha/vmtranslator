/*
code_generator does the main work of generating asm commands for the current vm
command and appends that to the asm file.
*/
void code_generator(char *command, char *vm_filename);

/*
type_of() takes the three arguments of a vm command and returns an int value 
based on the following table:
0 -> Arithematic and Logical Commands
1 -> Push
2 -> Pop
-1 -> unknown
tbc..:
*/
int type_of(char *arg1, char *arg2, char *arg3);

/*
typeof_pushpop() determines the type of push/pop command based on:
constant -> 0
local -> 1
argument -> 2
static > 3
temp -> 4
pointer -> 5
this -> 6
that -> 7
unknown -> -1
*/
int typeof_pushpop(char *arg2);

/*
typeof_al() determines the type of arithematic and logical command based on:
add -> 0
sub -> 1
neg -> 2
eq -> 3
gt -> 4
lt -> 5
and -> 6
or -> 7
not -> 8
unknown -1
*/
int typeof_al(char *arg1);

//code_generator_push() generates asm code for push commands 
char *code_generator_push(char *arg2, char *arg3, char *vm_filename);

//code_generator_pop() generates asm code for pop commands
char *code_generator_pop(char *arg2, char *arg3, char *vm_filename);
//code_generator_al() generates asm code for arithematic 
//and logical vm commands
char *code_generator_al(char *arg1);
