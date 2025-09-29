%{
#include <stdio.h>
int chars=0, words=0, lines=0;
%}

%%
\n          { lines++; chars++; }
[ \t]+      { words++; chars+=yyleng; }
[^ \t\n]+   { chars+=yyleng; }
%%

int main() {
    yylex();
    printf("Lines: %d\nWords: %d\nCharacters: %d\n", lines, words+1, chars);
    return 0;
}
