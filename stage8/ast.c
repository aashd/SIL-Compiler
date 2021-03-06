#include "sym_table.h"
#include "ast.h"

node* make_node(int type,node* pt1,node* pt2, node* pt3, gsymbol* gst,char* a , int num_val){
    node* i;
    i=(node*)malloc(sizeof(node));
    i->type=type;
    i->pt1=pt1;
    i->pt2=pt2;
    i->pt3=pt3;
    i->str=a;
    i->num_val=num_val;
    i->g_sym_tab=gst;
    return i;
}
int evaluate(node* i,gsymbol* head){
    printf("EVAL   %d\n",i->type);
    if(i==NULL){
        printf("EVAL tying to eval  NULL\n");
        exit(1);
    }
    char a[8];
    int* x;
    int temp,temp1;
    gsymbol* ptr;
    switch(i->type){
        case num_type:
            return i->num_val;
            break;
        case bool_type:
            return i->num_val;//either 1 or 0
            break;
        case id_type:
            ptr=search(i->str,head);
            if (i->pt1==NULL) {//norm
                if (ptr==NULL) {
                    yyerror("ID not in symbol table");
                }   
                else {
                    x=ptr->bind;
                    return *(x);
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in symbol table");
                }   
                else {
                    temp=evaluate(i->pt1,head);
                    if (temp >(ptr->size)-1) {/////////////int[a] 0-----a-1 only
                        yyerror("segmentation fault");
                    }
                    else {
                        x=ptr->bind;
                        return *(x+temp);
                    }
                }
            }
            break;
        case exp_type:
            temp=i->num_val;//op type in num_val
            printf("EXPR type  %d\n",temp);
            switch(temp){
                case add_op:
                    return evaluate(i->pt1,head)+evaluate(i->pt2,head);
                    break;
                case mul_op:
                    return evaluate(i->pt1,head)*evaluate(i->pt2,head);
                    break;
                case div_op:
                    if (evaluate(i->pt2,head)==0) {
                        yyerror("ERROR: Div error Segmentation fault") ;
                    }
                    return evaluate(i->pt1,head)/evaluate(i->pt2,head);
                    break;
                case neg_op:
                    if (i->pt2!=NULL) {
                        return evaluate(i->pt1,head)-evaluate(i->pt2,head);
                    }
                    else {
                        return -evaluate(i->pt1,head);
                    }
                    break;
                case mod_op:
                    if (evaluate(i->pt2,head)==0) {
                        yyerror("ERROR: Div error Segmentation fault") ;
                    }
                    return evaluate(i->pt1,head)%evaluate(i->pt2,head);
                    break;
                case great_op:
                    return evaluate(i->pt1,head)>evaluate(i->pt2,head);
                    break;
                case geq_op:
                    return evaluate(i->pt1,head)>=evaluate(i->pt2,head);
                    break;
                case less_op:
                    return evaluate(i->pt1,head)<evaluate(i->pt2,head);
                    break;
                case leq_op:
                    return evaluate(i->pt1,head)<=evaluate(i->pt2,head);
                    break;
                case comp_op:
                    return evaluate(i->pt1,head)==evaluate(i->pt2,head);
                    break;
                case neq_op:
                    return evaluate(i->pt1,head)!=evaluate(i->pt2,head);
                    break;
                case and_op:
                    return evaluate(i->pt1,head)&&evaluate(i->pt2,head);
                    break;
                case or_op:
                    return evaluate(i->pt1,head)||evaluate(i->pt2,head);
                    break;
                case not_op:
                    return !evaluate(i->pt1,head);
                    break;
                case no_op://for extensions
                    return evaluate(i->pt1,head);
                    break;
                default:
                    printf("exp_typr error\n");
            }
            break;
        case write_type:
            if (strcmp((i->pt1)->str,"a1")==0) {//int
                printf("%d\n",evaluate(i->pt1,head));
                break;
            }
            else {
                temp=evaluate(i->pt1,head);
                if (temp==0) {
                    printf("FALSE %d\n",temp);
                }
                else {
                    printf("TRUE %d\n",temp);
                }
            }
            break;
        case read_type:
            ptr=search((i->pt1)->str,head);
            if ((i->pt1)->pt1==NULL) {///normal read
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    x=ptr->bind;
                    if (ptr->type==1) {//int 
                        scanf("%d",x);
                    }
                    else {
                        scanf("%s",a);//input char string
                        if (strcmp(a,"TRUE")==0) {//bool
                            *(x)=1;
                        }
                        else {
                            *(x)=0;
                        }
                    }
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    temp=evaluate((i->pt1)->pt1,head);
                    if (temp> (ptr->size)-1) {
                        yyerror("Segmentation fault");
                    }
                    else {
                        x=ptr->bind;
                        if (ptr->type==1) {//int 
                            scanf("%d",&(*(x+temp)));
                        }
                        else {
                            scanf("%s",a);//input char string
                            if (strcmp(a,"TRUE")==0) {//bool
                                *(x+temp)=1;
                            }
                            else {
                                *(x+temp)=0;
                            }
                        }
                        x=ptr->bind;
                        scanf("%d",&(*(x+temp)));
                    }
                }
            }
            return 0;
            break;
        case assign_type:
            ptr=search((i->pt1)->str,head);
            if ((i->pt1)->pt1==NULL) {///normal read
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    x=ptr->bind;
                    temp=evaluate(i->pt2,head);
                    *(x)=temp;
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    temp=evaluate((i->pt1)->pt1,head);
                    if (temp> (ptr->size)-1) {
                        yyerror("Segmentation fault");
                    }
                    else {
                        x=ptr->bind;
                        temp1=evaluate(i->pt2,head);
                        *(x+temp)=temp1;///no need for temp*sizeof(int)
                        //x[temp]=temp1;
                    }
                }
            }
            return 0;
            break;
        case if_type:
            if(evaluate(i->pt1,head)){
                        break;
                evaluate(i->pt2,head);
            }
            if(i->pt3!=NULL){
                evaluate(i->pt3,head);
            }
            return 0;
            break;
        case while_type:
            while(evaluate(i->pt1,head)){
                evaluate(i->pt2,head);
            }
            return 0;
            break;
        case block_type:
            if(i->pt1!=NULL){
                evaluate(i->pt1,head);
            }
            evaluate(i->pt2,head);
            return 0;
            break;
        case id_list_type:
            if(i->pt1!=NULL){
                evaluate(i->pt1,head);
            }
            if ((i->pt2)->pt1==NULL) {//normal non array decl
                x=(int*)malloc(1*sizeof(int));
                add_sym((i->pt2)->str,(i->pt2)->num_val,1,x,head);//numval has type info 
            }
            else {//array declatations
                temp=evaluate((i->pt2)->pt1,head);
                if (temp==0) {
                    x=(int*)malloc(sizeof(int));
                }
                else {
                    x=(int*)malloc(temp*sizeof(int));
                }
                add_sym((i->pt2)->str,(i->pt2)->num_val,temp,x,head);//asd [a] size = a indexing starts from 0
            }
            return 0;
            break;
        case decl_type:
            evaluate(i->pt1,head);
            return 0;
            break;
        case decl_list_type:
            if (i->pt1!=NULL) {
                evaluate(i->pt1,head);
            }
            evaluate(i->pt2,head);
            return 0;
            break;
        default:
            yyerror("NODE ERROR");
    }
}
char*  dyna_copy(char* a)
{
    int n,j;
    char* i;
    n=strlen(a);
    i=(char*)malloc((n+1)*sizeof(char));
    for (j = 0; j < n; j++) {
        i[j]=a[j];
    }
    i[j]='\0';
    return i;
}
