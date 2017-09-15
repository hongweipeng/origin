#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "origin.h"

ORG_Interpreter *ORG_create_interpreter(void) {
    ORG_Interpreter *interpreter;
    interpreter = (ORG_Interpreter *)malloc(sizeof(ORG_Interpreter));

    interpreter->variable = NULL;
    interpreter->statement_list = NULL;

    org_set_current_interpreter(interpreter);
    //add_native_functions(interpreter);

    return interpreter;
}

// 编译
void ORG_compile(ORG_Interpreter *inter, FILE *fp) {
    extern int yyparse(void);
    extern FILE  *yyin;
    //org_set_current_interpreter(inter);

    yyin = fp;
    if (yyparse()) {
        if (inter->statement_list) {
            fprintf(stderr, "ORG_compile Error\n");
        }
    }
    //org_reset_string_literal_buffer();
}

void ORG_interpret_run(ORG_Interpreter *inter) {
    //inter->execute_storage = MEM_open_storage(0);
    //org_add_std_fp(inter);
    org_execute_statement_list(inter, NULL, inter->statement_list);
}

int main(int argc, char **argv) {
    ORG_Interpreter     *interpreter;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }
    interpreter = ORG_create_interpreter();
    ORG_compile(interpreter, fp);
    ORG_interpret_run(interpreter);
    fclose(fp);
    return 0;
}