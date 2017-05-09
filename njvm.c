#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"



int stack [1000];
int PC = 0;
int IR = 0;

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

void loadProgram(const char filename[]){
    FILE *program = fopen(filename, "r");   // "r" for reading

    /*
     *
     * read in file and save in memory.. no clue how that works
     *
     */

    fclose(program);
}

/* i think u fixed that allready  -- have a look
void listProgram(int prog){
    IR = 0;
    PC = 0;
    int program1Length = sizeof(program1)/ sizeof(program1[0]);
    while(IR < program1Length){
        switch(program1[IR] >> 24) {
            case HALT: {
                IR++;
                printf("HALT\n");
                break;

            }
            case PUSHC: {
                printf("PUSHC %d\n", (SIGN_EXTEND(program1[IR] & 0x00FFFFFF)));
                IR++;
                break;

            }
            case ADD: {
                IR++;
                printf("ADD\n");
                break;

            }
            case SUB: {
                IR++;
                printf("SUB\n");
                break;

            }
            case MUL: {
                IR++;
                printf("MUL\n");
                break;

            }
            case DIV: {
                IR++;
                printf("DIV\n");
                break;

            }
            case MOD: {
                IR++;
                printf("MOD\n");
                break;

            }
            case RDINT: {
                IR++;
                printf("RDINT\n");
                break;

            }
            case WRINT: {
                IR++;
                printf("WRINT\n");
                break;

            }
            case RDCHR: {
                IR++;
                printf("RDCHAR\n");
                break;

            }
            case WRCHR: {
                IR++;
                printf("WRCHAR\n");
                break;
            }
            default: {
                printf("Not defined!");
            }
        }
    }
}

 */
void executeProgram(unsigned int prog []){
    IR = 0;
    PC = 0;
    while(prog[IR] != HALT << 24){
        switch(prog[IR] >> 24){
            case PUSHC:
                {
                    push((SIGN_EXTEND(prog[IR] & 0x00FFFFFF)));
                    IR++;
                    break;

                }
            case ADD:
                {
                    IR++;
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
                    IR++;
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
                    IR++;
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
                    IR++;
                    printf("DIV\n");
                    break;

                }
            case MOD:
                {
                    IR++;
                    printf("MOD\n");
                    break;

                }
            case RDINT:
                {
                    IR++;
                    printf("RDINT\n");
                    break;

                }
            case WRINT:
                {
                    IR++;
                    printf("%d\n",stack[PC - 1]);
                    break;

                }
            case RDCHR:
                {
                    IR++;
                    printf("RDCHAR\n");
                    break;

                }
            case WRCHR:
                {
                    IR++;
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
	stack[PC++] = number;
}

void pop (){
	PC --;
}
