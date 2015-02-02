%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>


int sym_table[26];
    
void yyerror(char *msg);
extern int yylex();
%}
%union {
    int i;       
}
%token <i> NUM
%type <i> E 
%left '+'
%left '*'
%%
  
   input:
    |input S
    | input '\n'   {exit(1);}
    ;
  
   S: E '\n'{printf("%d\n",$1);} 
    ;

   E: E '+' E {$$=$1+$3;}
    | E '*' E {$$=$1*$3;}
    | '(' E ')'{$$=$2;}
    | NUM  {$$=$1;}
    ;

%%

void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
int main(){
    yyparse();
    return 0;   
}
