%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//AST stuff
#include "./ast.h"
//_______________


int sym_tab[26];
int regact;
int lab_no;
extern int yylex();
int char_int(char a);
void yyerror(char* msg);


%}
%union{
    int i;
    char j; 
    struct tnode* t;
}
%token <j> ID
%token <i> NUM
%token IF THEN ELSE ENDIF WHILE ENDWHILE DO
%token PRINT READ EXIT
%token EQ LEQ REQ
%type <t> E stmnt block  
%left  EQ 
%left '>' '<'
%left '+'
%left '*'
%%

    prog:
        |prog block '\n' {evaluate1($2);}
        ;
    
    block:stmnt '\n'          {$$=make_node(block_type,NULL,$1,NULL,'~',-1);}
        |block stmnt '\n'    {$$=make_node(block_type,$1,$2,NULL,'~',-1);} 
        ;
    
    stmnt:IF '(' E ')'THEN '\n' block   ENDIF ';'     {$$=make_node(if_type,$3,$7,NULL,'~',-1);}
        |IF '(' E ')' THEN '\n' block ELSE '\n' block  ENDIF ';' {$$=make_node(if_type,$3,$7,$10,'~',-1);} 
        |WHILE '(' E ')' DO '\n' block ENDWHILE ';'      {$$=make_node(while_type,$3,$7,NULL,'~',-1);}
        |READ '(' ID ')' ';'    {node* j;j=make_node(id_type,NULL,NULL,NULL,$3,-1);$$=make_node(read_type,j,NULL,NULL,'~',-1);}
        |PRINT '(' E  ')'  ';'   {
                                    $$=make_node(write_type,$3,NULL,NULL,'~',-1);
                                }
        |ID '=' E ';'          {node* i;i=make_node(id_type,NULL,NULL,NULL,$1,-1);$$=make_node(assign_type,i,$3,NULL,'~',-1);}
        ;
    
    
    E: E '+' E          {
                            $$=make_node(exp_type,$1,$3,NULL,'+',-1);
                        }
        | E '*' E       {
                            $$=make_node(exp_type,$1,$3,NULL,'*',-1);
                        }
        | E '>' E       {
                            $$=make_node(exp_type,$1,$3,NULL,'>',-1);
                        }
        | E '<' E       {
                            $$=make_node(exp_type,$1,$3,NULL,'<',-1);
                        }
        | E EQ E        {   
                            $$=make_node(exp_type,$1,$3,NULL,'=',-1);   
                        }
        | '(' E ')'     {$$=$2;}
        | NUM           {   node* i;
                            i=make_node(num_type,NULL,NULL,NULL,'~',$1);
                            $$=make_node(exp_type,i,NULL,NULL,'~',$1);
                        }
        | ID            {
                            node* i;
                            i=make_node(id_type,NULL,NULL,NULL,$1,-1);
                            $$=make_node(exp_type,i,NULL,NULL,'~',-1);
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
    regact=-1;
    lab_no=-1;  
    yyparse();
    return 0;
}
int char_int(char a){
    int i=-1;
    i=a-'a'; 
    return i;
}
