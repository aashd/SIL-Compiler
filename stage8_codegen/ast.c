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
int evaluate1(node* i,gsymbol* head)
{
    FILE * fp;
    fp= fopen("./code_gen","w+");
    fprintf(fp, "START\n");
    fflush(fp);
    evaluate(i,head,fp);
    fprintf(fp, "HALT\n");
    fflush(fp);
}
int evaluate(node* i,gsymbol* head,FILE* fp){
    if(i==NULL){
        //printf("EVAL tying to eval  NULL\n");
        //exit(1);
        return 0;
    }
    //printf("EVAL   %d\n",i->type);
    char a[8];
    int x,y,z,temp,temp1;
    gsymbol* ptr;
    switch(i->type){
        case num_type:
            y=assign_reg();
            x=i->num_val;
            fprintf(fp, "MOV R%d,%d\n",y,x);
            free_reg(y);
            return y;
            //return i->num_val;
            break;
        case bool_type:
            y=assign_reg();
            x=i->num_val;//either 0/1;
            fprintf(fp, "MOV R%d,%d\n",y,x);
            free_reg(y);
            return y;
            //return i->num_val;//either 1 or 0
            break;
        case id_type://same for bool/int
            ptr=search(i->str,head);
            if (i->pt1==NULL) {//norm
                if (ptr==NULL) {
                    yyerror("ID not in symbol table");
                }   
                else {
                    y=ptr->bind;
                    x=assign_reg();
                    fprintf(fp, "MOV R%d, [%d]\n",x,y);
                    free_reg(x);
                    return x;
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in symbol table");
                }   
                else {///array size is static so can add size checks at runtime
                    temp=evaluate(i->pt1,head,fp);//index required
                    temp1=assign_reg();
                    x=assign_lab();
                    y=assign_lab();
                    fprintf(fp, "MOV R%d ,%d\n",temp1,ptr->size);
                    fprintf(fp, "LT R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                    temp=evaluate(i->pt1,head,fp);//again evaluate
                    fprintf(fp, "MOV R%d ,%d\n",temp1,0);
                    fprintf(fp, "GE R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                        
                        temp=evaluate(i->pt1,head,fp);
                        temp1=assign_reg();
                        z=ptr->bind;//start mem 
                        fprintf(fp, "MOV R%d,%d \n",temp1,z);
                        fprintf(fp, "ADD R%d, R%d\n",temp,temp1);
                        fprintf(fp, "MOV R%d, [R%d]\n",temp1,temp);
                        
                    fprintf(fp, "JMP L%d\n",y);
                    fprintf(fp, "L%d:\n",x);//error if index >=size&& index <0
                    free_reg(temp-1);
                    temp=assign_reg();
                    fprintf(fp, "MOV R%d, %d\n",temp, 1000001);//error code for array over flowing
                    fprintf(fp, "OUT R%d\n",temp);
                    fprintf(fp, "HALT\n");
                    fprintf(fp, "L%d:\n",y);//exit
                    free_reg(temp1);
                    return temp1;
                }
            }
            break;
        case exp_type:
            temp=i->num_val;//op type in num_val
            //printf("EXPR type  %d\n",temp);
            switch(temp){
                case add_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "ADD R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,head)+evaluate(i->pt2,head);
                    break;
                case mul_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "MUL R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case div_op:
                    x=evaluate(i->pt1,head,fp);//divide checks
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "DIV R%d,R%d\n",x,y);
                    free_reg(x);
                    //if (evaluate(i->pt2,head)==0) {
                    //    yyerror("ERROR: Div error Segmentation fault") ;
                    //}
                    return x;
                    break;
                case neg_op:
                    if (i->pt2!=NULL) {
                        x=evaluate(i->pt1,head,fp);
                        y=evaluate(i->pt2,head,fp);
                        fprintf(fp, "SUB R%d,R%d\n",x,y);
                        free_reg(x);
                        return x;
                        //return evaluate(i->pt1,head)-evaluate(i->pt2,head);
                    }
                    else {
                        x=evaluate(i->pt1,head,fp);
                        fprintf(fp, "SUB 0,R%d\n",x);
                        free_reg(x);
                        return x;
                    }
                    break;
                case mod_op:
                    x=evaluate(i->pt1,head,fp);//div errors cant be checked at compile time?
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "MOD R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //if (evaluate(i->pt2,head)==0) {
                    //    yyerror("ERROR: Div error Segmentation fault") ;
                    //}
                    break;
                case great_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "GT R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case geq_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "GE R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case less_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "LT R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case leq_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "LE R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case comp_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "EQ R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    break;
                case neq_op:
                    x=evaluate(i->pt1,head,fp);
                    y=evaluate(i->pt2,head,fp);
                    fprintf(fp, "NE R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,head)!=evaluate(i->pt2,head);
                    break;
                case and_op:
                    y=assign_lab();
                    z=assign_lab();
                    x=evaluate(i->pt1,head,fp);// will return bool
                    fprintf(fp, "JZ R%d L%d\n",x,y);
                    temp=evaluate(i->pt2,head,fp);
                    fprintf(fp, "JZ R%d L%d\n",temp,y);
                    fprintf(fp, "MOV R%d 1\n",x);//set 1 and return
                    fprintf(fp, "JMP  L%d\n",z);
                    fprintf(fp, " L%d:\n",y);//set 0 and return
                    fprintf(fp, "MOV R%d 0\n",x);
                    fprintf(fp, " L%d:\n",z);//end
                    free_reg(x);    
                    return x;
                    //return evaluate(i->pt1,head)&&evaluate(i->pt2,head);
                    break;
                case or_op:
                    y=assign_lab();
                    z=assign_lab();
                    x=evaluate(i->pt1,head,fp);// will return bool
                    fprintf(fp, "JNZ R%d L%d\n",x,y);
                    temp=evaluate(i->pt2,head,fp);
                    fprintf(fp, "JNZ R%d L%d\n",temp,y);
                    fprintf(fp, "MOV R%d 0\n",x);//set 1 and return
                    fprintf(fp, "JMP  L%d\n",z);
                    fprintf(fp, " L%d:\n",y);//set 1 and return
                    fprintf(fp, "MOV R%d 1\n",x);
                    fprintf(fp, " L%d:\n",z);//end
                    free_reg(x);    
                    return x;
                    //return evaluate(i->pt1,head)||evaluate(i->pt2,head);
                    break;
                case not_op:
                    y=assign_lab();
                    z=assign_lab();
                    x=evaluate(i->pt1,head,fp);// will return bool
                    fprintf(fp, "JZ R%d L%d\n",x,y);
                    fprintf(fp, "MOV R%d 0\n",x);//set 0 tp return reg
                    fprintf(fp, "JMP  L%d\n",z);
                    fprintf(fp, " L%d:\n",y);//set 1 and return
                    fprintf(fp, "MOV R%d 1\n",x);
                    fprintf(fp, " L%d:\n",z);//end
                    free_reg(x);    
                    return x;
                    //return !evaluate(i->pt1,head);
                    break;
                case no_op://for extensions
                    return evaluate(i->pt1,head,fp);
                    break;
                default:
                    printf("exp_typr error\n");
            }
            break;
        case write_type:
            x=evaluate(i->pt1,head,fp);
            fprintf(fp, "OUT R%d\n",x);
            free_reg(x-1);
            return 0;
            break;
        case read_type:
            ptr=search((i->pt1)->str,head);
            if ((i->pt1)->pt1==NULL) {///normal read
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    x=ptr->bind;
                    y=assign_reg();
                    fprintf(fp, "IN R%d\n",y);
                    fprintf(fp, "MOV [%d], R%d\n",x,y);
                    free_reg(temp-1);
                    return 0;
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    temp=evaluate((i->pt1)->pt1,head,fp);//index required
                    temp1=assign_reg();
                    x=assign_lab();
                    y=assign_lab();
                    fprintf(fp, "MOV R%d ,%d\n",temp1,ptr->size);
                    fprintf(fp, "LT R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                    temp=evaluate((i->pt1)->pt1,head,fp);//again evaluate
                    fprintf(fp, "MOV R%d ,%d\n",temp1,0);
                    fprintf(fp, "GE R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                        
                        temp=evaluate((i->pt1)->pt1,head,fp);//index
                        temp1=assign_reg();
                        z=ptr->bind;//start mem 
                        fprintf(fp, "MOV R%d,%d \n",temp1,z);
                        fprintf(fp, "ADD R%d, R%d\n",temp,temp1);
                        fprintf(fp, "IN R%d\n",temp1);
                        fprintf(fp, "MOV [R%d], R%d\n",temp,temp1);
                        
                    fprintf(fp, "JMP L%d\n",y);
                    fprintf(fp, "L%d:\n",x);//error if index >=size&& index <0
                    free_reg(temp-1);
                    temp=assign_reg();
                    fprintf(fp, "MOV R%d, %d\n", temp, 1000001);//error code for array over flowing
                    fprintf(fp, "OUT R%d\n",temp);
                    fprintf(fp, "HALT\n");
                    fprintf(fp, "L%d:\n",y);//exit
                    free_reg(temp-1);
                    return 1;
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
                    temp=evaluate(i->pt2,head,fp);//
                    fprintf(fp, "MOV [%d], R%d\n",x,temp);
                    free_reg(temp-1);
                    return 1;
                }
            }
            else {//array
                if (ptr==NULL) {
                    yyerror("ID not in sym table");
                }
                else {
                    temp=evaluate((i->pt1)->pt1,head,fp);//index required
                    temp1=assign_reg();
                    x=assign_lab();
                    y=assign_lab();
                    fprintf(fp, "MOV R%d ,%d\n",temp1,ptr->size);
                    fprintf(fp, "LT R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                    temp=evaluate((i->pt1)->pt1,head,fp);//again evaluate
                    fprintf(fp, "MOV R%d ,%d\n",temp1,0);
                    fprintf(fp, "GE R%d ,R%d\n",temp,temp1);
                    fprintf(fp, "JZ R%d, L%d\n",temp,x);
                    free_reg(temp-1);
                        
                        temp=evaluate((i->pt1)->pt1,head,fp);//index
                        temp1=assign_reg();
                        z=ptr->bind;//start mem 
                        fprintf(fp, "MOV R%d,%d \n",temp1,z);
                        fprintf(fp, "ADD R%d, R%d\n",temp,temp1);
                        free_reg(temp1-1);
                        temp1=evaluate(i->pt2,head,fp);
                        fprintf(fp, "MOV [R%d], R%d\n",temp,temp1);
                        
                    fprintf(fp, "JMP L%d\n",y);
                    fprintf(fp, "L%d:\n",x);//error if index >=size&& index <0
                    free_reg(temp-1);
                    temp=assign_reg();
                    fprintf(fp, "MOV R%d, %d\n", temp, 1000001);//error code for array over flowing
                    fprintf(fp, "OUT R%d\n",temp);
                    fprintf(fp, "HALT\n");
                    fprintf(fp, "L%d:\n",y);//exit
                    free_reg(temp-1);
                    return 1;
                }
            }
            return 0;
            break;
        case if_type:
            x=evaluate(i->pt1,head,fp);
            y=assign_lab();
            z=assign_lab();
            fprintf(fp, "JZ  R%d, L%d\n",x,y);
            evaluate(i->pt2,head,fp);//if
            fprintf(fp, "JMP  L%d\n",z);
            fprintf(fp, " L%d:\n",y);
            evaluate(i->pt3,head,fp);//else block==NULL =>noting printed here
            fprintf(fp, " L%d:\n",z);
            free_reg(x-1);    
            return 0;
            break;
        case while_type:
            y=assign_lab();
            z=assign_lab();
            fprintf(fp, " L%d:\n",y);
            x=evaluate(i->pt1,head,fp);//expr
            fprintf(fp, "JZ R%d, L%d\n",x,z);
            evaluate(i->pt2,head,fp);//block
            free_reg(x-1);
            fprintf(fp, "JMP L%d\n",y);
            fprintf(fp, "L%d:\n",z);
            free_reg(x-1);
            return 0;
            break;
        case block_type:
            if(i->pt1!=NULL){
                evaluate(i->pt1,head,fp);
            }
            evaluate(i->pt2,head,fp);
            return 0;
            break;
        case id_list_type://same for int bool

            if(i->pt1!=NULL){//for recursion
                evaluate(i->pt1,head,fp);
            }
            if ((i->pt2)->pt1==NULL) {//normal non array decl
                x=assign_mem(1);
                add_sym((i->pt2)->str,(i->pt2)->num_val,1,x,head);//numval has type info x has bind info 
            }
            else {//array declatations no need for dynamic checks for array size  only num
                x=((i->pt2)->pt1)->num_val;//directly accessing size
                y=assign_mem(x);//size 0 is dealt in the parser itself || change it to here?
                add_sym((i->pt2)->str,(i->pt2)->num_val,x,y,head);//asd [a] size = a indexing starts from 0
            }
            return 0;
            break;
        case decl_type:
            evaluate(i->pt1,head,fp);
            return 0;
            break;
        case decl_list_type:
            if (i->pt1!=NULL) {
                evaluate(i->pt1,head,fp);
            }
            evaluate(i->pt2,head,fp);
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
int assign_reg()
{
    reg_act=reg_act+1;
    return reg_act;
}
int free_reg(int y)
{
    reg_act=y;
    return 0;
}
int assign_lab()
{
    lab_no=lab_no+1;
    return lab_no;
}
int assign_mem(int size)
{
    int temp;
    temp=mem_act;
    mem_act=mem_act+size;//mem_Act will have 1st free location
    return temp;
}
