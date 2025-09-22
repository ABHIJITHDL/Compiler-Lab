#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int regCount = 0;
char reg[4][5] = {"AX", "BX", "CX", "DX"};
char regVar[4][10] = {"", "", "", ""}; // Track which variable is in each register

struct tac {
    char op[10];
    char op1[10]; 
    char op2[10];
    char res[10];
};

int getReg(char* var) {
    // First check if variable is already in a register
    for (int i = 0; i < 4; i++) {
        if (strcmp(regVar[i], var) == 0) {
            return i;
        }
    }
    
    // If not found, allocate a new register
    if (regCount < 4) {
        int newReg = regCount++;
        printf("MOV %s, %s\n", reg[newReg], var);
        strcpy(regVar[newReg], var);
        return newReg;
    }
    
    // If all registers used, reuse AX
    printf("MOV %s, %s\n", reg[0], var);
    strcpy(regVar[0], var);
    return 0;
}

void generateCode(struct tac t) {
    printf("// %s = %s %s %s\n", t.res, t.op1, t.op, t.op2);
    
    int r1 = getReg(t.op1);
    
    switch(t.op[0]) {
        case '+':
            printf("ADD %s, %s\n", reg[r1], t.op2);
            break;
        case '-':
            printf("SUB %s, %s\n", reg[r1], t.op2);
            break;
        case '*':
            printf("MUL %s, %s\n", reg[r1], t.op2);
            break;
        case '/':
            printf("DIV %s, %s\n", reg[r1], t.op2);
            break;
    }
    
    printf("MOV %s, %s\n", t.res, reg[r1]);
    strcpy(regVar[r1], t.res);
    printf("\n");
}

int main() {
    FILE *fp = fopen("tac.txt", "r");
    if (fp == NULL) {
        printf("Error: cannot open tac.txt\n");
        return 1;
    }

    char line[MAX];
    printf("Assembly Code Generation:\n");
    printf("========================\n\n");
    
    while (fgets(line, sizeof(line), fp)) {
        struct tac t;
        sscanf(line, " %s = %s %s %s", t.res, t.op1, t.op, t.op2);
        generateCode(t);
    }

    fclose(fp);
    return 0;
}