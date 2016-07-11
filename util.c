/*************************************************************************
	> File Name: util.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月10日 星期日 12时19分21秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "origin.h"

//通用工具包


static ORG_Interpreter *st_current_interperter;

ORG_Interpreter *org_get_current_interperter(void) {
    return st_current_interperter;
}

//获取函数
FunctionDefine *org_search_function(char *name) {
    FunctionDefine *pos;
    ORG_Interpreter *inter;
    inter = org_get_current_interpreter();

    for (pos = inter->function_list; pos != NULL; pos = pos->next) {
        if (!strcmp(pos->name, name)) {
            break;
        }
    }
    return pos;
}

//申请内存空间
void *org_malloc(size_t size) {
    void *p;
    ORG_Interpreter *inter;
    inter = org_get_current_interpreter();
    p = MEM_storage_malloc(inter->interpreter_storage, size);
    return p;
}

void *org_execute_malloc(ORG_Interpreter *inter, size_t size) {
    void *p;
    p = MEM_storage_malloc(inter->execute_storage, size);
    return p;
}

//获取变量
Variable *org_search_local_variable(LocalEnvironment *env, char *identifier) {
    Variable *pos;
    if (env == NULL) {
        return NULL;
    }

    for (pos = env->variable; pos != NULL; pos = pos->next) {
        if(!strcmp(pos->name, identifier)) {
            break;
        }
    }
    return pos;
}

//获取全局变量
Variable *org_search_global_variable(ORG_Interpreter *inter, char *identifier) {
    Variable *pos;
    for (pos = inter->variable; pos != NULL; pos = pos->next) {
        if (!strcmp(pos->name, identifier)) {
            break;
        }
    }
    return pos;
}

//添加变量
void org_add_local_variable(LocalEnvironment *env, char *identifier, ORG_Value *value) {
    Variable *new_variable;
    new_variable = MEM_malloc(sizeof(Variable));
    new_variable->name = identifier;
    new_variable->value = *value;
    new_variable->next = env->variable;
    env->variable = new_variable;
}

//添加全局变量
void ORG_add_global_variable(ORG_Interpreter *inter, char *identifier, ORG_Value *value) {
    Variable *new_variable;
    new_variable = org_execute_malloc(inter, sizeof(Variable));
    new_variable->name = org_execute_malloc(inter, strlen(identifier) + 1);
    strcpy(new_variable->name, identifier);
    new_variable->value = *value;
    new_variable->next = inter->variable;
    inter->variable = new_variable;
}

char *org_get_operator_string(ExpressionType type)
{
    char        *str;

    switch (type) {
        case BOOLEAN_EXPRESSION:    /* FALLTHRU */
        case INT_EXPRESSION:        /* FALLTHRU */
        case DOUBLE_EXPRESSION:     /* FALLTHRU */
        case STRING_EXPRESSION:     /* FALLTHRU */
        case IDENTIFIER_EXPRESSION:
            //DBG_panic(("bad expression type..%d\n", type));
            break;
        case ASSIGN_EXPRESSION:
            str = "=";
            break;
        case ADD_EXPRESSION:
            str = "+";
            break;
        case SUB_EXPRESSION:
            str = "-";
            break;
        case MUL_EXPRESSION:
            str = "*";
            break;
        case DIV_EXPRESSION:
            str = "/";
            break;
        case MOD_EXPRESSION:
            str = "%";
            break;
        case LOGICAL_AND_EXPRESSION:
            str = "&&";
            break;
        case LOGICAL_OR_EXPRESSION:
            str = "||";
            break;
        case EQ_EXPRESSION:
            str = "==";
            break;
        case NE_EXPRESSION:
            str = "!=";
            break;
        case GT_EXPRESSION:
            str = "<";
            break;
        case GE_EXPRESSION:
            str = "<=";
            break;
        case LT_EXPRESSION:
            str = ">";
            break;
        case LE_EXPRESSION:
            str = ">=";
            break;
        case MINUS_EXPRESSION:
            str = "-";
            break;
        case FUNCTION_CALL_EXPRESSION:  /* FALLTHRU */
        case NULL_EXPRESSION:  /* FALLTHRU */
        case EXPRESSION_TYPE_COUNT_PLUS_1:
        default:
            printf("bad expression type..\n");
            //DBG_panic(("bad expression type..%d\n", type));
    }

    return str;
}




