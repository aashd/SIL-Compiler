%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

typedef struct node{
    int flag;
    int ival;
    char op;
    struct node* left;
    struct node* right;
    struct node * parent;
}tnode;
tnode* mknode(char* str, tnode* left_p, tnode* right_p);
tnode* mkleaf(int a);
int isleaf(tnode* a);
int prefix(tnode* a);
int infix(tnode* a);
int postfix(tnode* a);

void yyerror(char *msg);
char* concat(char* a,char*b);
char* dyna_copy(char* a);
extern int yylex();
%}
%union {
    struct node* t;
    int i;       
}
%token <i> NUM
%type <t> E
%left '+'
%left '*'
%%
S : E   {prefix($1);}

E : E '+' E {$$=mknode("+",$1,$3);}
  | E '*' E  {$$=mknode("*",$1,$3);} 
  | '(' E ')' {$$= $2;}
  | NUM    {$$=mkleaf($1);}
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
