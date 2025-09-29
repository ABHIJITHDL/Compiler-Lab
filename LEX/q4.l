%{
#include <stdio.h>
int v=0, c=0;
%}

%%
[aeiouAEIOU] { v++; }
[a-zA-Z]     { c++; }
.|\n         { ; }
%%

int main() {
    yylex();
    printf("Vowels: %d\nConsonants: %d\n", v, c);
    return 0;
}
