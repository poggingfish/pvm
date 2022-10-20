typedef enum ops{
    PUSHA,
    PUSHB,
    PUSHC,
    PACC,
    POPA,
    POPB,
    POPC,
    MOVA,
    MOVB,
    MOVC,
    ADD,
    DEBUG,
    END,
    SUB,
    MP,
    ML,
    SPC,
    SYSCALL,
    JMP,
    SLBL,
    RET,
    JE
} ops;
int exec(long long int *prog);
struct prog_hold{
    long long int prog[10000];
} prog_hold;
long long int* load(char * file);
int labels[51200];
long long int* memory;
int stack[1000];
int sp = 0;
int A = 0;
int B = 0;
int C = 0;
int ReturnStack[1000];
int RSP = 0;