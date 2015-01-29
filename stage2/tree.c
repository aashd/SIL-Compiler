#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
    
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

tnode* mknode(char* str, tnode* left_p, tnode* right_p)
{
    tnode* i;
    i=(tnode*)malloc(sizeof(tnode));
    left_p->parent=i;
    right_p->parent=i;
    i->left=left_p;
    i->right=right_p;
    i->parent=NULL;
    i->flag=0;
    i->op=str[0];
    return i;   
}
tnode* mkleaf(int a)
{
    tnode* i;
    i=(tnode*)malloc(sizeof(tnode));
    i->left=NULL;
    i->right=NULL;
    i->flag=1;
    i->ival=a;
    i->parent=NULL;
    return i;
}
int isleaf(tnode* a)
{
    if (a->flag==1) {
        return 1;
    }
    else {
        return 0;
    }
}
int prefix(tnode* a)
{
    if (isleaf(a)) {
        printf(" %d ",a->ival);
        return 0;
    }
    else {
        printf(" %c ",a->op);
        prefix(a->left);
        prefix(a->right);
        return 0;
    }   
}
int infix(tnode* a)
{
    if (isleaf(a)) {
        printf(" %d ",a->ival);
        return 0;
    }   
    else {
        infix(a->left);
        printf(" %c  ",a->op);  
        infix(a->right);
        return 0;
    }   
}  
int postfix(tnode* a)
{
    if (isleaf(a)) {
        printf(" %d ",a->ival);
        return 0;
    }
    else {
        postfix(a->left);
        postfix(a->right);
        printf(" %c  ",a->op);  
        return 0;
    }
}               
int tree_eval(tnode* a)
{
    if (isleaf(a)) {
        return a->ival; 
    }
    else {
        if (a->op=='+') {
            return tree_eval(a->left)+tree_eval(a->right);
        }
        else if (a->op=='*') {
            return tree_eval(a->left)*tree_eval(a->right);
        }
    }
}
