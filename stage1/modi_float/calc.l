%{
    #include<stdio.h>
    #include<stdlib.h>
    #include "y.tab.h"
%}



%%

[0-9]+\.[0-9]+  {yylval.i = atof(yytext); return NUM;}
[0-9]+          {yylval.i = atof(yytext); return NUM;}
[+()*\n]  {return yytext[0];}
[\t\f\v ] ;
.          {yyerror("syntax error\n");} 

%%
int yywrap(void){return 1;}
