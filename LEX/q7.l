%{
#include "q7.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUM; }
[+\-*/()]   { return yytext[0]; }
\n          { return '\n'; }
[ \t]       ;   /* ignore spaces */

%%

int yywrap() {
    return 1;
}