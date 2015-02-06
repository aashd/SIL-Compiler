
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


typedef struct tnode{
    int type;//nine types
    struct tnode* pt1;
    struct tnode*pt2;
    struct tnode* pt3;
    char ch;//for id temporary and ops in exp 
    int num_val;//For only num nodes
}node;
node* make_node(int type,node* pt1,node* pt2, node* pt3, char a , int num_val);
int evaluate(node* i);
int sym_tab[26];

node* make_node(int type,node* pt1,node* pt2, node* pt3, char a , int num_val){
    node* i;
    i=(node*)malloc(sizeof(node));
    i->type=type;
    i->pt1=pt1;
    i->pt2=pt2;
    i->pt3=pt3;
    i->ch=a;
    i->num_val=num_val;
    return i;
}
int evaluate(node* i){
    if(i==NULL){
        printf("EVAL tying to eval  NULL\n");
        exit(1);
    }
    int x;
    switch(i->type){
        case num_type:
            return i->num_val;
            break;
        case id_type:
            return sym_tab[char_int(i->ch)];// check of initialised /uninitialised
            break;
        case exp_type:
            switch(i->ch){
                case '+':
                    return evaluate(i->pt1)+evaluate(i->pt2);
                    break;
                case '*':
                    return evaluate(i->pt1)*evaluate(i->pt2);
                    break;
                case '>':
                    return evaluate(i->pt1)>evaluate(i->pt2);
                case '<':
                    return evaluate(i->pt1)<evaluate(i->pt2);
                    break;
                case '=':
                    return evaluate(i->pt1)==evaluate(i->pt2);
                    break;
                default:
                    printf("exp_typr error\n");
            }
            break;
        case write_type:
            printf("%d\n",evaluate(i->pt1));
            break;
        case read_type:
            scanf("%d",&x);
            sym_tab[char_int((i->pt1)->ch)]=x;
            return 0;
            break;
        case assign_type:
            sym_tab[char_int((i->pt1)->ch)]=evaluate(i->pt2);

            return 0;
            break;
        case if_type:
            if(evaluate(i->pt1)){
                evaluate(i->pt2);
            }
            if(i->pt3!=NULL){
                evaluate(i->pt3);
            }
            return 0;
            break;
        case while_type:
            while(evaluate(i->pt1)){
                evaluate(i->pt2);
            }
            return 0;
            break;
        case block_type:
            if(i->pt1!=NULL){
                evaluate(i->pt1);
            }
            evaluate(i->pt2);
            return 0;
            break;
        default:
            yyerror("NODE ERROR");
    }
}
