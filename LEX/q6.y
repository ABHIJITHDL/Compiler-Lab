%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
%}

%token LETTER DIGIT

%%
S : LETTER T ;
T : LETTER T
  | DIGIT T
  | /* empty */
  ;
%%

void yyerror(const char *s) {
    printf("Invalid identifier\n");
}

int main() {
    printf("Enter identifier: ");
    yyparse();
    printf("Valid identifier\n");
    return 0;
}
