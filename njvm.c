#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"



int stack [STACK_SIZE]; /*stack */
int PC; /*program counter */
int SP; /*stack pointer */
int FP; /*frame pointer */
int *staticDataArea; /*static data area (for global vars) */


int main(int argc, char *argv [])
{
    
    /*manage parameter input */
  {
    int i;
    i=1;
    for(; i < argc; i++) 
    {
        if (strcmp(argv[i], ARG_VERSION) == 0) 
        {
            printf("Current Version: %d\n", NJVM_VERSION);
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], ARG_HELP) == 0) 
        {
            printHelp();
            exit(EXIT_SUCCESS);
        }
        else
        {
            loadProgram(argv[i]);
        }
    }
  }
  
  exit(EXIT_SUCCESS);
  
}

void printHelp(void)
{
	printf("Usage: njvm [option] [option] ... \n");
	printf("Arguments: \n");
	printf("%s\tShows the current version of njvm.\n", ARG_VERSION);
	printf("%s\t\tShows this help screen.\n", ARG_HELP);
    printf("\n");
}

void loadProgram(const char filename[]) {
    FILE *program = fopen("/home/lukas/Desktop/KsP/KsP/staticData", "r");   /*"r" for reading */
    if (program == NULL) {
        perror("File not found! ");
    } else {
        char *format;
        char *expectedString = "NJBF";
        int version;
        int instrSize;
        unsigned int *instructions; /*unsigned Integer containing the instructions */
        int numVars;
        unsigned int instr;

        printf("%s was found and opened ..\n", filename);

        /*Read first 4 bytes and check them for correct format */
        format = (char*) malloc (4);
        fread(format, 1, 4, program);
        if(strcmp(format, expectedString) != 0){
            perror("Incorrect Format!\n");
        }

        /*Read next 4 bytes and check them for correct Version */
        fread(&version, 4, 1, program);
        if(version != NJVM_VERSION){
            perror("Incorrect Version!\n");
        }

        /*Read next 4 bytes and check for the number of instructions */
        fread(&instrSize, 4, 1, program);
        instructions = (unsigned int*) malloc(instrSize * sizeof(int));

        /*Read next 4 bytes and check for the number of vars in static data area*/
        fread(&numVars, 4, 1, program);
        staticDataArea = (int*)malloc(numVars * sizeof(int));

        /*Read the next 4 bytes n times (based on instrSize) */
        do {
            fread(&instr, 4, 1, program);
            instructions[PC] = instr;
            PC ++;
        }
        while (PC < instrSize);

        PC = 0;
        SP = 0;

        listProgram(instructions, instrSize);

        PC = 0;
        SP = 0;

        executeProgram(instructions, numVars);

        fclose(program);
    }
}




void listProgram(unsigned int instructions [], int instrSize){

    while(PC < instrSize){
        int ins = instructions[PC];
        switch(instructions[PC] >> 24) {
            case HALT:
            {
                printf("HALT\n");
                PC++;
                break;
            }
            case PUSHC:
            {
                printf("PUSHC %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;

            }
            case ADD:
            {
                printf("ADD\n");
                PC++;
                break;
            }
            case SUB:
            {
                printf("SUB\n");
                PC++;
                break;
            }
            case MUL:
            {
                printf("MUL\n");
                PC++;
                break;
            }
            case DIV:
            {
                printf("DIV\n");
                PC++;
                break;
            }
            case MOD:
            {
                printf("MOD\n");
                PC++;
                break;
            }
            case RDINT:
            {
                printf("RDINT\n");
                PC++;
                break;
            }
            case WRINT:
            {
                printf("WRINT\n");
                PC++;
                break;
            }
            case RDCHR:
            {
                printf("RDCHAR\n");
                PC++;
                break;
            }
            case WRCHR:
            {
                printf("WRCHAR\n");
                PC++;
                break;
            }
            case PUSHG:
            {
                printf("PUSHG %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;
            }
            case POPG:
            {
                printf("POPG %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;
            }
            case ASF:
            {
                printf("ASF %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;
            }
            case RSF:
            {
                printf("RSF\n");
                PC++;
                break;
            }
            case PUSHL:
            {
                printf("PUSHL %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;
            }
            case POPL:
            {
                printf("POPL %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;
            }
            default: {
                printf("Not defined!");
            }
        }
    }

}

void executeProgram(unsigned int instructions [], int staticDataArea_size){

    while(instructions[PC] != HALT << 24){
        int ins = instructions[PC];
        switch(instructions[PC] >> 24){
            case PUSHC:
                {
                    push((SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                    PC++;
                    break;
                }
            case ADD:
                {
                    int add_num1 = pop();
                    int add_num2 = pop();
                    int sum;
                    sum = add_num2 + add_num1;
                    push(sum);
                    PC++;
                    break;

                }
            case SUB:
                {
                    int diff_num1 = pop();
                    int diff_num2 = pop();
                    int diff;
                    diff = diff_num2 - diff_num1;
                    push(diff);
                    PC++;
                    break;
                }
            case MUL:
                {
                    int prod_num1 = pop();
                    int prod_num2 = pop();
                    int prod;
                    prod = prod_num2 * prod_num1;
                    push(prod);
                    PC++;
                    break;
                }
            case DIV:
                {
                    int quo_num1 = pop ();
                    int quo_num2 = pop ();
                    if(quo_num1 == 0){
                        perror("You are trying to divide with '0', operation aborted!\n");
                    }
                    else {
                        int quo;
                        quo = quo_num2 / quo_num1;
                        push(quo);
                    }
                    PC++;
                    break;
                }
            case MOD:
                {
                    int mod_num1 = pop();
                    int mod_num2 = pop();
                    int mod;
                    mod = mod_num2 % mod_num1;
                    push(mod);
                    PC++;
                    break;
                }
            case RDINT:
                {
                    int num;
                    scanf("%d",&num);
                    push(num);
                    PC++;
                    break;

                }
            case WRINT:
                {
                    int num = pop();
                    printf("%d\n",num);
                    PC++;
                    break;

                }
            case RDCHR:
                {
                    char character;
                    scanf("%c",&character);
                    push(character);
                    PC++;
                    break;

                }
            case WRCHR:
                {
                    int c = pop();
                    printf("%c",c);
                    PC++;
                    break;
                }
            case PUSHG:
                {
                    int n = (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF));
                    if(n >= staticDataArea_size || n < 0){
                        perror("Out of bounds. You're not pointing to an index within the static data area!\n");
                    }
                    else {
                        push(staticDataArea[n]);
                    }
                    PC++;
                    break;
                }
            case POPG:
                {
                    int n = (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF));
                    if(n >=  staticDataArea_size || n < 0){
                        perror("Out of bounds. You're not pointing to an index within the static data area!\n");
                    }
                    else {
                        int val = pop();
                        staticDataArea[n] = val;
                    }
                    PC++;
                    break;
                }
            case ASF:
                {
                    int n = (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF));
                    if(n > STACK_SIZE - 2){
                        perror("Stack is too small to hold this stack frame!\n");
                    } else if (n <= 0){
                        perror("Cannot create a stack frame smaller than 1!\n");
                    } else {
                        push(FP);
                        FP = SP;
                        SP = SP + n; /*allocate new stack frame on stack by size 'n' */
                    }
                    PC++;
                    break;
                }
            case RSF:
                {
                    SP = FP;
                    FP = pop();
                    PC++;
                    break;
                }
            case PUSHL:
                {
                    int n = (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF));
                    if(n >= SP - FP || n < 0){
                        perror("Out of bounds. You're not pointing to an index within the stack frame!\n");
                    }
                    else {
                        push(stack[FP + n]);
                    }
                    PC++;
                    break;
                }
            case POPL:
                {
                    int n = (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF));
                    int val = pop();
                    stack[FP + n] = val;
                    PC++;
                    break;
                }
            default:
                {
                    printf("Not defined!\n");
                    PC++;
                }


        }
    }
}

void push(int number){
    if(SP > STACK_SIZE - 1){
        perror("Stack is full, push cannot be performed!\n");
    }
        else if(SP < 0){
        perror("Stack pointer is below zero, push cannot be performed!\n");
    }

    else {
        stack[SP++] = number;
    }
}

int pop (){
    return stack[--SP];
}
