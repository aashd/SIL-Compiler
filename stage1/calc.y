%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void yyerror(char *msg);
char* concat(char* a,char*b);
char* dyna_copy(char* a);
extern int yylex();
%}
%union {
    char* f;
}
%token <f> NUM
%type <f> E
%left '+'
%left '*'
%%
S : E   {printf("%s\n",$1);};

E : E '+' E  {$$=concat("+",concat($1,$3));printf(" in E+e %s  %d\n",$1,strlen($1));}
  | E '*' E  {$$=concat("*",concat($1,$3));/*printf("%s
%d\n",$$,strlen($$));*/}
  | '(' E ')' {$$= $2;}
  | NUM    {$$= dyna_copy($1);printf("%s  %d\n",$$,strlen($$));}
  ;

%%

void yyerror(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}
char* concat(char* a,char* b){
    int y,x;
    char* i;
    x=strlen(a)+strlen(b)+1;
    i=(char*)malloc((strlen(a)+strlen(b)+1)*sizeof(char)); 
    y=0;
    for(y=0;y<x-1;y++){
        if(y<strlen(a)){
            i[y]=a[y];
        }
        else{
            i[y]=b[y-strlen(a)];
        }
    }
    printf("a   %s   %d\n",a,strlen(a));
    printf("b   %s   %d\n",b,strlen(b));
    printf("%s\n",i);
    i[y]='\0';
    return i;
}
char* dyna_copy(char* a){
    int l,y;
    char* i;
    l=strlen(a);
    i=(char*)malloc((l+1)*sizeof(char));
    for(y=0;y<l;y++){
        i[y]=a[y];
        
    }
    i[y]='\0';
    return i;
}
int main(){
    yyparse();
    return 0;   
}
