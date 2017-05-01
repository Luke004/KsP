#include <stdio.h>
#include <string.h>
#include "njvm.h"



int stack [1000];
int PC = 0;
int IR = 0;
unsigned int program1 [] = {
    (PUSHC << 24)|IMMEDIATE(3),
    (PUSHC << 24)|IMMEDIATE(4),
    (ADD << 24),
    (PUSHC << 24)|IMMEDIATE(10),
    (PUSHC << 24)|IMMEDIATE(6),
    (SUB << 24),
    (MUL << 24),
    (WRINT << 24),
    (PUSHC << 24)|IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)
};
int program1Length = 11;

int main(int argc, char *argv []){

    if(argc == 1){
            printf("Kein Kommando eingegeben\n");
            printf("--help für eine Kommandoliste");
            exit(99);
    }

    if (strcmp(argv[1], "--prog1") == 0) {
        printf("Ninja Virtual Machine started ...\n");
        listProgram(1);
        executeProgram(1);
        printf("Ninja Virtual Machine stopped ...\n");
    }
	else if (strcmp(argv[1], "--prog2") == 0){
		printf("Es wurde prog2 ausgewählt");
	}
	else if (strcmp(argv[1], "--prog3") == 0){
		printf("Es wurde prog3 ausgewählt");
	}
	else if (strcmp(argv[1], "--help") == 0){
		printf("Es wurde die Hilfe ausgewählt");
	}
	else {
		printf("Kein Kommando eingegeben, Programm wird beendet ..");

	}

	return 1;
}

void listProgram(int prog){
    IR = 0;
    PC = 0;
    switch (prog){
        case 1:
        {
            while(IR < program1Length){
                switch(program1[IR] >> 24){
                    case HALT:
                        {
                            IR++;
                            printf("HALT\n");
                            break;

                        }
                    case PUSHC:
                        {
                            printf("PUSHC %d\n", (SIGN_EXTEND(program1[IR] & 0x00FFFFFF)));
                            IR++;
                            break;

                        }
                    case ADD:
                        {
                            IR++;
                            printf("ADD\n");
                            break;

                        }
                    case SUB:
                        {
                            IR++;
                            printf("SUB\n");
                            break;

                        }
                    case MUL:
                        {
                            IR++;
                            printf("MUL\n");
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
                            printf("WRINT\n");
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
                            printf("WRCHAR\n");
                            break;
                        }
                    default:
                        {
                            printf("Not defined!");
                        }

                }
            }
        }

        case 2:
        {

        }

        case 3:
        {

        }

            }
}

void executeProgram(int prog){
    IR = 0;
    PC = 0;
    while(program1[IR] != HALT << 24){
        switch(program1[IR] >> 24){
            case PUSHC:
                {
                    push((SIGN_EXTEND(program1[IR] & 0x00FFFFFF)));
                    IR++;
                    break;

                }
            case ADD:
                {
                    IR++;
                    int sum = stack[PC - 1] + stack [PC - 2];
                    pop();
                    pop();
                    push(sum);
                    break;

                }
            case SUB:
                {
                    IR++;
                    int diff = stack[PC - 2] - stack [PC - 1];
                    pop();
                    pop();
                    push(diff);
                    break;

                }
            case MUL:
                {
                    IR++;
                    int prod = stack[PC - 2] * stack [PC - 1];
                    pop();
                    pop();
                    push(prod);
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
