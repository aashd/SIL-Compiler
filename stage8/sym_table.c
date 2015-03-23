#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef struct gsymt_node {
    char* name;
    int type;//for type checking
    int size;//for arrays
    int* bind;//?
    struct gsymt_node* next;
}gsymbol;
gsymbol* make_syt_entry(char* name,int type,int size,int* bind,gsymbol* next);
int* allocate_mem(int size);
gsymbol* search(char* var_name, gsymbol* head);
gsymbol* search1(char* var_name, gsymbol* sym);
int add_sym(char* name, int type, int size,int * bind,gsymbol* head);

gsymbol* make_syt_entry(char* name,int type,int size,int* bind,gsymbol* next)
{
    gsymbol* i;
    i=(gsymbol*)malloc(sizeof(gsymbol));
    (*i).name=name;
    i->type=type;
    i->size=size;
    i->bind=bind;
    i->next=next;
}
int* allocate_mem(int size){
    int* i;
    i=(int* )malloc(sizeof(int)*size);
    return i;
}
gsymbol* search(char* var_name, gsymbol* head){
    if (head->next==NULL) {
        //printf("symtable empty\n");
        return NULL;
    }
    else {
        gsymbol* i;
        i=search1(var_name, head->next);
        return i;
    }
}
gsymbol* search1(char* var_name, gsymbol* sym){
    if (sym==NULL) {
        //printf("UNDEFINED variable\n");
        return NULL;
    }
    else {
        if (strcmp(var_name,sym->name)==0) {
            //printf("MATCH\n");
            return sym;
        }
        else {
            search1(var_name,sym->next);
        }
    }
}
int add_sym(char* name, int type, int size,int * bind,gsymbol* head){
    if (search(name, head)!=NULL) {
        printf("ERROR: Variable already declared\n");
        return 0;
    }
    else {
        gsymbol* i;
        gsymbol* j;
        i=make_syt_entry(name,type,size,bind,NULL);
        j=head;
        while (j->next!=NULL) {
            j=j->next;
        }
        j->next=i;
        return 1;
    }
}
