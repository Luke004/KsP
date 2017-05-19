#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"



int stack [STACK_SIZE]; /*stack */
int PC; /*program counter */
int SP; /*stack pointer */
int FP; /*frame pointer */
int *staticDataArea; /*static data area (holds global vars) */
ReturnRegister returnRegister; /*return value register (holds method return values) */
int RP; /*return register pointer */


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
            loadProgram(argv[1],true);
        }
        else {
            loadProgram(argv[1],false);
        }

  }
  
  exit(EXIT_SUCCESS);
  
}

/*
 * prints a simple help screen
 */
void printHelp(void)
{
    printf("Execute Programs:\n");
    printf("njvm [program]\t\t\t executes a program\n");
    printf("njvm [program] %s\t executes a program in debug mode\n", DEBUG_MODE);
    printf("\n");
    printf("Direct Arguments: \n");
	printf("njvm %s\tShows the current version of njvm.\n", ARG_VERSION);
	printf("njvm %s\t\tShows this help screen.\n", ARG_HELP);

    printf("\n");
}

void loadProgram(const char filename[], bool debug  ) {
    FILE *program = fopen("/home/lukas/Desktop/KsP/KsP/prog1.bin", "r");   /*"r" for reading */
    char *format;
    char *expectedString = "NJBF";
    int version;
    int instrSize;
    unsigned int *instructions; /*unsigned Integer containing the instructions */
    int numVars;
    unsigned int instr;
    int i;

    /* return if program does not exist */
    if (program == NULL) {
        perror("File not found! ");
        exit(EXIT_FAILURE);
    }

    /*Read first 4 bytes and check them for correct format */
    format = (char *) malloc(4);
    fread(format, 1, 4, program);
    if (strcmp(format, expectedString) != 0) {
        perror("Incorrect Format!\n");
        exit(EXIT_FAILURE);
    }

    /*Read next 4 bytes and check them for correct Version */
    fread(&version, 4, 1, program);
    if (version > NJVM_VERSION) {
        perror("Your Version is too old!\n");
        exit(EXIT_FAILURE);
    }

    /*Read next 4 bytes and check for the number of instructions */
    fread(&instrSize, 4, 1, program);
    instructions = (unsigned int *) malloc(instrSize * sizeof(int));

    /*Read next 4 bytes and check for the number of vars in static data area*/
    fread(&numVars, 4, 1, program);
    staticDataArea = (int *) malloc(numVars * sizeof(int));

    /*Read the next 4 bytes n times (based on instrSize) */
    i = 0;
    do {
        fread(&instr, 4, 1, program);
        instructions[i] = instr;
        i++;
    } while (i < instrSize);

    PC = 0;
    SP = 0;
    RP = 0;

    if (debug == true) {
        char *commands[6] = {"inspect", "list", "breakpoint", "step", "run", "quit"};
        char *input = (char*) malloc(12);
        int breakpoint = -1;
        printf("DEBUG: file %s loaded ", filename);
        printf("(code size = %d, ", instrSize);
        printf("data size = %d)\n", numVars);

        printf("Ninja Virtual Machine started\n");

        /* performs one step after another till user writes "run" or "quit" represented by commands[4] and commands[5] */
        while (strcmp(input, commands[5]) != 0) {

            listInstruction(instructions[PC--]);
            printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");
            scanf("%s", input);

            /* inspect */
            if (strcmp(input, commands[0]) == 0)
            {
                printf("DEBUG [inspect]: stack, data?\n");
                scanf("%s", input);

                /* inspect stack */
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
                }

                /* inspect data */
                else if(strcmp(input, "data") == 0) {
                    int i = 0;
                    while(i < numVars) {
                        printf("data[%d]: ", i);
                        printf("%d\n", staticDataArea[i++]);
                    }
                    printf("   ---  end of data   ---\n");
                }
                continue;
            }

            /* list */
            if (strcmp(input, commands[1]) == 0) {

                int temp = PC;
                PC = 0;
                listProgram(instructions,instrSize);
                PC = temp;
                printf(" --- end of list ---\n");
                continue;
            }

            /* breakpoint */
            if (strcmp(input, commands[2]) == 0) {
                if(breakpoint == -1)
                    printf("DEBUG [breakpoint]: cleared\n");
                else {
                    printf("DEBUG [breakpoint]: %d\n", breakpoint);
                }
                printf("DEBUG [breakpoint]: address to set, -1 to clear, <ret> for no change?\n");
                scanf("%s", input);
                int test = strtol(input, NULL,10);
                while(test == 0 && strcmp(input, "ret") != 0){
                    printf("Invalid command, try again!\n");
                    scanf("%s", input);
                    test = strtol(input, NULL, 10);
                }
                if(strcmp(input, "ret") == 0){
                    printf("DEBUG: no changes made\n", filename);
                    continue;
                } else {
                    if(test == -1){
                        breakpoint = test;
                        printf("DEBUG [breakpoint]: cleared\n");
                    } else {
                        breakpoint = test;
                        printf("DEBUG [breakpoint]: now set at %d\n", breakpoint);
                    }
                }
                continue;
            }

            /* step     */
            else if (strcmp(input, commands[3]) == 0) {
                makeDebugStep(instructions, numVars, 1);
                continue;
            }

            /* run      */
            else if (strcmp(input, commands[4]) == 0) {
                /*if no breakpoint was set */
                if(breakpoint == -1) {
                    executeProgram(instructions, numVars);
                    fclose(program);
                    printf("Ninja Virtual Machine stopped\n");
                    exit(EXIT_SUCCESS);
                } else if(breakpoint > 0) {
                    makeDebugStep(instructions, numVars, breakpoint - PC);
                } else {
                    printf("DEBUG [breakpoint]: set below zero, please set it above zero to continue!\n");
                }

            }
        }
    }

    /* debug = false ->> execute program normal */
    else {
        printf("Ninja Virtual Machine started\n");
        executeProgram(instructions, numVars);
    }

    /* finish execution */
    printf("Ninja Virtual Machine stopped\n");
    fclose(program);
}



/*
 * prints all instructions in programm
 */
void listProgram(unsigned int instructions [], int instrSize){

    while(PC < instrSize){
        listInstruction(instructions[PC]);
    }

}

/*
 * converts instruction to string and prints it
 */
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
        case CALL:
        {
            printf("CALL %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }case RET:
        {
            printf("BRT \n");
            PC++;
            break;
        }
        case DROP:
        {
            printf("DROP %d\n", (SIGN_EXTEND(instruction & 0x00FFFFFF)));
            PC++;
            break;
        }
        case PUSHR:
        {
            printf("BRT \n");
            PC++;
            break;
        }
        case POPR:
        {
            printf("BRT \n");
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

/*
 * executes all instructions
 */
void executeProgram(unsigned int instructions [], int staticDataArea_size){
    while(instructions[PC] != HALT){
        execInstruction(instructions[PC], staticDataArea_size);
    }
}

/*
 * executes n commands depending on the size of 'steps'
 */
void makeDebugStep(unsigned int instructions [], int staticDataArea_size, int steps) {
    while (instructions[PC] != HALT && steps != 0) {
        execInstruction(instructions[PC], staticDataArea_size);
        steps--;
    }
}

/*
 * executes single instruction
 */
void execInstruction(unsigned int instruction_binary, int staticDataArea_size){
    switch(instruction_binary >> 24){
        case PUSHC:
        {
            push(opcode(instruction_binary));
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
            int n = opcode(instruction_binary);
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
            int n = opcode(instruction_binary);
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
            int n = opcode(instruction_binary);
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
            int n = opcode(instruction_binary);
            push(stack[FP + n]);
            PC++;
            break;
        }
        case POPL:
        {
            int val = pop();
            stack[FP + opcode(instruction_binary)] = val;
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
            PC = opcode(instruction_binary);;
            break;
        }
        case BRF:
        {
            int jump = pop();
            if(jump == 0){
                PC = opcode(instruction_binary);;
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
            int jump = pop();
            if(jump == 1){
                PC = opcode(instruction_binary);
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
        case CALL:
        {
            int n = opcode(instruction_binary);
            push(PC + 1);
            PC = n;
            break;
        }case RET:
        {
            int n = pop();
            PC = n;
            break;
        }
        case DROP:
        {
            int n = opcode(instruction_binary);
            int i = 0;
            while(i < n){
                pop();
                i++;
            }
            PC++;
            break;
        }
        case PUSHR:
        {
            push(returnRegister.this[--returnRegister.size]);
            PC++;
            break;
        }
        case POPR:
        {
            returnRegister.this[returnRegister.size++] = pop();
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

int opcode(int binary) {
    return (SIGN_EXTEND(binary & 0x00FFFFFF));
}
