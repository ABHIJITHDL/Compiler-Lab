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
input : /* empty */
      | input line
      ;

line  : E '\n'    { printf("= %d\n", $1); }
      ;

E : E '+' E   { $$ = $1 + $3; }
  | E '-' E   { $$ = $1 - $3; }
  | E '*' E   { $$ = $1 * $3; }
  | E '/' E   { $$ = $1 / $3; }
  | '(' E ')' { $$ = $2; }
  | NUM       { $$ = $1; }
  ;
%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
