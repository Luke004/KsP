/* Define Arguments */

#define ARG_VERSION "--version"
#define ARG_HELP "--help"
#define ARG_PROG1 "--prog1"
#define ARG_PROG2 "--prog2"
#define ARG_PROG3 "--prog3"
#define DEBUG_MODE "--debug"

/* VM Version Info */
#define NJVM_VERSION 3

/*make boolean type */
typedef int bool;
#define true 1
#define false 0

#define HALT 0
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10
#define PUSHG 11
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16
#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22
#define JMP 23
#define BRF 24
#define BRT 25

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))
#define STACK_SIZE 10000


void printHelp(void);
void loadProgram(const char filename[], int debug);
void listProgram(unsigned int prog [], int instrSize);
void executeProgram(unsigned int instructions [], int staticDataArea_size);
void execInstruction(unsigned int instruction, int staticDataArea_size);
void listInstruction(unsigned int instruction);
void makeDebugStep(unsigned int instructions [], int staticDataArea_size, int steps);
void push(int number);
int pop();