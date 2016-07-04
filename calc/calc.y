%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1 //系统保留定义的token，值0-255
%}
%union {
    int          int_value;
    double       double_value;
}
/*%token是声明一个标记，编译后产生#define ADD 259 (0-255是系统保留的token)*/
%token <double_value>      DOUBLE_LITERAL
%token ADD SUB MUL DIV LP RP CR
%type <double_value> expression term primary_expression
%%
line_list /*多行规则*/
    : line /*单行规则*/
    | line_list line
    ;
line
    : expression CR /*一个表达式后接换行符*/
    {
        printf(">>%lf\n", $1);
    }
expression /*表达式规则*/
    : term /*和项*/
    | expression ADD term /*表达式 + 和项*/
    {
        $$ = $1 + $3;
    }
    | expression SUB term
    {
        $$ = $1 - $3;
    }
    ;
term /*和项规则*/
    : primary_expression /*一元表达式*/
    | term MUL primary_expression
    {
        $$ = $1 * $3;
    }
    | term DIV primary_expression
    {
        $$ = $1 / $3;
    }
    ;
primary_expression /*一元表达式规则*/
    : DOUBLE_LITERAL /*实数的字面常量*/
    | LP expression RP /*括号*/
    {
        $$ = $2;
    }
    ;                 
%%
int yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, "parser error near %s\n", yytext);
    return 0;
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;
    if (yyparse()) {
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
}
