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


