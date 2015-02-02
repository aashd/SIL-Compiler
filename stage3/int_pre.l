%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "y.tab.h"
%}
%%
"print" {return PRI;}
"exit"  {return EXI;}
"read" {return REA;}
[0-9]+  {yylval.i=atoi(yytext);return NUM;}
[a-z]   {yylval.j=yytext[0];return ID;}
[+*\n();=] {return yytext[0]; }
%%
