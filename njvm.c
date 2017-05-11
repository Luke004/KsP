#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"



int stack [STACK_SIZE]; /*stack */
int PC; /*program counter */
int SP; /*stack pointer */


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
    FILE *program = fopen("/home/lukas/Desktop/KsP/KsP/prog1", "r");   /*"r" for reading */
    if (program == NULL) {
        perror("File not found! ");
    } else {
        char *format;
        char *expectedString = "NJBF";
        int version;
        int instrSize;
        unsigned int *instructions; /*unsigned Integer containing the instructions */
        int numVars;
        int instr;

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
        instructions = (unsigned int*) malloc(instrSize);

        /*Read next 4 bytes and check for the number of vars in static data area*/
        fread(&numVars, 4, 1, program);

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

        executeProgram(instructions);

        fclose(program);
    }
}




void listProgram(unsigned int instructions [], int instrSize){

    while(PC < instrSize){
        switch(instructions[PC] >> 24) {
            case HALT: {
                PC++;
                printf("HALT\n");
                break;

            }
            case PUSHC: {
                printf("PUSHC %d\n", (SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                PC++;
                break;

            }
            case ADD: {
                PC++;
                printf("ADD\n");
                break;

            }
            case SUB: {
                PC++;
                printf("SUB\n");
                break;

            }
            case MUL: {
                PC++;
                printf("MUL\n");
                break;

            }
            case DIV: {
                PC++;
                printf("DIV\n");
                break;

            }
            case MOD: {
                PC++;
                printf("MOD\n");
                break;

            }
            case RDINT: {
                PC++;
                printf("RDINT\n");
                break;

            }
            case WRINT: {
                PC++;
                printf("WRINT\n");
                break;

            }
            case RDCHR: {
                PC++;
                printf("RDCHAR\n");
                break;

            }
            case WRCHR: {
                PC++;
                printf("WRCHAR\n");
                break;
            }
            default: {
                printf("Not defined!");
            }
        }
    }

}

void executeProgram(unsigned int instructions []){

    while(instructions[PC] != HALT << 24){
        switch(instructions[PC] >> 24){
            case PUSHC:
                {
                    PC++;
                    push((SIGN_EXTEND(instructions[PC] & 0x00FFFFFF)));
                    break;
                }
            case ADD:
                {
                    int sum;
                    PC++;
                    sum = stack[PC - 1] + stack [PC - 2];
                    pop();
                    pop();
                    push(sum);
                    break;

                }
            case SUB:
                {
                    int diff;
                    PC++;
                    diff = stack[PC - 2] - stack [PC - 1];
                    pop();
                    pop();
                    push(diff);
                    break;
                }
            case MUL:
                {
                    int prod;
                    PC++;
                    prod = stack[PC - 2] * stack [PC - 1];
                    pop();
                    pop();
                    push(prod);
                    break;
                }
            case DIV:
                {
                    if(stack[PC - 1] == 0){
                        perror("You are trying to divide with '0', operation aborted!\n");
                    }
                    else {
                        int quo;
                        PC++;
                        quo = stack[PC - 2] / stack [PC - 1];
                        pop();
                        pop();
                        push(quo);
                    }
                    break;
                }
            case MOD:
                {
                    int mod;
                    PC++;
                    mod = stack[PC - 2] % stack [PC - 1];
                    pop();
                    pop();
                    push(mod);
                    break;
                }
            case RDINT:
                {
                    PC++;
                    printf("RDINT\n");
                    break;

                }
            case WRINT:
                {
                    PC++;
                    printf("%d\n",stack[PC - 1]);
                    break;

                }
            case RDCHR:
                {
                    PC++;
                    printf("RDCHAR\n");
                    break;

                }
            case WRCHR:
                {
                    PC++;
                    printf("%c",stack [PC - 1]);
                    break;
                }
            default:
                {
                    printf("Not defined!\n");
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

void pop (){
	SP --;
}
