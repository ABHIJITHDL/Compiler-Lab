%{
#include <stdio.h>
%}

%{
#include <stdio.h>
%}

%%
"int"|"float"|"return"|"if" { printf("Keyword: %s\n", yytext); }
[0-9]+      { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("Identifier: %s\n", yytext); }
"+"|"-"|"*"|"/"|"="|"=="|"!="|"<"|">"|"<="|">=" { printf("Operator: %s\n", yytext); }
"("|")"|"{"|"}"|"["|"]"|";"|","|"." { printf("Separator: %s\n", yytext); }
\n          { /* ignore newline */ }
[ \t]       { /* ignore spaces */ }
.           { printf("Unknown: %s\n", yytext); }
%%

int main() {
    yylex();
    return 0;
}
