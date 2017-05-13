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
        if (strcmp(argv[1], ARG_VERSION) == 0)
        {
            printf("Current Version: %d\n", NJVM_VERSION);
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[1], ARG_HELP) == 0)
        {
            printHelp();
            exit(EXIT_SUCCESS);
        }
        else if(argc == 3 && strcmp(argv[2], DEBUG_MODE) == 0)
        {
            loadProgram(argv[1],1);
        }
        else {
            loadProgram(argv[1],0);
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

void loadProgram(const char filename[], int debug) {
    FILE *program = fopen("/home/lukas/Desktop/KsP/KsP/newprog2", "r");   /*"r" for reading */
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

        /*Read first 4 bytes and check them for correct format */
        format = (char *) malloc(4);
        fread(format, 1, 4, program);
        if (strcmp(format, expectedString) != 0) {
            perror("Incorrect Format!\n");
        } else {
            /*Read next 4 bytes and check them for correct Version */
            fread(&version, 4, 1, program);
            if (version != NJVM_VERSION) {
                perror("Incorrect Version!\n");
            } else {
                /*Read next 4 bytes and check for the number of instructions */
                fread(&instrSize, 4, 1, program);
                instructions = (unsigned int *) malloc(instrSize * sizeof(int));

                /*Read next 4 bytes and check for the number of vars in static data area*/
                fread(&numVars, 4, 1, program);
                staticDataArea = (int *) malloc(numVars * sizeof(int));

                /*Read the next 4 bytes n times (based on instrSize) */
                do {
                    fread(&instr, 4, 1, program);
                    instructions[PC] = instr;
                    PC++;
                } while (PC < instrSize);

                PC = 0;
                SP = 0;

                if (debug == 1) {
                    char *commands[6] = {"inspect", "list", "breakpoint", "step", "run", "quit"};
                    char *input = (char*) malloc(12);
                    printf("DEBUG: file %s loaded ", filename);
                    printf("(code size = %d, ", instrSize);
                    printf("data size = %d)\n", numVars);
                    printf("Ninja Virtual Machine started\n");
                    listInstruction(instructions[PC--]);
                    printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
                    scanf("%s", input);

                    while (strcmp(input, commands[5]) != 0) {
                        if (strcmp(input, commands[0]) == 0) {
                            /*inspect */
                            printf("DEBUG [inspect]: stack, data?\n");
                            scanf("%s", input);
                            if(strcmp(input, "stack") == 0) {
                                int SP_output = SP;
                                if(FP == SP){
                                    printf("FP, SP --> [%d]: xxx\n",SP_output);
                                    SP_output--;
                                }
                                else {
                                    printf("SP   -->   [%d]: xxx\n", SP_output);
                                    SP_output--;
                                }
                                do{
                                    if(FP == SP_output) {
                                        printf("FP    -->  [%d]: ", FP);
                                        printf("%d\n", stack[FP]);
                                    }
                                    else if(SP_output >= 0){
                                        printf("           [%d]: ", SP_output);
                                        printf("%d\n", stack[SP_output]);
                                    }
                                    SP_output--;
                                }while (SP_output >= 0);
                                printf(" -- bottom of stack --\n");
                                listInstruction(instructions[PC--]);
                                printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
                                scanf("%s", input);
                            } else if(strcmp(input, "data") == 0) {

                            }
                        } else if (strcmp(input, commands[1]) == 0) {
                            /*list */
                            int temp = PC;
                            PC = 0;
                            listProgram(instructions,instrSize);
                            PC = temp;
                            printf(" --- end of list ---\n");
                            listInstruction(instructions[--PC]);
                            printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
                            scanf("%s", input);
                        } else if (strcmp(input, commands[2]) == 0) {
                            /*breakpoint */
                            scanf("%s", input);
                        } else if (strcmp(input, commands[3]) == 0) {
                            /*step */
                            makeDebugStep(instructions, numVars, 1);
                            printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
                            scanf("%s", input);
                        } else if (strcmp(input, commands[4]) == 0) {
                            /*run */
                            executeProgram(instructions, numVars);
                            fclose(program);
                            printf("Ninja Virtual Machine stopped\n");
                            exit(EXIT_SUCCESS);
                        } else {
                            printf("Invalid command! \n");
                            scanf("%s", input);
                        }
                    }
                }
                else {
                    printf("Ninja Virtual Machine started\n");
                    executeProgram(instructions, numVars);
                }
                printf("Ninja Virtual Machine stopped\n");
                fclose(program);
            }
        }
    }
}



void listProgram(unsigned int instructions [], int instrSize){

    while(PC < instrSize){
        listInstruction(instructions[PC]);
    }

}

void listInstruction(unsigned int instruction){
    switch(instruction >> 24) {
        case HALT:
        {
            printf("HALT\n");
            PC++;
            break;
        }
        case PUSHC:
        {
            printf("PUSHC %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
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
            printf("PUSHG %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case POPG:
        {
            printf("POPG %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case ASF:
        {
            printf("ASF %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
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
            printf("PUSHL %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case POPL:
        {
            printf("POPL %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case EQ:
        {
            printf("EQ\n");
            PC++;
            break;
        }
        case NE:
        {
            printf("NE\n");
            PC++;
            break;
        }
        case LT:
        {
            printf("LT\n");
            PC++;
            break;
        }
        case LE:
        {
            printf("LE\n");
            PC++;
            break;
        }
        case GT:
        {
            printf("GT\n");
            PC++;
            break;
        }
        case GE:
        {
            printf("GE\n");
            PC++;
            break;
        }
        case JMP:
        {
            printf("JMP %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case BRF:
        {
            printf("BRF %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case BRT:
        {
            printf("BRT %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        default:
        {
            printf("Not defined!");
            PC++;
        }
    }
}

void executeProgram(unsigned int instructions [], int staticDataArea_size){
    while(instructions[PC] != HALT << 24){
        execInstruction(instructions[PC], staticDataArea_size);
    }
}

void makeDebugStep(unsigned int instructions [], int staticDataArea_size, int steps) {
    while (instructions[PC] != HALT << 24 && steps != 0) {
        execInstruction(instructions[PC], staticDataArea_size);
        listInstruction(instructions[PC--]);
        steps--;
    }
}

void execInstruction(unsigned int instruction, int staticDataArea_size){
    switch(instruction >> 24){
        case PUSHC:
        {
            push((SIGN_EXTEND(instruction & 0x00FFFFFF)));
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
            printf("Please enter an integer number: \n");
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
            printf("Please enter a character ascii code: \n");
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
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
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
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
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
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
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
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
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
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
            int val = pop();
            stack[FP + n] = val;
            PC++;
            break;
        }
        case EQ:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 == num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case NE:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 != num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case LT:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 < num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case LE:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 <= num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case GT:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 > num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case GE:
        {
            int num1 = pop();
            int num2 = pop();
            if(num2 >= num1){
                push(1);
            }
            else
            {
                push(0);
            }
            PC++;
            break;
        }
        case JMP:
        {
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
            PC = n;
            break;
        }
        case BRF:
        {
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
            int jump = pop();
            if(jump == 0){
                PC = n;
            }
            else if(jump == 1){
                PC++;
            }
            else {
                perror("No valid input for boolean (has to be '0' or '1'\n");
                PC++;
            }
            break;
        }
        case BRT:
        {
            int n = (SIGN_EXTEND(instruction & 0x00FFFFFF));
            int jump = pop();
            if(jump == 1){
                PC = n;
            }
            else if (jump == 0){
                PC++;
            }
            else {
                perror("No valid input for boolean (has to be '0' or '1'\n");
                PC++;
            }
            break;
        }
        default:
        {
            printf("Not defined!\n");
            PC++;
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
