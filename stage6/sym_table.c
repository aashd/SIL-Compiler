#include "y.tab.h"
#include "sym_table.h"
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
gsymbol* search(char* var_name){
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
int add_sym(char* name, int type, int size,int * bind){
    if (search(name)!=NULL) {
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
