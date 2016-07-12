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

//布尔表达式
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

static void release_if_string(ORG_Value *v) {
    if (v->type == ORG_STRING_VALUE) {
        org_release_string(v->u.string_value);
    }
}

static Variable *search_global_variable_form_env(ORG_Interpreter *inter, LocalEnvironment *env, char *name) {
    GlobalVariableRef *pos;
    if (env == NULL) {
        return org_search_global_variable(inter, name);
    }

    for (pos = env->global_variable; pos; pos = pos->next) {
        if (!strcmp(pos->variable->name, name)) {
            return pos->variable;
        }
    }

    return NULL;
}

static ORG_Value eval_identifier_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    ORG_Value v;
    Variable *vp;

    vp = org_search_local_variable(inter, env, expr->u.identifier);
    if (vp != NULL) {
        v = vp->value;
    } else {
        vp = search_global_variable_form_env(inter, env, expr->u.identifier);
        if (vp != NULL) {
            v = vp->value;
        } else {
            //runtime error
            exit(1);
        }
    }
    refer_if_string(&v);
    return  v;
}

static ORG_Value eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr);

/**
 * identifier
 */
static ORG_Value eval_assgin_expression(ORG_Interpreter *inter, LocalEnvironment *env, char *identifier, Expression *expression) {
    ORG_Value v;
    Variable *left;
    v = eval_expression(inter, env, expression);

    left = org_search_local_variable(env, identifier);

    if (left == NULL) {
        left = search_global_variable_form_env(inter, env, identifier);
    }

    if (left != NULL) {
        release_if_string(&left->value);
        left->value = v;
        refer_if_string(&v);
    } else {
        if (env != NULL) {
            org_add_local_variable(env, identifier, &v);
        } else {
            ORG_add_global_variable(inter, identifier, &v);
        }
        //refer_if_string(&v);
    }
    return v;
}

static ORG_Boolean eval_binary_boolean(ORG_Interpreter *inter, ExpressionType op, ORG_Boolean left,
                                       ORG_Boolean right, int line_number) {

    ORG_Boolean result;
    if (op == EQ_EXPRESSION) {
        result = (left == right);
    } else if (op == NE_EXPRESSION) {
        result = (left != right);
    } else {
        char *op_str = org_get_operator_string(op);
        //run time error
    }

    return result;
}

static void eval_binary_int(ORG_Interpreter *inter, ExpressionType op,
                            int left, int right, ORG_Value *result, int line_number) {

    if (dkc_is_match_operator(op)) {
        result->type = ORG_INT_VALUE;
    } else if (dkc_is_compare_operator(op)) {
        result = ORG_BOOLEAN_VALUE;
    } else {
        //debug
    }

    switch (op) {
        case BOOLEAN_EXPRESSION:
        case INT_EXPRESSION:
        case DOUBLE_EXPRESSION:
        case STRING_EXPRESSION:
        case IDENTIFIER_EXPRESSION:
        case ASSIGN_EXPRESSION:
            //debug
            break;
        case ADD_EXPRESSION:
            result->u.int_value = left + right;
            break;
        case SUB_EXPRESSION:
            result->u.int_value = left - right;
            break;
        case MUL_EXPRESSION:
            result->u.int_value = left * right;
            break;
        case DIV_EXPRESSION:
            result->u.int_value = left / right;
        case MOD_EXPRESSION:
            result->u.int_value = left % right;
            break;
        case LOGICAL_AND_EXPRESSION:
        case LOGICAL_OR_EXPRESSION:
            //debug
            break;
        case EQ_EXPRESSION:
            result->u.int_value = left == right;
            break;


    }
}


















