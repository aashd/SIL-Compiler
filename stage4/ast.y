%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define block_type 1
#define if_type 2
#define while_type 3
#define read_type 4 
#define write_type 5
#define assign_type 6
#define exp_type 7
#define id_type 8
#define num_type 9
int sym_tab[26];
extern int yylex();
int char_int(char a);
void yyerror(char* msg);
typedef struct tnode{
    int type;//nine types
    struct tnode* pt1;
    struct tnode*pt2;
    struct tnode* pt3;
    char ch;//for id temporary and ops in exp 
    int num_val;//For only num nodes
}node;
%}
%union{
    int i;
    char j; 
    struct tnode* t;
}
%token <j> ID
%token <i> NUM
%token IF THEN ELSE ENDIF WHILE ENDWHILE 
%token PRINT READ EXIT
%token EQ LEQ REQ
%type <t> E stmnt block  
%left '+'
%left '*'
%%

    prog:block '\n' {evaluate($1);}
        ;
    
    block:stmnt           {}
        |block stmnt     {} 
        ;
    
    stmnt:IF E THEN block ENDIF ';'     {}
        |IF E THEN block ELSE block ENDIF ';' {} 
        |WHILE E THEN block ENDWHILE ';'      {}
        |REA '(' ID ')' ';'    {}
        |WRI '(' E  ')'  ';'   {}
        |ID '=' E ';'          {}
        ;
    
    
    E: E '+' E  {}
        | E '*' E  {}
        | E '>' E {}
        | E '<' E {}
        | E EQ E {}
        | '(' E ')' {$$=$1;}
        | NUM  {$$=make_node(num_type,NULL,NULL,NULL,~,$1);}
        | ID   {$$=make_node(id_type,NULL,NULL,NULL,$1,-1);}
        ;

%%
node* make_node(int type,node* pt1,node* pt2, node* pt3, char ch , int num_val){
    i=(node*)malloc(sizeof(node));
    i->type=type;
    i->pt1=pt1;
    i->pt2=pt2;
    i->pt3=pt3;
    i->ch=ch;
    i->num_val=num_val;
    return i;
}
void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
int main(){
    yyparse();
    return 0;
}
int char_int(char a){
    int i=-1;
    i=a-'a'; 
    return i;
}
