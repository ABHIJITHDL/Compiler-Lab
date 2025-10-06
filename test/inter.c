#include<stdio.h>
#include<string.h>
#include<ctype.h>

char stack[100];
int top=-1;

void push(char c){
    stack[++top]=c;
}

int pop(){
    if(top<0){
        return -1;
    }
    return stack[top--];
}

int isOp(char c){
    return strchr("+-*/^",c)!=NULL;
}

int getPrec(char c){
    switch (c){
        case '+':
        case '-': return 1;
        case '*':
        case '/':return 2;
        case '^':return 3;
        default:return 0;
    }
}

int main(){
    char input[100];
    printf("Enter the expression: ");
    fgets(input,sizeof(input),stdin);
    input[strcspn(input,"\n")] = '\0';

    char* res = NULL;
    char* pos = strchr(input,'=');
    char* expression = input;
    if(pos!=NULL){
        *pos = '\0';
        res = input;
        expression = pos+1;
    }
    char post[100];
    int j=0;

    for(int i=0;i<strlen(expression);i++){
        char c = expression[i];
        if(isalnum(c)){
            post[j++] = c;
        }else if(c=='('){
            push(c);
        }else if(isOp(c)){
            while(top!=-1 && getPrec(stack[top])>=getPrec(c)){
                post[j++] = pop();
            }
            push(c);
        }else if(c==')'){
            while(stack[top]!='('){
                post[j++] = pop();
            }
            pop();
        }
    }

    while(top!=-1){
        post[j++] = pop();
    }

    post[j] = '\0';
    printf("Postfix: %s\n",post);

    char opstack[100][10];
    int t=0;
    int opt=-1;
    for(int i=0;i<strlen(post);i++){
        char c = post[i];
        if(isalnum(c)){
            char temp[10];
            temp[0] = c;
            temp[1] = '\0';
            strcpy(opstack[++opt],temp);
        }else{
            char op2[10], op1[10], temp_var[10];
            strcpy(op2,opstack[opt--]);
            strcpy(op1,opstack[opt--]);

            sprintf(temp_var,"t%d",t++);
            printf("%s = %s %c %s\n",temp_var,op1,c,op2);
            strcpy(opstack[++opt],temp_var);
        }
    }

    if(res != NULL){
        printf("%s = %s\n", res, opstack[opt]);
    }

    return 0;

}