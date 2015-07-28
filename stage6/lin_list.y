%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sym_table.h"
#include  "ast.h"


gsymbol* head;////does it make it global?
extern int yylex();
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
%token EQ 
%type <t> E stmnt block  decl_list decl id_list 
%left  EQ 
%left '>' '<'
%left '+'
%left '*'
%%

    prog:
        |DECL  decl_list ENDDECL  block  EXIT  {evaluate($2);evaluate($4);}
        |prog block EXIT {evaluate($2,head);}
        ;

    decl_list:decl          {$$=make_node(decl_list_type,NULL,$1,NULL,head," ",-1);}//growing to the left tree
        |decl_list decl     {$$=make_node(decl_list_type,$1,$2,NULL,head," ",-1);}
        ;
    
    decl: INT id_list ';'        {$$=make_node(decl_type,$2,NULL,NULL,head," ",-1);}
        ;
    
    id_list: ID                 {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$1,-1);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",-1);
                                }   
        |  ID '['NUM']'         {
                                    node* i;
                                    node* j;
                                    j=make_node(num_type,NULL,NULL,NULL,head," ",$3);
                                    i=make_node(id_type,j,NULL,NULL,head,$1,-1);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",-1);
                                }
        |id_list ',' ID             {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$3,-1);
                                    $$=make_node(id_list_type,$1,i,NULL,head," ",-1);
                                }
        |id_list ',' ID '['NUM']'   {
                                    node* i;
                                    node* j;
                                    i=make_node(num_type,NULL,NULL,NULL,head," ",$5);
                                    j=make_node(id_type,i,NULL,NULL,head,$3,-1);
                                    $$=make_node(id_list_type,$1,j,NULL,head," ",-1);
                                } 
        ;
    
    block:stmnt           {$$=make_node(block_type,NULL,$1,NULL,head," ",-1);}
        |block stmnt     {$$=make_node(block_type,$1,$2,NULL,head," ",-1);} 
        ;
    
    stmnt:IF '(' E ')'THEN  block   ENDIF ';'                   {$$=make_node(if_type,$3,$6,NULL,head," ",-1);}
        |IF '(' E ')' THEN  block ELSE  block  ENDIF ';'    {$$=make_node(if_type,$3,$6,$8,head," ",-1);} 
        |WHILE '(' E ')' DO  block ENDWHILE ';'      {$$=make_node(while_type,$3,$6,NULL,head," ",-1);}
        |READ '(' ID ')' ';'                            {
                                                            node* j;
                                                            j=make_node(id_type,NULL,NULL,NULL,head,$3,-1);
                                                            $$=make_node(read_type,j,NULL,NULL,head," ",-1);
                                                        }
        |PRINT '(' E  ')'  ';'                          {
                                                            $$=make_node(write_type,$3,NULL,NULL,head," ",-1);
                                                            
                                                        }
        |ID '=' E ';'                                   {   
                                                            node* i;
                                                            //printf("makenode %s\n",$1);
                                                            i=make_node(id_type,NULL,NULL,NULL,head,$1,-1);
                                                            $$=make_node(assign_type,i,$3,NULL,head," ",-1);
                                                        }
        | ID '[' E  ']'  '=' E ';'                      {
                                                            node* i; 
                                                            i=make_node(id_type,$3,NULL,NULL,head,$1,-1);
                                                            $$=make_node(assign_type,i,$6,NULL,head," ",-1);
                                                        } 
        |READ '(' ID '[' E  ']'  ')' ';'                {
                                                            node* j;
                                                            j=make_node(id_type,$5,NULL,NULL,head,$3,-1);
                                                            $$=make_node(read_type,j,NULL,NULL,head," ",-1);
                                                        }
        ;
    
    
    E: E '+' E              {$$=make_node(exp_type,$1,$3,NULL,head," ",add_op);}
        | E '*' E           {$$=make_node(exp_type,$1,$3,NULL,head," ",mul_op);}
        | E '>' E           {$$=make_node(exp_type,$1,$3,NULL,head," ",great_op);}
        | E '<' E           {$$=make_node(exp_type,$1,$3,NULL,head," ",less_op);}
        | E EQ E            {$$=make_node(exp_type,$1,$3,NULL,head," ",comp_op);}
        | '(' E ')'         {$$=$2;}
        | NUM               {$$=make_node(num_type,NULL,NULL,NULL,head," ",$1);}///////looks fishy
        | ID                {$$=make_node(id_type,NULL,NULL,NULL,head,$1,-1);}
        | ID '[' E  ']'     {
                                $$= make_node(id_type,$3,NULL,NULL,head,$1,-1);
                            }
        ;

%%
void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
int main(){
    head=make_syt_entry(NULL,-1,-1,NULL,NULL);
    yyparse();
    return 0;
}
