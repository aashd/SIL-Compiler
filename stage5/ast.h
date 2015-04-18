#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#define block_type 1
#define if_type 2
#define while_type 3
#define read_type 4 
#define write_type 5
#define assign_type 6
#define exp_type 7
#define id_type 8
#define num_type 9

typedef struct tnode{
    int type;//nine types
    struct tnode* pt1;
    struct tnode*pt2;
    struct tnode* pt3;
    char ch;//for id temporary and ops in exp 
    int num_val;//For only num nodes
}node;
node* make_node(int type,node* pt1,node* pt2, node* pt3, char a , int num_val);
int evaluate(node* i,FILE* fp);
int evaluate1(node* i);
int free_reg(int y);
int assign_lab();
int assign_reg();
int sym_tab[26];
int regact;
int lab_no;
