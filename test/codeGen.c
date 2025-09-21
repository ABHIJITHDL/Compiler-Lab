#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int regLoc[26];                
char reg[4][5] = {"AX","BX","CX","DX"};
int regStatus[4] = {0,0,0,0};  

struct tac {
    char op[10];
    char op1[10];
    char op2[10];
    char res[10];
};

int getReg(char* c) {
    int idx = c[0] - 'a';
    if(regLoc[idx] != -1) { 
        return regLoc[idx];  
    }
    for(int i=0;i<4;i++) {
        if(regStatus[i] == 0) {
            regStatus[i] = 1;
            regLoc[idx] = i;
            printf("MOV %s, %s\n", reg[i], c); 
            return i;
        }
    }
    return -1; 
}

void generateCode(struct tac t) {
    int op1Reg = getReg(t.op1);

    switch(t.op[0]) {
        case '+': printf("ADD %s, %s\n", reg[op1Reg], t.op2); break;
        case '-': printf("SUB %s, %s\n", reg[op1Reg], t.op2); break;
        case '*': printf("MUL %s, %s\n", reg[op1Reg], t.op2); break;
        case '/': printf("DIV %s, %s\n", reg[op1Reg], t.op2); break;
        default : printf("Invalid operation\n");
    }

    int resIdx = t.res[0] - 'a';
    if (resIdx != t.op1[0]-'a') {
        printf("MOV %s, %s\n", t.res, reg[op1Reg]);
    }
    regLoc[resIdx] = op1Reg;

    if (resIdx != t.op1[0]-'a') {
        regLoc[t.op1[0]-'a'] = -1;
    }

    printf("\n");
}2


int main() {
    for(int i=0;i<26;i++) regLoc[i] = -1;

    FILE *fp = fopen("tac.txt", "r");
    if(fp == NULL) {
        printf("Error: cannot open tac.txt\n");
        return 1;
    }

    char line[MAX];
    while(fgets(line, sizeof(line), fp)) {
        struct tac t;
        sscanf(line, " %s = %s %s %s", t.res, t.op1, t.op, t.op2);
        generateCode(t);
    }

    fclose(fp);
    return 0;
}
