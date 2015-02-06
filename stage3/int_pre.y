%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int sym_tab[26];
extern int yylex();
int char_int(char a);
void yyerror(char* msg);
%}
%union{
    int i;
    char j; 
}
%token <j> ID
%token <i> NUM
%token PRI
%token REA
%token EXI
%type <i> E 
%left '+'
%left '*'
%%


    input:
        |input stmnt '\n'
        |input '\n'   {;}
        ;


    stmnt: PRI '(' E ')' ';'  { printf("%d\n",$3); }
        | REA '(' ID  ')' ';'{int x,j; j=char_int($3);scanf("%d",&x); sym_tab[j]=x;}
        | EXI ';'            {exit(1);}
        | ID '=' E ';'           {sym_tab[char_int($1)]=$3;}
        ;

       E: E '+' E  {$$=$1+$3;}
        | E '*' E  {$$=$1*$3;}
        | '(' E ')' {$$=$2;}
        | NUM  {$$=$1;}
        | ID   {
                int j;
                j=sym_tab[char_int($1)];
                if(j>=0){
                    $$=j;
                }
                else{
                    yyerror("Uninitialised variable");
                }
                }
        ;
%%
void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
int main(){
    int i;
    for(i=0;i<26;i++){
        sym_tab[i]=-1; 
    }
    yyparse();
    return 0;
}
int char_int(char a){
    int i=-1;
    i=a-'a'; 
    return i;
}
