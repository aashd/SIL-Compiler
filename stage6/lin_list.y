%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//AST stuff
#define block_type 1
#define if_type 2
#define while_type 3
#define read_type 4 
#define write_type 5
#define assign_type 6
#define exp_type 7
#define id_type 8
#define num_type 9
#define decl_type 10
#define decl_list_type 11

typedef struct tnode{
    int type;//nine types
    struct tnode* pt1;
    struct tnode*pt2;
    struct tnode* pt3;
    char* str;//for id temporary and ops in exp 
    int num_val;//For only num nodes
}node;
node* make_node(int type,node* pt1,node* pt2, node* pt3, char* a , int num_val);
int evaluate(node* i);
//_______________


extern int yylex();
int char_int(char a);
void yyerror(char* msg);


%}
%union{
    int i;
    char* j; 
    struct tnode* t;
}
%token <j> ID
%token <i> NUM
%token IF THEN ELSE ENDIF WHILE ENDWHILE DO DECL ENDDECL INT
%token PRINT READ EXIT
%token EQ LEQ REQ
%type <t> E stmnt block  decl_list decl
%left  EQ 
%left '>' '<'
%left '+'
%left '*'
%%

    prog:
        |DECL '\n' decl_list ENDDECL  block '\n'  {evaluate($3);evaluate($6);}
        |prog block '\n' {evaluate($2);}
        ;

    decl_list:decl  '\n'        {$$=make_node(decl_list_type,NULL,$1,NULL,NULL," ",-1);}
        |decl_list decl '\n'    {$$=make_node(decl_list_type,$1,$2,NULL,NULL," ",-1);}
        ;
    
    decl: INT ID ';'        {node* i;i=make_node(id_type,NULL,NULL,NULL,NULL,$2,-1);$$=make_node(decl_type,i,NULL,NULL," ",-1);}
        | INT ID '['NUM']'';' {node* i;
                                node* j;
                                i=make_node(id_type,NULL,NULL,NULL,NULL,$2,-1);
                                j=make_node(id_type,NULL,NULL,NULL,NULL," ",$4);
                                $$=make_node(decl_type,i,j,NULL,NULL," ",-1);}
        ;
    
    block:stmnt '\n'          {$$=make_node(block_type,NULL,$1,NULL,NULL," ",-1);}
        |block stmnt '\n'    {$$=make_node(block_type,$1,$2,NULL,NULL," ",-1);} 
        ;
    
    stmnt:IF '(' E ')'THEN '\n' block   ENDIF ';'     {$$=make_node(if_type,$3,$7,NULL," ",NULL,-1);}
        |IF '(' E ')' THEN '\n' block ELSE '\n' block  ENDIF ';' {$$=make_node(if_type,$3,$7,$10,NULL," ",-1);} 
        |WHILE '(' E ')' DO '\n' block ENDWHILE ';'      {$$=make_node(while_type,$3,$7,NULL,NULL," ",-1);}
        |READ '(' ID ')' ';'    {node* j;j=make_node(id_type,NULL,NULL,NULL,$3,-1);$$=make_node(read_type,j,NULL,NULL,NULL," ",-1);}
        |PRINT '(' E  ')'  ';'   {$$=make_node(write_type,$3,NULL,NULL,NULL," ",-1);}
        |ID '=' E ';'          {node* i;printf("makenode %s\n",$1);i=make_node(id_type,NULL,NULL,NULL,NULL,$1,-1);$$=make_node(assign_type,i,$3,NULL,NULL," ",-1);}
        ;
    
    
    E: E '+' E  {$$=make_node(exp_type,$1,$3,NULL,NULL,"+",-1);}
        | E '*' E  {$$=make_node(exp_type,$1,$3,NULL,NULL,"*",-1);}
        | E '>' E {$$=make_node(exp_type,$1,$3,NULL,NULL,">",-1);}
        | E '<' E {$$=make_node(exp_type,$1,$3,NULL,NULL,"<",-1);}
        | E EQ E {$$=make_node(exp_type,$1,$3,NULL,NULL,"==",-1);}
        | '(' E ')' {$$=$2;}
        | NUM  {$$=make_node(num_type,NULL,NULL,NULL,NULL," ",$1);}
        | ID   {$$=make_node(id_type,NULL,NULL,NULL,NULL,$1,-1);}
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
int char_int(char a){
    int i=-1;
    i=a-'a'; 
    return i;
}
