/*************************************************************************
	> File Name: string_pool.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 09时33分49秒
字符串通过 + 连接时,需要配合垃圾回收机制,本程序通过引用计数方式进行回收
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "origin.h"
#include "ORG_dev.h"

static ORG_String *alloc_org_string(ORG_Interpreter *inter, char *str, ORG_Boolean is_literal) {
    ORG_String *ret;

    ret = MEM_malloc(sizeof(ORG_String));
    ret->ref_count = 0;
    ret->is_literal = is_literal;
    ret->string = str;
    return  ret;
}

/* 将字面字符串转为ORG_String*/
ORG_String *org_literal_to_org_string(ORG_Interpreter *inter, char *str) {
    ORG_String *ret;
    ret = alloc_org_string(inter, str, ORG_TRUE);
    ret->ref_count = 1;
    return ret;
}

/* 引用计数自增*/
void org_refer_string(ORG_String *str) {
    str->ref_count++;
}

/* 引用计数自减 为0时则释放*/
void org_release_string(ORG_String *str) {
    str->ref_count --;
    /*DBG*/

    if (str->ref_count == 0) {
        if (!str->is_literal) {
            MEM_free(str->string);
        }
        MEM_free(str);
    }
}

ORG_String *org_create_origin_string(ORG_Interpreter *inter, char *str) {
    ORG_String *ret;
    ret = alloc_org_string(inter, str, ORG_FALSE);
    ret->ref_count = 1;
    return ret;
}


