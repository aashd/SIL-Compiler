%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "y.tab.h"
%}
%%
"if"    {return IF;}
"then"  {return THEN;}
"endif" {return ENDIF;}
"else"  {return ELSE;}
"while" {return WHILE;}
"do"    {return DO;}
"endwhile" {return ENDWHILE;}
"print" {return PRINT;}
"exit"  {return EXIT;}
"read" {return READ;}
"=="   {return EQ;}
[0-9]+  {yylval.i=atoi(yytext);return NUM;}
[a-z]   {yylval.j=yytext[0];return ID;}
[+*><\n();=] {return yytext[0]; }
[\t ]    {;}
.  {yyerror("lexical error");}
%%
