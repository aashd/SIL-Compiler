%{
    #include<stdio.h>
    #include<stdlib.h>
    #include "y.tab.h"
%}



%%

[0-9]+  {yylval.i = atoi(yytext); return NUM;}
[+()*\n]  {return yytext[0];}
[\t\f\v ] ;
.           ;

%%
int yywrap(void){return 1;}
