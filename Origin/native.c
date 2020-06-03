/*************************************************************************
	> File Name: native.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 17时18分31秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "ORG_dev.h"
#include "origin.h"

#define NATIVE_LIB_NAME "crowbar.lang.file"

static ORG_NativePointerInfo st_native_lib_info = {
        NATIVE_LIB_NAME
};

ORG_Value org_nv_print_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args) {
    ORG_Value value;

    value.type = ORG_NULL_VALUE;
    /*size_t size = 0;*/
    /*char *ready_print;*/

    if (arg_count < 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_FEW_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    } else if (arg_count > 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_MANY_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }

    switch (args[0].type) {
        case ORG_BOOLEAN_VALUE:
            if (args[0].u.boolean_value) {
                printf("true");
            } else {
                printf("false");
            }
            break;
        case ORG_INT_VALUE:
            printf("%d", args[0].u.int_value);
            break;
        case ORG_DOUBLE_VALUE:
            printf("%f", args[0].u.double_value);
            break;
        case ORG_STRING_VALUE:
            printf("%s", args[0].u.string_value->string);
            break;
        case ORG_NATIVE_POINTER_VALUE:
            printf("(%s:%p)",
                   args[0].u.native_pointer.info->name,
                   args[0].u.native_pointer.pointer);
            break;
        case ORG_NULL_VALUE:
            printf("null");
            break;
    }

    return value;
}

ORG_Value org_nv_fopen_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args) {
    ORG_Value value;
    FILE *fp;

    if (arg_count < 2) {
        /*org_runtime_error(0, ARGUMENT_TOO_FEW_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    } else if (arg_count > 2) {
        /*org_runtime_error(0, ARGUMENT_TOO_MANY_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    if (args[0].type != ORG_STRING_VALUE || args[1].type != ORG_STRING_VALUE) {
        /*org_runtime_error(0, FOPEN_ARGUMENT_TYPE_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }

    fp = fopen(args[0].u.string_value->string, args[1].u.string_value->string);
    if (fp == NULL) {
        value.type = ORG_NULL_VALUE;
    } else {
        value.type = ORG_NATIVE_POINTER_VALUE;
        value.u.native_pointer.info = &st_native_lib_info;
        value.u.native_pointer.pointer = fp;
    }

    return value;
}

static ORG_Boolean check_native_pointer(ORG_Value *value) {
    return value->u.native_pointer.info == &st_native_lib_info;
}

ORG_Value org_nv_fclose_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args) {
    ORG_Value value;
    FILE *fp;

    value.type = ORG_NULL_VALUE;
    if (arg_count < 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_FEW_ERR,MESSAGE_ARGUMENT_END);*/
        exit(1);
    } else if (arg_count > 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_MANY_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    if (args[0].type != ORG_NATIVE_POINTER_VALUE || !check_native_pointer(&args[0])) {
        /*org_runtime_error(0, FCLOSE_ARGUMENT_TYPE_ERR,MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    fp = args[0].u.native_pointer.pointer;
    fclose(fp);

    return value;
}

ORG_Value org_nv_fgets_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args) {
    ORG_Value value;
    FILE *fp;
    char buf[LINE_BUF_SIZE];
    char *ret_buf = NULL;
    int ret_len = 0;

    if (arg_count < 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_FEW_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    } else if (arg_count > 1) {
        /*org_runtime_error(0, ARGUMENT_TOO_MANY_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    if (args[0].type != ORG_NATIVE_POINTER_VALUE || !check_native_pointer(&args[0])) {
        /*org_runtime_error(0, FGETS_ARGUMENT_TYPE_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    fp = args[0].u.native_pointer.pointer;

    while (fgets(buf, LINE_BUF_SIZE, fp)) {
        int new_len;
        new_len = ret_len + strlen(buf);
        ret_buf = MEM_realloc(ret_buf, new_len + 1);
        if (ret_len == 0) {
            strcpy(ret_buf, buf);
        } else {
            strcat(ret_buf, buf);
        }
        ret_len = new_len;
        if (ret_buf[ret_len-1] == '\n')
            break;
    }
    if (ret_len > 0) {
        value.type = ORG_STRING_VALUE;
        value.u.string_value = org_create_origin_string(interpreter, ret_buf);
    } else {
        value.type = ORG_NULL_VALUE;
    }

    return value;
}

ORG_Value org_nv_fputs_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args) {
    ORG_Value value;
    FILE *fp;

    value.type = ORG_NULL_VALUE;
    if (arg_count < 2) {
        /*org_runtime_error(0, ARGUMENT_TOO_FEW_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    } else if (arg_count > 2) {
        /*org_runtime_error(0, ARGUMENT_TOO_MANY_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    if (args[0].type != ORG_STRING_VALUE || (args[1].type != ORG_NATIVE_POINTER_VALUE || !check_native_pointer(&args[1]))) {
        /*org_runtime_error(0, FPUTS_ARGUMENT_TYPE_ERR, MESSAGE_ARGUMENT_END);*/
        exit(1);
    }
    fp = args[1].u.native_pointer.pointer;

    fputs(args[0].u.string_value->string, fp);

    return value;
}

void org_add_std_fp(ORG_Interpreter *inter) {
    ORG_Value fp_value;

    fp_value.type = ORG_NATIVE_POINTER_VALUE;
    fp_value.u.native_pointer.info = &st_native_lib_info;

    fp_value.u.native_pointer.pointer = stdin;
    ORG_add_global_variable(inter, "STDIN", &fp_value);

    fp_value.u.native_pointer.pointer = stdout;
    ORG_add_global_variable(inter, "STDOUT", &fp_value);

    fp_value.u.native_pointer.pointer = stderr;
    ORG_add_global_variable(inter, "STDERR", &fp_value);
}
