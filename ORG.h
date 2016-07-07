/*************************************************************************
	> File Name: ORG.h
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月04日 星期一 23时26分04秒
 ************************************************************************/

#ifndef _ORG_H
#define _ORG_H

#include <stdio.h>

typedef struct ORG_Interpreter_tag ORG_Interpreter;

ORG_Interpreter *ORG_create_interpreter(void);
void ORG_compile(ORG_Interpreter *interpreter, FILE *fp);
void ORG_interpret(ORG_Interpreter *interpreter);
void ORG_dispose_interpreter(ORG_Interpreter *interpreter);

#endif
