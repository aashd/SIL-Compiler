%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "y.tab.h"
char*  dyna_copy(char* a);
%}
%%
"if"                {return IF;}
"then"              {return THEN;}
"endif"             {return ENDIF;}
"else"              {return ELSE;}
"while"             {return WHILE;}
"do"                {return DO;}
"endwhile"          {return ENDWHILE;}
"decl"              {return DECL;}
"int"               {return INT;}
"bool"              {return BOOL;}
"enddecl"           {return ENDDECL;}
"print"             {return PRINT;}
"exit"              {return EXIT;}
"read"              {return READ;}
"=="                {return EQ;}
"<="                {return LEQ;}
">="                {return GEQ;}
"!="                {return NEQ;}
"AND"               {return AND;}
"OR"                {return OR;}
"NOT"               {return NOT;}
"TRUE"              {
                        yylval.i=1;
                        return BOOL_L;
                    }
"FALSE"             {
                        yylval.i=0;
                        return BOOL_L;
                    }
[0-9]+              {
                        yylval.i=atoi(yytext);  
                        //printf("in lex %s",yytext);
                        return NUM;
                    }
[a-z]+              {
                        yylval.j=dyna_copy(yytext);
                        //printf("in lex %s\n",yytext);
                        return ID;
                    }
[+*><();=,\[\]/\-%]        {return yytext[0]; }
[\t \n]    {;}
.  {yyerror("lexical error");}
%%
