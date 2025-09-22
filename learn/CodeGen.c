#include<stdio.h>
#include<ctype.h>
#include<string.h>

int regCount = 0;
char reg[4][5] = {"AX","BX","CX","DX"};
char regVar[4][10] = {"","","",""};

struct tac {
    char res[10];
    char op1[10];
    char op2[10];
    char op[10];
};



int getReg(char* c){
    for(int i=0;i<4;i++){
        if(strcmp(regVar[i],c)==0){
            return i;
        }
    }

    if(regCount<4){
        printf("MOV %s,%s\n",reg[regCount],c);
        strcpy(regVar[regCount],c);
        return regCount++;
    }

    printf("MOV %s,%s\n",reg[0],c);
    strcpy(regVar[0],c);
    return 0;

}

void codeGen(struct tac t){
    int regI = getReg(t.op1);

    switch(t.op[0]){
        case '+':   printf("ADD %s,%s\n",reg[regI],t.op2);
                break;
        case '-':   printf("SUB %s,%s\n",reg[regI],t.op2);
                break;
        case '*':   printf("MUL %s,%s\n",reg[regI],t.op2);
                break;
        case '/':   printf("DIV %s,%s\n",reg[regI],t.op2);
                break;
        default:   printf("Invalid operator\n");
                break;

    }
    printf("MOV %s,%s\n",t.res,reg[regI]);
    strcpy(regVar[regI],t.res);
}

int main(){

    FILE *fp = fopen("tac.txt","r");
    char line[100];

    while(fgets(line,sizeof(line),fp)){
        struct tac t;
        sscanf(line,"%s = %s %s %s",t.res,t.op1,t.op,t.op2);
        codeGen(t);
    }
    fclose(fp);
    return 0;
}