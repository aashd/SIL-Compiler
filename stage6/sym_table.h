#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef struct gsymt_node {
    char* name;
    int type;//for type checking
    int size;//for arrays
    int* bind;//?
    struct symt_node* next;
}gsymbol;
gsymbol* make_syt_entry(char* name,int type,int size,int* bind,gsymbol* next);
int* allocate_mem(int size);
gsymbol* search(char* var_name);
gsymbol* search1(char* var_name, gsymbol* sym);
int add_sym(char* name, int type, int size,int * bind);
