/*************************************************************************
	> File Name: eval.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月11日 星期一 22时29分53秒
 ************************************************************************/
/* 表达式求值 */
#include <math.h>
#include <string.h>
#include "ORG.h"
#include "origin.h"

static ORG_Value eval_boolean_expression(ORG_Boolean boolean_value) {
    ORG_Value v;
    v.type = ORG_BOOLEAN_VALUE;
    v.u.boolean_value = boolean_value;
    return v;
}

static  ORG_Value eval_int_expression(int int_value) {
    ORG_Value v;
    v.type = ORG_INT_VALUE;
    v.u.int_value = int_value;
    return v;
}

static ORG_Value eval_double_expression(double double_value) {
    ORG_Value v;
    v.type = ORG_DOUBLE_VALUE;
    v.u.double_value = double_value;
    return  v;
}

static  ORG_Value eval_string_expression(ORG_Interpreter *inter, char *string_value) {
    ORG_Value v;
    v.type = ORG_STRING_VALUE;
    v.u.string_value = org_literal_to_org_string(inter, string_value);
    return v;
}

static ORG_Value eval_null_expression(void) {
    ORG_Value v;
    v.type = ORG_NULL_VALUE;
    return v;
}

static void refer_if_string(ORG_Value *v) {
    if (v->type == ORG_STRING_VALUE) {
        org_refer_string(v->u.string_value);
    }
}

























