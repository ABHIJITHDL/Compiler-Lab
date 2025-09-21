#include<stdio.h>
#include<string.h>

int main(){
    char input[100],nonTerm;
    char alpha[10][10],beta[10][10];
    int a=0,b=0;

    printf("Enter the production as E->a|b\n");
    fgets(input,sizeof(input),stdin);
    input[strcspn(input,"\n")] = 0;

    nonTerm=input[0];
    char *rhs = strstr(input,"->")+2;

    char *token = strtok(rhs,"|");

    while(token!=NULL){
        if(token[0] == nonTerm){
            strcpy(alpha[a],token+1);
            a++;
        }else{
            strcpy(beta[b],token);
            b++;
        }
        token = strtok(NULL,"|");
    }

    if(a==0){
        printf("No left recursion\n");
    }else{
        printf("After removing left recursion\n");

        printf("%c->",nonTerm);
        for(int i=0;i<b;i++){
            printf("%s%c'",beta[i],nonTerm);
            if(i<b-1){
                printf(" | ");
            }
        }

        printf("\n%c'->",nonTerm);
        for(int i=0;i<a;i++){
            printf("%s%c'",alpha[i],nonTerm);
                printf(" | ");
        }
        printf("#\n");
    }
    return 0;
}