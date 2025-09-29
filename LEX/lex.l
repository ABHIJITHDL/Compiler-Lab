%{
#include <stdio.h>
#include <string.h>

/* Hardcoded input filename */
const char *input_filename = "sample.c";
%}

/* Keywords */
KEYWORDS int|float|double|char|if|else|while|for|return|void|struct

%%

"//".*              ;  // single-line comment (ignore)
"/*"([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*"*"+"/"   ;  // multi-line comment (ignore)
\"([^\\\"]|\\.)*\"   { printf("STRING: %s\n", yytext); }

{KEYWORDS}          { printf("KEYWORD: %s\n", yytext); }

[0-9]+\.[0-9]+      { printf("FLOAT: %s\n", yytext); }
[0-9]+              { printf("NUMBER: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*   { printf("IDENTIFIER: %s\n", yytext); }

"=="|"!="|"<="|">="|"<"|">"|"="|"+"|"-"|"*"|"/" { 
                      printf("OPERATOR: %s\n", yytext); 
                  }

";"|","|"("|")"|"{"|"}"|"["|"]" { 
                      printf("SEPARATOR: %s\n", yytext); 
                  }

[ \t\n]+            ;  // ignore whitespace

.                   { printf("UNKNOWN: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    FILE *fp = fopen(input_filename, "r");
    if (!fp) {
        perror("File open error");
        return 1;
    }
    yyin = fp;    // set the input for lexer
    yylex();
    fclose(fp);
    return 0;
}
