#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"



int stack [10000]; //stack
int PC = NULL; //program counter
int SP = NULL //stack pointer


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
            loadProgram(argv[i]); //not sure if that works
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
    FILE *program = fopen("/home/lukas/Desktop/KsP/KsP/prog1", "r");   // "r" for reading
    if (program == NULL) {
        perror("File not found! ");
    } else {
        printf("%s was found and opened ..\n", filename);

        //Read first 4 bytes and check them for correct format
        char *format;
        char *expectedString = "NJBF";
        format = (char*) malloc (4);
        fread(format, 1, 4, program);
        if(strcmp(format, expectedString) != 0){
            perror("Incorrect Format!\n");
        }

        //Read next 4 bytes and check them for correct Version
        int version = NULL;
        int expectedVersion = 2;
        fread(&version, 4, 1, program);
        if(version != expectedVersion){
            perror("Incorrect Version!\n");
        }

        //Read next 4 bytes and check for the number of instructions
        int instrSize = NULL;
        fread(&instrSize, 4, 1, program);
        unsigned int instructions [instrSize]; //unsigned Integer containing the instructions

        //Read next 4 bytes and check for the number of vars in static data area
        int numVars = NULL;
        fread(&numVars, 4, 1, program);

        //Read the next 4 bytes n times (based on instrSize)
        int instr = NULL;
        for(int n = 0; n < instrSize; n++){
            fread(&instr, 4, 1, program);
            instructions[n] = instr;
        }

        PC = 0;
        SP = 0;

        /*
         *
         * read in file and save in memory.. no clue how that works
         *
         */

        fclose(program);
    }
}




void listProgram(int prog){
    /*
    int program1Length = sizeof(program1)/ sizeof(program1[0]);
    while(PC < program1Length){
        switch(program1[PC] >> 24) {
            case HALT: {
                PC++;
                printf("HALT\n");
                break;

            }
            case PUSHC: {
                printf("PUSHC %d\n", (SIGN_EXTEND(program1[PC] & 0x00FFFFFF)));
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
     */
}

void executeProgram(unsigned int prog []){
    PC = 0;
    PC = 0;
    while(prog[PC] != HALT << 24){
        switch(prog[PC] >> 24){
            case PUSHC:
                {
                    push((SIGN_EXTEND(prog[PC] & 0x00FFFFFF)));
                    PC++;
                    break;

                }
            case ADD:
                {
                    PC++;
                    {
                        int sum;
                        sum = stack[PC - 1] + stack [PC - 2];
                        pop();
                        pop();
                        push(sum);
                    }
                    break;

                }
            case SUB:
                {
                    PC++;
                    {
                        int diff;
                        diff = stack[PC - 2] - stack [PC - 1];
                        pop();
                        pop();
                        push(diff);
                    }
                    break;
                }
            case MUL:
                {
                    PC++;
                    {
                        int prod;
                        prod = stack[PC - 2] * stack [PC - 1];
                        pop();
                        pop();
                        push(prod);
                    }
                    break;

                }
            case DIV:
                {
                    PC++;
                    printf("DIV\n");
                    break;

                }
            case MOD:
                {
                    PC++;
                    printf("MOD\n");
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
                    printf("Not defined!");
                }


        }
    }
}

void push(int number){
	stack[SP++] = number;
}

void pop (){
	SP --;
}
