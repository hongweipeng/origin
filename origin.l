%{

    //词法分析器
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "DBG.h"
    #include "origin.h"
    #include "y.tab.h"
    
    /*
    解析一个文件完毕后会调用yywrap,返回表示1结束,返回0表示继续
    */
    int yywrap(void) {
        return 1;
    }

    static void increase_line_num(void) {
        org_get_current_interpreter()->current_line_number++;
    }

    //定义保留字
%}

%start COMMENT STRING_LITERAL_STATE

%%
<INITIAL>"function"     return FUNCTION;
<INITIAL>"if"           return IF;
<INITIAL>"else"         return ELSE;
<INITIAL>"elseif"       return ELSEIF;
<INITIAL>"while"        return WHILE;
<INITIAL>"for"          return FOR;
<INITIAL>"continue"     return CONTINUE;
<INITIAL>"break"        return BREAK;
<INITIAL>"null"         return NULL_O;
<INITIAL>"true"         return TRUE_O;
<INITIAL>"false"        return FALSE_O;
<INITIAL>"global"       return GLOBAL_O;
<INITIAL>"return"       return RETURN_O;
<INITIAL>"("            return LP;
<INITIAL>")"            return RP;
<INITIAL>"{"            return LC;
<INITIAL>"}"            return RC;
<INITIAL>";"            return SEMICOLON;
<INITIAL>","            return COMMA;
<INITIAL>"&&"           return LOGICAL_AND;
<INITIAL>"||"           return LOGICAL_OR;
<INITIAL>"="            return ASSIGN;
<INITIAL>"=="           return EQ;
<INITIAL>"!="           return NE;
<INITIAL>">"            return GT;
<INITIAL>">="           return GE;
<INITIAL>"<"            return LT;
<INITIAL>"<="           return LE;
<INITIAL>"+"            return ADD;
<INITIAL>"-"            return SUB;
<INITIAL>"*"            return MUL;
<INITIAL>"/"            return DIV;
<INITIAL>"%"            return MOD;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    //标识符(变量名，函数名)
    yylval.identifier = org_create_identifier(yytext);
    return IDENTIFIER;
}
<INITIAL>([1-9][0-9]*)|0 {
    //整数类型
    Expression *expression = org_alloc_expression(INT_EXPRESSION);
    sscanf(yytext, "%d", &expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>[0-9]\.[0-9]+ {
    //处理double类型
    Expression *expression = org_alloc_expression(DOUBLE_EXPRESSION);
    sscanf(yytext, "%lf", &expression->u.double_value);
    yylval.expression = expression;
    return DOUBLE_EXPRESSION;
}

<INITIAL>\" {
    //处理字符串
    org_open_string_literal();
    BEGIN STRING_LITERAL_STATE;
}

<INITIAL>[ \t] ;/*对于空格和制表符，无动作*/
<INITIAL>\n     {increase_line_num();}
<INITIAL>#      {BEGIN COMMENT;}
<INITIAL>.      {
    char buf[LINE_BUF_SIZE];
    if(isprint(yytext[0])) {
        buf[0] = yytext[0];
        buf[1] = '\0';
    }else {
        sprintf(buf, "0x%02x", (unsigned char)yytext[0]);
    }
    //编译错误
    //org_compile_error();
}

<COMMENT>\n {
    //单行注释结束,条件设为初始条件
    increase_line_num();
    BEGIN INITIAL;
}
<COMMENT>.  ;/*注释的其他字符都不予处理*/

<STRING_LITERAL_STATE>\"    {
    //遇到字符串形如 "this is string\n"
    Expression *expression = org_alloc_expression(STRING_LITERAL_STATE);
    expression->u.string_value = org_close_string_literal();
    yylval.expression = expression;
    BEGIN INITIAL;
    return STRING_LITERAL;
}

<STRING_LITERAL_STATE>\n    {
    org_add_string_literal('\n');
    increase_line_num();
}
<STRING_LITERAL_STATE>\\\"  org_add_string_literal('"');
<STRING_LITERAL_STATE>\\n   org_add_string_literal('\n');
<STRING_LITERAL_STATE>\\t   org_add_string_literal('\t');
<STRING_LITERAL_STATE>\\\\  org_add_string_literal('\\');
<STRING_LITERAL_STATE>.     org_add_string_literal(yytext[0]);

%%
