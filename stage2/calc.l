%{
    #include<stdio.h>
    #include<stdlib.h>
    #include "y.tab.h"
%}



%%

[0-9]+  {yylval.i = atoi(yytext); return NUM;}
[+()*]  {return yytext[0];}
[\t\f\v\n ] ;
.           ;

%%
int yywrap(void){return 1;}
