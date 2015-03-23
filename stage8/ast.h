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
#define decl_type 10
#define decl_list_type 11
#define id_list_type 12  
#define bool_type 13

#define add_op 1
#define mul_op 2
#define div_op 12
#define neg_op 13
#define mod_op 14   
#define less_op 3
#define great_op 4
#define comp_op 5
#define neq_op 11
#define leq_op 6
#define geq_op 7
#define and_op 8
#define or_op 9
#define not_op 10
#define no_op 15

typedef struct tnode{
    int type;//thirteen types
    struct tnode* pt1;
    struct tnode*pt2;
    struct tnode* pt3;
    char* str;//for id in id type and int/bool in expressions  
    int num_val;//For only num nodes and ops inexp and int/bool in all declarations
    gsymbol* g_sym_tab;//pointer to head node of glob sym table
}node;
node* make_node(int type,node* pt1,node* pt2, node* pt3, gsymbol* gst,char* a , int num_val);
int evaluate(node* i,gsymbol* head);
char* dyna_copy(char* a);
