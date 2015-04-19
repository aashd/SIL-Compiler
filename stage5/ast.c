#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
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
int evaluate1(node* i)
{
    FILE * fp;
    fp= fopen("./code_gen","w+");
    fprintf(fp, "START\n");
    evaluate(i,fp);
    fprintf(fp, "HALT\n");
}
int evaluate(node* i,FILE* fp){
    if(i==NULL){
        printf("EVAL tying to eval  NULL\n");//block null?
        return 0;
        //exit(1);
    }
    int x,y,z;
    switch(i->type){
        case num_type:
            y=assign_reg();
            x=i->num_val;
            fprintf(fp, "MOV R%d,%d\n",y,x);
            free_reg(y);
            return y;
            //return i->num_val;
            break;
        case id_type:
            y=assign_reg();
            x=char_int(i->ch);
            fprintf(fp, "MOV R%d,[%d]\n",y,x);
            free_reg(y);
            return y;
            //return sym_tab[char_int(i->ch)];// check of initialised /uninitialised
            break;
        case exp_type:
            switch(i->ch){
                case '+':
                    x=evaluate(i->pt1,fp);
                    y=evaluate(i->pt2,fp);
                    fprintf(fp, "ADD R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,fp)+evaluate(i->pt2,fp);
                    break;
                case '*':
                    x=evaluate(i->pt1,fp);
                    y=evaluate(i->pt2,fp);
                    fprintf(fp, "MUL R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,fp)*evaluate(i->pt2,fp);
                    break;
                case '>':
                    x=evaluate(i->pt1,fp);
                    y=evaluate(i->pt2,fp);
                    fprintf(fp, "GT R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,fp)>evaluate(i->pt2,fp);
                case '<':
                    x=evaluate(i->pt1,fp);
                    y=evaluate(i->pt2,fp);
                    fprintf(fp, "LT R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,fp)<evaluate(i->pt2,fp);
                    break;
                case '=':
                    x=evaluate(i->pt1,fp);
                    y=evaluate(i->pt2,fp);
                    fprintf(fp, "EQ R%d,R%d\n",x,y);
                    free_reg(x);
                    return x;
                    //return evaluate(i->pt1,fp)==evaluate(i->pt2,fp);
                    break;
                case '~':
                    return evaluate(i->pt1,fp);
                default:
                    printf("exp_typr error\n");
            }
            break;
        case write_type:
            x=evaluate(i->pt1,fp);
            fprintf(fp, "OUT R%d\n",x);
            free_reg(x-1);
            return 0;
            //printf("%d\n",evaluate(i->pt1,fp));
            break;
        case read_type:
            x=char_int((i->pt1)->ch);
            y=assign_reg();
            fprintf(fp, "IN R%d\n",y);
            fprintf(fp, "MOV [%d], R%d\n",x,y);
            free_reg(y-1);
            return 0;
            break;
        case assign_type:
            x=char_int((i->pt1)->ch);
            y=evaluate(i->pt2,fp);
            fprintf(fp, "MOV [%d], R%d\n",x,y);
            free_reg(y-1);
            //sym_tab[char_int((i->pt1)->ch)]=evaluate(i->pt2,fp);
            return 0;
            break;
        case if_type:
            x=evaluate(i->pt1,fp);
            y=assign_lab();
            z=assign_lab();
            fprintf(fp, "JZ  R%d, L%d\n",x,y);//expects block NULL=> ret 0 and no error
            evaluate(i->pt2,fp);//if
            fprintf(fp, "JMP  L%d\n",z);
            fprintf(fp, " L%d:\n",y);
            evaluate(i->pt3,fp);//else
            fprintf(fp, " L%d:\n",z);
            free_reg(x-1);    
            return 0;
            //if(evaluate(i->pt1,fp)){

             //   evaluate(i->pt2,fp);
            //}
            //if(i->pt3!=NULL){
             //   evaluate(i->pt3,fp);
            //}
            //return 0;
            break;
        case while_type:
            y=assign_lab();
            z=assign_lab();
            fprintf(fp, " L%d:\n",y);
            x=evaluate(i->pt1,fp);//expr
            fprintf(fp, "JZ R%d, L%d\n",x,z);
            evaluate(i->pt2,fp);//block
            free_reg(x-1);
            fprintf(fp, "JMP L%d\n",y);
            fprintf(fp, "L%d:\n",z);
            free_reg(x-1);
            //while(evaluate(i->pt1,fp)){
            //    evaluate(i->pt2,fp);
            //}
            return 0;
            break;
        case block_type:
            if(i->pt1!=NULL){
                evaluate(i->pt1,fp);
            }
            evaluate(i->pt2,fp);
            return 0;
            break;
        default:
            yyerror("NODE ERROR");
    }
}
int assign_reg()
{
    regact=regact+1;
    return regact;
}
int free_reg(int y)//always free 1 less
{
    regact=y;
    return 0;
}
int assign_lab()
{
    lab_no=lab_no+1;
    return lab_no;
}
