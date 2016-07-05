%{

    //词法分析器
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
    
    /*
    解析一个文件完毕后会调用yywrap,返回表示1结束,返回0表示继续
    */
    int yywrap(void) {
        return 1;
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





%%
