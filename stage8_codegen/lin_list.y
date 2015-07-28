%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sym_table.h"
#include  "ast.h"


gsymbol* head;////does it make it global?
char* one ;
char* zero;
int reg_act;//registers temp active
int lab_no;//labels active
int mem_act;//mem location active for binding new var
extern int yylex();
void yyerror(char* msg);
int positive_check(int a);
int type_E(node* a);
int type_id(char* a, gsymbol* head);


%}
%union{
    int i;
    char* j; 
    struct tnode* t;
}
%token <j> ID
%token <i> NUM BOOL_L
%token IF THEN ELSE ENDIF WHILE ENDWHILE DO DECL ENDDECL INT BOOL
%token PRINT READ EXIT
%token EQ  LEQ GEQ NEQ AND NOT OR
%type <t> E stmnt block  decl_list decl  decl_block int_decl bool_decl
%left  AND OR 
%left  EQ NEQ 
%left  '<' LEQ '>' GEQ
%left '+' '-'
%left '*' '/' '%'
%left NOT
%%

    prog:
        |prog decl_block  block  EXIT   {evaluate1($3,head);}
        |prog block EXIT                        {evaluate1($2,head);}
        ;

    decl_block: DECL  decl_list ENDDECL         {evaluate1($2,head);}
        ;


    decl_list:decl              {$$=make_node(decl_list_type,NULL,$1,NULL,head," ",-1);}//growing to the left tree
        |decl_list decl         {$$=make_node(decl_list_type,$1,$2,NULL,head," ",-1);}
        ;
    
    decl:  int_decl ';'       {$$=make_node(decl_type,$1,NULL,NULL,head," ",1);}//int => 1 bool => 0
        |  bool_decl ';'       {$$=make_node(decl_type,$1,NULL,NULL,head," ",0);}
        ;
    
    int_decl: INT ID            {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$2,1);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",1);//nomenclature changes id list struct all remains the same
                                }
        |  INT ID '['NUM']'     {
                                    node* i;
                                    node* j;
                                    j=make_node(num_type,NULL,NULL,NULL,head," ",$4);
                                    i=make_node(id_type,j,NULL,NULL,head,$2,1);
                                    positive_check($4);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",1);
                                }
        |  int_decl ',' ID      {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$3,1);
                                    $$=make_node(id_list_type,$1,i,NULL,head," ",1);
                                }
        |int_decl ',' ID '['NUM']'   {
                                    node* i;
                                    node* j;
                                    i=make_node(num_type,NULL,NULL,NULL,head," ",$5);
                                    j=make_node(id_type,i,NULL,NULL,head,$3,1);
                                    positive_check($5);
                                    $$=make_node(id_list_type,$1,j,NULL,head," ",1);
                                } 
        
        ;
    

    bool_decl: BOOL ID            {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$2,0);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",0);//nomenclature changes id list struct all remains the same
                                }
        |  BOOL ID '['NUM']'     {
                                    node* i;
                                    node* j;
                                    j=make_node(num_type,NULL,NULL,NULL,head," ",$4);
                                    i=make_node(id_type,j,NULL,NULL,head,$2,0);
                                    positive_check($4);
                                    $$=make_node(id_list_type,NULL,i,NULL,head," ",0);
                                }
        |  bool_decl ',' ID      {
                                    node* i;
                                    i=make_node(id_type,NULL,NULL,NULL,head,$3,0);
                                    $$=make_node(id_list_type,$1,i,NULL,head," ",0);
                                }
        |bool_decl ',' ID '['NUM']'   {
                                    node* i;
                                    node* j;
                                    i=make_node(num_type,NULL,NULL,NULL,head," ",$5);
                                    j=make_node(id_type,i,NULL,NULL,head,$3,0);
                                    positive_check($5);
                                    $$=make_node(id_list_type,$1,j,NULL,head," ",0);
                                } 
        ;
        

    
    block:stmnt           {$$=make_node(block_type,NULL,$1,NULL,head," ",-1);}
        |block stmnt     {$$=make_node(block_type,$1,$2,NULL,head," ",-1);} 
        ;
    
    stmnt:IF '(' E ')'THEN  block   ENDIF ';'                   {$$=make_node(if_type,$3,$6,NULL,head," ",-1);}
        |IF '(' E ')' THEN  block ELSE  block  ENDIF ';'    {$$=make_node(if_type,$3,$6,$8,head," ",-1);} 
        |WHILE '(' E ')' DO  block ENDWHILE ';'      {$$=make_node(while_type,$3,$6,NULL,head," ",-1);}
        |READ '(' ID ')' ';'                            {
                                                            if(type_id($3,head)==1){
                                                                node* j;
                                                                j=make_node(id_type,NULL,NULL,NULL,head,$3,-1);
                                                                $$=make_node(read_type,j,NULL,NULL,head," ",-1);
                                                            }
                                                            else{
                                                                yyerror("Read only for int type");
                                                            }
                                                        }
        |PRINT '(' E  ')'  ';'                          {
                                                            if(type_id($3,head)==1){
                                                                $$=make_node(write_type,$3,NULL,NULL,head," ",-1);
                                                            }
                                                            else{
                                                                yyerror("Read only for int type");
                                                            }
                                                            
                                                        }
        |ID '=' E ';'                                   {   
                                                            node* i;
                                                            //printf("makenode %s\n",$1);
                                                            i=make_node(id_type,NULL,NULL,NULL,head,$1,-1);
                                                            if(type_E($3)!=type_id($1,head)){
                                                               yyerror("ERROR: Types mismatch") ;
                                                            }
                                                            
                                                            $$=make_node(assign_type,i,$3,NULL,head," ",-1);
                                                        }
        | ID '[' E  ']'  '=' E ';'                      {
                                                            node* i; 
                                                            if(type_E($6)!=type_id($1,head)){
                                                               yyerror("ERROR: Types mismatch") ;
                                                            }
                                                            if(type_E($3)!=1){
                                                               yyerror("ERROR: Arrays expect Arithmetic expression not boolean") ;
                                                            }
                                                            
                                                            i=make_node(id_type,$3,NULL,NULL,head,$1,-1);
                                                            $$=make_node(assign_type,i,$6,NULL,head," ",-1);
                                                        } 
        |READ '(' ID '[' E  ']'  ')' ';'                {
                                                            node* j;
                                                            if(type_E($5)!=1){
                                                               yyerror("ERROR: Arrays expect Arithmetic expression not boolean") ;
                                                            }
                                                            j=make_node(id_type,$5,NULL,NULL,head,$3,-1);
                                                            $$=make_node(read_type,j,NULL,NULL,head," ",-1);
                                                        }
        ;
    
    
    E: E '+' E              {   
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                //printf("%d  %d",type_E($1),type_E($3));
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,one,add_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,add_op);
                                    yyerror("ERROR: add Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }
        | E '*' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,one,mul_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,mul_op);
                                    yyerror("ERROR: Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }
        | E '-' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,one,neg_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,neg_op);
                                    yyerror("ERROR: Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }
        | '-' E             {
                                if(type_E($2)==1){
                                    $$=make_node(exp_type,$2,NULL,NULL,head,one,neg_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,neg_op);
                                    yyerror("ERROR: Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }
        | E '/' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,one,div_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,mul_op);
                                    yyerror("ERROR: Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }
        | E '%' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,one,mod_op);
                                }
                                else{
                                    //$$=make_node(exp_type,$1,$3,NULL,head,zero,mul_op);
                                    yyerror("ERROR: Arithmetic ops expect only arithmetic expressions not boolean");
                                }
                            }


        | E '>' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,great_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        | E '<' E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,less_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        | E EQ E            {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,comp_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        | E LEQ E            {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,leq_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        | E GEQ E            {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,geq_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        
        | E NEQ E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==1){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,neq_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Relational ops can be performed over only arithmetic expressions");
                                }
                            }
        | E AND E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==0){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,and_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Logical ops can be performed over only boolean expressions");
                                }
                            }
        
        | E OR E           {
                                if(type_E($1)!=type_E($3)){   
                                    yyerror("ERROR: Type mismatch");
                                }
                                if(type_E($1)==0){
                                    $$=make_node(exp_type,$1,$3,NULL,head,zero,or_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Logical ops can be performed over only boolean  expressions");
                                }
                            }
        | NOT E           {
                                if(type_E($2)==0){
                                    $$=make_node(exp_type,$2,NULL,NULL,head,zero,not_op);//the logical operators always return bool
                                }
                                else{
                                    yyerror("ERROR: Logical ops can be performed over only boolean expressions");
                                }
                            }
        
        | '(' E ')'         {$$=$2;}
        | NUM               {
                                node* i;
                                i=make_node(num_type,NULL,NULL,NULL,head," ",$1);
                                $$=make_node(exp_type,i,NULL,NULL,head,one,no_op);
                            }///////looks fishy
        | ID                {
                                node* i;
                                int type_info;
                                type_info=type_id($1,head);
                                i=make_node(id_type,NULL,NULL,NULL,head,$1,type_info);
                                if(type_info==1){
                                    $$=make_node(exp_type,i,NULL,NULL,head,one,no_op);//int type
                                }
                                else{
                                    $$=make_node(exp_type,i,NULL,NULL,head,zero,no_op);//bool
                                }
                            }
        | ID '[' E  ']'     {
                                node* i;
                                int type_info;
                                type_info=type_id($1,head);
                                if(type_E($3)!=1){
                                    yyerror("ERROR: Array Indices cannot be bool type");
                                }
                                i=make_node(id_type,$3,NULL,NULL,head,$1,type_info);
                                if(type_info==1){
                                    $$=make_node(exp_type,i,NULL,NULL,head,one,no_op);
                                }
                                else{
                                    $$=make_node(exp_type,i,NULL,NULL,head,zero,no_op);
                                }
                            }
        | BOOL_L            {
                                node* i;
                                i=make_node(bool_type,NULL,NULL,NULL,head," ",$1);
                                $$=make_node(exp_type,i,NULL,NULL,head,zero,no_op);
                            }
        ;


%%
void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
int positive_check(int a){//no use redundant
    if(a>0){
        return 1;
    }
    else{
        yyerror("ERROR: Arrays shouls have positive size ");
        return 0;
    }

}
int type_id(char* a, gsymbol* head){//type of id
    gsymbol* ptr;
    ptr=search(a,head);
    if(ptr==NULL){
        yyerror("ERROR: Undeclared variable");
    }
    else{
        return ptr->type;
    }
}
int type_E(node* a){
    if(strcmp(a->str,"a1")==0){
        return 1;//one//int
    }
    else{
        return 0;
    }
}
int main(){
    head=make_syt_entry(NULL,-1,-1,0,NULL);
    reg_act=-1;
    lab_no=-1;
    mem_act=0;//iniatially 0
    zero=dyna_copy("a0");
    one=dyna_copy("a1");
    yyparse();
    return 0;
}
