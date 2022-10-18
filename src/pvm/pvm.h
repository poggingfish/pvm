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
} ops;
int exec(long long int *prog);
struct prog_hold{
    long long int prog[10000];
} prog_hold;
long long int* load(char * file);