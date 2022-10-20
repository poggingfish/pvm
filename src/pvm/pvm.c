/*

The PVM Compiler Toolchain

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pvm.h"
int pvmcall(int syscall_num){
    if (syscall_num == 0){
        printf("%d",stack[sp--]);
    }
    else if (syscall_num == 1){
        printf("%c",stack[sp--]);
    }
    else if (syscall_num == 2){
        int pos = stack[sp--];
        while (memory[pos] != 0){
            printf("%c",memory[pos]);
            pos++;
        }
    }
}
int exec(long long int *prog){
    int t;
    int y;
    int z;
    int x;
    int ACC = 0;
    int i = 0;
    while(prog[i] != END){
        int op = prog[i];
        switch (op)
        {
        case PUSHA:
            stack[++sp] = A;
            break;
        case PUSHB:
            stack[++sp] = B;
            break;
        case PUSHC:
            stack[++sp] = C;
            break;
        case PACC:
            stack[++sp] = ACC;
            break;
        case POPA:
            A = stack[sp--];
            break;
        case POPB:
            B = stack[sp--];
            break;
        case POPC:
            C = stack[sp--];
            break;
        case MOVA:
            A = prog[++i];
            break;
        case MOVB:
            B = prog[++i];
            break;
        case MOVC:
            C = prog[++i];
            break;
        case ADD:
            x = stack[sp--];
            y = stack[sp--];
            ACC = x+y;
            break;
        case SUB:
            x = stack[sp--];
            y = stack[sp--];
            ACC = y-x;
            break;
        case DEBUG:
            printf("DEBUG: %d\n", A);
            break;
        case MP:
            x = stack[sp--];
            stack[++sp] = memory[x];
            break;
        case ML:
            y = stack[sp--];
            x = stack[sp--];
            memory[y] = x;
            break;
        case SPC:
            i = stack[sp--];
            break;
        case SYSCALL:
            pvmcall(prog[++i]);
            break;
        case JMP:
            ReturnStack[++RSP] = i; 
            i = labels[stack[sp--]]-1;
            break;
        case SLBL:
            y = stack[sp--];
            x = stack[sp--];
            labels[x] = i+y;
            break;
        case RET:
            i = ReturnStack[RSP--];
            break;
        case JE:
            ReturnStack[++RSP] = i;
            x = stack[sp--];
            y = stack[sp--];
            t = stack[sp--];
            z = stack[sp--];
            if (t == z){
                i = labels[y]-1;
            }
            else{
                i = labels[x]-1;
            }
            break;
        default:
            printf("ERR: IDENT %d does not exist.\n", op);
            free(memory);
            exit(1);
        }
        i++;
    }
}
long long int* load(char * file){
    int ptr = 0;
    FILE* fileptr = fopen(file,"r");
    fseek(fileptr, 0, SEEK_END);
    long fsize = ftell(fileptr);
    fseek(fileptr, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, fileptr);
    char *token = strtok(string, "\n");
    while (token != NULL)
    {
        char *end_token;
        char *args[5];
        int argsptr = 0;
        char *token2 = strtok_r(token, " ", &end_token);
        while (token2 != NULL){
            args[argsptr++] = token2;
            token2 = strtok_r(NULL, " ", &end_token);
        }
        char *op = args[0];
        if (!strcmp(op, "MOVA")){
            prog_hold.prog[ptr++] = MOVA;
            prog_hold.prog[ptr++] = atoi(args[1]);
        }
        else if (!strcmp(op, "MOVB")){
            prog_hold.prog[ptr++] = MOVB;
            prog_hold.prog[ptr++] = atoi(args[1]);
        }
        else if (!strcmp(op, "MOVC")){
            prog_hold.prog[ptr++] = MOVC;
            prog_hold.prog[ptr++] = atoi(args[1]);
        }
        else if (!strcmp(op, "PUSHA")){
            prog_hold.prog[ptr++] = PUSHA;
        }
        else if (!strcmp(op, "PUSHB")){
            prog_hold.prog[ptr++] = PUSHB;
        }
        else if (!strcmp(op, "PUSHC")){
            prog_hold.prog[ptr++] = PUSHC;
        }
        else if (!strcmp(op, "PACC")){
            prog_hold.prog[ptr++] = PACC;
        }
        else if (!strcmp(op, "ADD")){
            prog_hold.prog[ptr++] = ADD;
        }
        else if (!strcmp(op, "SUB")){
            prog_hold.prog[ptr++] = SUB;
        }
        else if (!strcmp(op, "POPA")){
            prog_hold.prog[ptr++] = POPA;
        }
        else if (!strcmp(op, "POPB")){
            prog_hold.prog[ptr++] = POPB;
        }
        else if (!strcmp(op, "POPC")){
            prog_hold.prog[ptr++] = POPC;
        }
        else if (!strcmp(op, "DEBUG")){
            prog_hold.prog[ptr++] = DEBUG;
        }
        else if (!strcmp(op, "ML")){
            prog_hold.prog[ptr++] = ML;
        }
        else if (!strcmp(op, "MP")){
            prog_hold.prog[ptr++] = MP;
        }
        else if (!strcmp(op, "SPC")){
            prog_hold.prog[ptr++] = SPC;
        }
        else if (!strcmp(op, "SYSCALL")){
            prog_hold.prog[ptr++] = SYSCALL;
            prog_hold.prog[ptr++] = atoi(args[1]);
        }
        else if (!strcmp(op, "lbl")){
            labels[atoi(args[1])] = ptr;
        }
        else if (!strcmp(op, "SLBL")){
            prog_hold.prog[ptr++] = SLBL;
        }
        else if (!strcmp(op, "JMP")){
            prog_hold.prog[ptr++] = JMP;
        }
        else if (!strcmp(op, "RET")){
            prog_hold.prog[ptr++] = RET;
        }
        else if (!strcmp(op, "JE")){
            prog_hold.prog[ptr++] = JE;
        }
        else{
            printf("ERR: Unknown command: %s\n", op);
            exit(1);
        }
        token2 = NULL;
        token = strtok(NULL, "\n");
    }
    free(string);
    prog_hold.prog[ptr++] = END;
    fclose(fileptr);
    return prog_hold.prog;
}
int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Usage: ./pvm <File>\n");
        exit(1);
    }
    printf("PVM: Allocating ~50KB.\n");
    memory = malloc(1024*50);
    load(argv[1]);
    exec(prog_hold.prog);
    free(memory);
}