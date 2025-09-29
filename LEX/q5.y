%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
%}

%token NUM
%left '+' '-'
%left '*' '/'

%%
E : E '+' E
  | E '-' E
  | E '*' E
  | E '/' E
  | '(' E ')'
  | NUM
  ;
%%

void yyerror(const char *s) {
    printf("Invalid expression\n");
}

int main() {
    printf("Enter expression: ");
    if (yyparse() == 0)
        printf("Valid expression\n");
    return 0;
}
