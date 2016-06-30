%{
#include <stdio.h>
#include "y.tab.h"

int yywrap(void) {
    return 1;
}


%}

%%
"+"		return ADD;/*ADD等宏在y.tab.h中已定义*/
"-"		return SUB;
"*"		return MUL;
"/"		return DIV;
"\n"	return CR;

([1-9][0-9]*)|0|([0-9]+\.[0-9]+) {
	double temp;
	sscanf(yytext, "%lf", &temp);
	yylval.double_value = temp; /*解析出的值放在全局变量yylval中，yylval是个联合体，可放任意记号值*/
	return DOUBLE_LITERAL;
}

[ \t]	;/*对空格和制表符，其动作为空*/
. 		{
	fprintf(stderr, "lexical error.\n");
	exit(1);
}

%%
