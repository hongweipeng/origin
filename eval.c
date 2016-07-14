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
        case NE_EXPRESSION:
            result->u.boolean_value = left != right;
            break;
        case GT_EXPRESSION:
            result->u.boolean_value = left > right;
            break;
        case GE_EXPRESSION:
            result->u.boolean_value = left >= right;
            break;
        case LT_EXPRESSION:
            result->u.boolean_value = left < right;
            break;
        case LE_EXPRESSION:
            result->u.boolean_value = left <= right;
            break;
        case MINUS_EXPRESSION:              /* FALLTHRU */
        case FUNCTION_CALL_EXPRESSION:      /* FALLTHRU */
        case NULL_EXPRESSION:               /* FALLTHRU */
        case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
        default:
            printf("");
            //DBG_panic(("bad case...%d", operator));
    }
}

static void eval_binary_double(ORG_Interpreter *inter, ExpressionType *op, double left, double right,
                               ORG_Value *result, int line_number) {

    if (dkc_is_math_operator(op)) {
        result->type = ORG_BOOLEAN_VALUE;
    } else if (dkc_is_compare_operator(op)) {
        result->type = ORG_BOOLEAN_VALUE;
    } else {
        //debug
    }

    switch (op) {
        case BOOLEAN_EXPRESSION:    /* fail */
        case INT_EXPRESSION:        /* fail */
        case DOUBLE_EXPRESSION:     /* fail */
        case STRING_EXPRESSION:     /* fail */
        case IDENTIFIER_EXPRESSION: /* fail */
        case ASSIGN_EXPRESSION:
            //DBG_panic(("bad case...%d", operator));
            break;
        case ADD_EXPRESSION:
            result->u.double_value = left + right;
            break;
        case SUB_EXPRESSION:
            result->u.double_value = left - right;
            break;
        case MUL_EXPRESSION:
            result->u.double_value = left * right;
            break;
        case DIV_EXPRESSION:
            result->u.double_value = left / right;
            break;
        case MOD_EXPRESSION:
            result->u.double_value = fmod(left, right);
            break;
        case LOGICAL_AND_EXPRESSION:        /* fail */
        case LOGICAL_OR_EXPRESSION:
            //DBG_panic(("bad case...%d", operator));
            break;
        case EQ_EXPRESSION:
            result->u.int_value = left == right;
            break;
        case NE_EXPRESSION:
            result->u.int_value = left != right;
            break;
        case GT_EXPRESSION:
            result->u.int_value = left > right;
            break;
        case GE_EXPRESSION:
            result->u.int_value = left >= right;
            break;
        case LT_EXPRESSION:
            result->u.int_value = left < right;
            break;
        case LE_EXPRESSION:
            result->u.int_value = left <= right;
            break;
        case MINUS_EXPRESSION:              /* fail */
        case FUNCTION_CALL_EXPRESSION:      /* fail */
        case NULL_EXPRESSION:               /* fail */
        case EXPRESSION_TYPE_COUNT_PLUS_1:  /* fail */
        default:
            printf("");
            //DBG_panic(("bad default...%d", operator));
    }

}

static ORG_Boolean eval_compare_string(ExpressionType op, ORG_Value *left, ORG_Value *right, int line_number) {
    ORG_Boolean result;
    int cmp;
    cmp = strcmp(left->u.string_value, right->u.string_value);

    if (operator == EQ_EXPRESSION) {
        result = (cmp == 0);
    } else if (operator == NE_EXPRESSION) {
        result = (cmp != 0);
    } else if (operator == GT_EXPRESSION) {
        result = (cmp > 0);
    } else if (operator == GE_EXPRESSION) {
        result = (cmp >= 0);
    } else if (operator == LT_EXPRESSION) {
        result = (cmp < 0);
    } else if (operator == LE_EXPRESSION) {
        result = (cmp <= 0);
    } else {
        char *op_str = crb_get_operator_string(operator);
        //runtime error
    }
    crb_release_string(left->u.string_value);
    crb_release_string(right->u.string_value);

    return result;
}

static ORG_Boolean eval_binary_null(ORG_Interpreter *inter, ExpressionType op, ORG_Value *left, ORG_Value *right, int line_number) {

    ORG_Boolean result;
    if (op == EQ_EXPRESSION) {
        result = (left->type == ORG_NULL_VALUE && right->type == ORG_NULL_VALUE);
    } else if (operator == NE_EXPRESSION) {
        result = !(left->type == CRB_NULL_VALUE && right->type == CRB_NULL_VALUE);
    } else {
        char *op_str = crb_get_operator_string(operator);
        //runtime error
    }
    release_if_string(left);
    release_if_string(right);

    return result;
}

ORG_String *chain_string(ORG_Interpreter *inter, ORG_String *left, ORG_String *right) {
    //like  "123" + "adc"
    int len;
    char *str;
    ORG_String *ret;

    len = strlen(left->string) + strlen(right->string);
    str = MEM_malloc(len + 1);
    strcpy(str, left->string);
    strcat(str, right->string);
    ret = org_create_origin_string(inter, str);

    org_release_string(left->string);
    org_release_string(right->string);
    return ret;
}

ORG_Value org_eval_binary_expression(ORG_Interpreter *inter, LocalEnvironment *env,
                                     ExpressionType op, Expression *left, Expression *right) {

    ORG_Value left_val;
    ORG_Value right_val;
    ORG_Value result;

    left_val = eval_expression(iner, env, left);
    right_val = eval_expression(inter, env, right);


    /**
     * 对于加法运算
     * 1 整数 + 整数   返回整数
     * 2 实数 + 实数   返回实数
     * 3 整数 + 实数   左边转换为实数
     * 4 实数 + 整数   右边转换为实数
     * 5 字符串 + 字符串 返回连接后的字符串
     * 6 字符串 + 整数   右边转换为字符串
     * 7 字符串 + 实数    右边转换为字符串
     * 8 字符串 + 布尔    右边转换为true或false的字符串
     * 9 字符串 + null    右边转换为null的字符串
     */
    if (left_val.type == ORG_INT_VALUE && right_val.type == ORG_INT_VALUE) { //  1 + 2
        eval_binary_int(inter, op, left_val.u.int_value, right_val.u.int_value, &result, left->line_number);
    } else if (left_val.type == ORG_DOUBLE_VALUE && right_val.type == ORG_DOUBLE_VALUE) { // 1.0 + 2.3
        eval_binary_double(inter, op , left_val.u.double_value, right_val.u.double_value, &result, left->line_number);
    } else if (left_val.type == ORG_INT_VALUE && right_val.type == ORG_DOUBLE_VALUE) { // 1 + 2.0
        left_val.u.double_value = (double) left_val.u.int_value;
        eval_binary_double(inter, op , left_val.u.double_value, right_val.u.double_value, &result, left->line_number);
    } else if (left_val.type == ORG_DOUBLE_VALUE && right_val.type == ORG_INT_VALUE) { // 3.0 + 1
        right_val.u.double_value = (double) right_val.u.int_value;
        eval_binary_double(inter, op , left_val.u.double_value, right_val.u.double_value, &result, left->line_number);
    } else if (left_val.type == ORG_STRING_VALUE && op == ADD_EXPRESSION) {
        char buf[LINE_BUF_SIZE];
        ORG_String *right_str;

        if (right_val.type == ORG_INT_VALUE) {
            sprintf(buf, "%d", right_val.u.int_value);
            right_str = org_create_origin_string(inter, MEM_strdum(buf));
        } else if(right_val.type == ORG_DOUBLE_VALUE) {
            sprintf(buf, "%lf", right_val.u.double_value);
            right_str = org_create_origin_string(inter, MEM_strdum(buf));
        } else if (right_val.type == ORG_BOOLEAN_VALUE) {
            right_str = (right_val.u.boolean_value) ? org_create_origin_string(inter, MEM_strdup("true")) : org_create_origin_string(inter, MEM_strdup("false"));
        } else if (right_val.type == ORG_STRING_VALUE) {
            right_str = right_val.u.string_value;
        }

        result.type = ORG_STRING_VALUE;
        result.u.string_value = chain_string(inter, left_val.u.string_value, right_str);
    } else if (left_val.type == ORG_BOOLEAN_VALUE && right_val.type == ORG_BOOLEAN_VALUE) {
        //

        result.type = ORG_BOOLEAN_VALUE;
        result.u.boolean_value = eval_binary_boolean(inter, op, left_val.u.boolean_value, right_val.u.boolean_value, left->line_number);
    } else if (left_val.type == ORG_STRING_VALUE && right_val.type == ORG_STRING_VALUE) {
        // "12" > "abc"
        result.type == ORG_BOOLEAN_VALUE;
        result.u.boolean_value = eval_compare_string(op, &left_val, &right_val, left->line_number);
    } else if (left_val.type == ORG_NULL_VALUE || right_val.type == ORG_NULL_VALUE) {
        result.type = ORG_NULL_VALUE;
        result.u.boolean_value = eval_binary_null(inter, op, &left_val, &right_val);
    } else {
        char *op_str = org_get_operator_string(op);
        //runtime error
        exit(1);
    }

    return result;

}

static ORG_Value eval_logical_and_or_express(ORG_Interpreter *inter, LocalEnvironment *env, ExpressionType op, Expression *left, Expression *right) {
    ORG_Value left_val;
    ORG_Value right_val;
    ORG_Value result;

    result.type = ORG_BOOLEAN_VALUE;
    left_val = eval_expression(inter, env, left);
}











