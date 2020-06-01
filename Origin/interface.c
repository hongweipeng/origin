/*************************************************************************
	> File Name: interface.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 15时06分08秒
 ************************************************************************/

#include "MEM.h"
#include "DBG.h"
#include "origin.h"

#define GLOBAL_VARIABLE_DEFINE

static void add_native_functions(ORG_Interpreter *inter) {
    ORG_add_native_function(inter, "print", org_nv_print_proc);
}

ORG_Interpreter *ORG_create_interpreter(void) {
    MEM_Storage storage;
    ORG_Interpreter *interpreter;

    storage = MEM_open_storage(0);
    interpreter = MEM_storage_malloc(storage, sizeof(struct ORG_Interpreter_tag));

    interpreter->interpreter_storage = storage;
    interpreter->execute_storage = NULL;
    interpreter->variable = NULL;
    interpreter->function_list = NULL;
    interpreter->statement_list = NULL;
    interpreter->current_line_number = 1;

    org_set_current_interpreter(interpreter);
    add_native_functions(interpreter);

    return interpreter;
}

// 编译
void ORG_compile(ORG_Interpreter *inter, FILE *fp) {
    extern int yyparse(void);
    extern FILE  *yyin;
    org_set_current_interpreter(inter);

    yyin = fp;
    if (yyparse()) {
        fprintf(stderr, "Error Error\n");
    }
    org_reset_string_literal_buffer();
}

void ORG_interpret(ORG_Interpreter *inter) {
    inter->execute_storage = MEM_open_storage(0);
    org_add_std_fp(inter);
    org_execute_statement_list(inter, NULL, inter->statement_list);
}

static void release_global_strings(ORG_Interpreter *inter) {
    while (inter->variable) {
        Variable *tmp = inter->variable;
        inter->variable = tmp->next;
        if(tmp->value.type == ORG_STRING_VALUE) {
            org_release_string(tmp->value.u.string_value);
        }
    }
}

void ORG_dispose_interpreter(ORG_Interpreter *inter) {
    release_global_strings(inter);

    if (inter->execute_storage) {
        MEM_dispose_storage(inter->execute_storage);
    }

    MEM_dispose_storage(inter->interpreter_storage);
}

void ORG_add_native_function(ORG_Interpreter *inter, char *name, ORG_NativeFunctionPro *pro) {
    FunctionDefine *fd;
    fd = org_malloc(sizeof(FunctionDefine));
    fd->name = name;
    fd->type = NATIVE_FUNCTION_DEF;
    fd->u.native_f.pro = pro;
    fd->next = inter->function_list;

    inter->function_list = fd;
}












