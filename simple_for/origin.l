%{

    //词法分析器
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "origin.h"
    #include "y.tab.h"

    /*
    解析一个文件完毕后会调用yywrap,返回表示1结束,返回0表示继续
    */
    int yywrap(void) {
        return 1;
    }

    //定义保留字
%}

%%
<INITIAL>"if"           return IF;

<INITIAL>"for"          return FOR;
<INITIAL>"break"        return BREAK;

<INITIAL>"("            return LP;
<INITIAL>")"            return RP;
<INITIAL>"{"            return LC;
<INITIAL>"}"            return RC;
<INITIAL>";"            return SEMICOLON;


<INITIAL>"="            return ASSIGN;

<INITIAL>">"            return GT;
<INITIAL>">="           return GE;
<INITIAL>"<"            return LT;
<INITIAL>"<="           return LE;

<INITIAL>"+"            return ADD;
<INITIAL>"-"            return SUB;
<INITIAL>"*"            return MUL;
<INITIAL>"/"            return DIV;

<INITIAL>"print"        return PRINT;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    //标识符(变量名，函数名)
    char *new_str;
    new_str = (char *)malloc(strlen(yytext) + 1);
    strcpy(new_str, yytext);
    yylval.identifier = new_str;
    return IDENTIFIER;
}

<INITIAL>([1-9][0-9]*)|0 {
    //整数类型
    Expression *expression = org_alloc_expression(INT_EXPRESSION);
    sscanf(yytext, "%d", &expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>\n     ;
<INITIAL>[ \t] ;/*对于空格和制表符，无动作*/

