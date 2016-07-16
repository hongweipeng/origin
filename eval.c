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

//变量名出现在表达式中,此函数被调用
static ORG_Value eval_identifier_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    ORG_Value v;
    Variable *vp;

    //首先查找局部变量
    vp = org_search_local_variable(env, expr->u.identifier);
    if (vp != NULL) {
        v = vp->value;
    } else {
        //如果没有,则通过全局查找
        vp = search_global_variable_form_env(inter, env, expr->u.identifier);
        if (vp != NULL) {
            v = vp->value;
        } else {
            //仍然没有则报错
            //runtime error
            exit(1);
        }
    }
    refer_if_string(&v);
    return  v;
}

static ORG_Value eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr);

/**
 * 变量赋值时调用
 * identifier
 */
static ORG_Value eval_assign_expression(ORG_Interpreter *inter, LocalEnvironment *env, char *identifier, Expression *expression) {
    ORG_Value v;
    Variable *left;

    //获得右侧表达式结果
    v = eval_expression(inter, env, expression);

    //查找局部变量
    left = org_search_local_variable(env, identifier);

    if (left == NULL) {//找不到在全局查找
        left = search_global_variable_form_env(inter, env, identifier);
    }

    if (left != NULL) {
        release_if_string(&left->value);
        left->value = v;
        refer_if_string(&v);
    } else { // 没有找到变量,注册新的变量
        if (env != NULL) {
            // 函数内的布局变量
            org_add_local_variable(env, identifier, &v);
        } else {
            // 函数外的全局变量
            ORG_add_global_variable(inter, identifier, &v);
        }
        refer_if_string(&v);
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

    if (dkc_is_math_operator(op)) {
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

static void eval_binary_double(ORG_Interpreter *inter, ExpressionType op, double left, double right,
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

    if (op == EQ_EXPRESSION) {
        result = (cmp == 0);
    } else if (op == NE_EXPRESSION) {
        result = (cmp != 0);
    } else if (op == GT_EXPRESSION) {
        result = (cmp > 0);
    } else if (op == GE_EXPRESSION) {
        result = (cmp >= 0);
    } else if (op == LT_EXPRESSION) {
        result = (cmp < 0);
    } else if (op == LE_EXPRESSION) {
        result = (cmp <= 0);
    } else {
        char *op_str = org_get_operator_string(op);
        //runtime error
    }
    org_release_string(left->u.string_value);
    org_release_string(right->u.string_value);

    return result;
}

static ORG_Boolean eval_binary_null(ORG_Interpreter *inter, ExpressionType op, ORG_Value *left, ORG_Value *right, int line_number) {

    ORG_Boolean result;
    if (op == EQ_EXPRESSION) {
        result = (left->type == ORG_NULL_VALUE && right->type == ORG_NULL_VALUE);
    } else if (op == NE_EXPRESSION) {
        result = !(left->type == ORG_NULL_VALUE && right->type == ORG_NULL_VALUE);
    } else {
        char *op_str = org_get_operator_string(op);
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

/**
 * 函数对所有二元运算进行评估
 */
ORG_Value org_eval_binary_expression(ORG_Interpreter *inter, LocalEnvironment *env,
                                     ExpressionType op, Expression *left, Expression *right) {

    ORG_Value left_val;
    ORG_Value right_val;
    ORG_Value result;

    left_val = eval_expression(inter, env, left);
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
            right_str = org_create_origin_string(inter, MEM_strdup(buf));
        } else if(right_val.type == ORG_DOUBLE_VALUE) {
            sprintf(buf, "%lf", right_val.u.double_value);
            right_str = org_create_origin_string(inter, MEM_strdup(buf));
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
        result.u.boolean_value = eval_binary_null(inter, op, &left_val, &right_val, left->line_number);
    } else {
        char *op_str = org_get_operator_string(op);
        //runtime error
        exit(1);
    }

    return result;

}

// 布尔运算 支持短路求值
static ORG_Value eval_logical_and_or_expression(ORG_Interpreter *inter, LocalEnvironment *env, ExpressionType op, Expression *left, Expression *right) {
    ORG_Value left_val;
    ORG_Value right_val;
    ORG_Value result;

    result.type = ORG_BOOLEAN_VALUE;
    left_val = eval_expression(inter, env, left);

    if (left_val.type != ORG_BOOLEAN_VALUE) {
        //runtime error
        exit(1);
    }

    if (op == LOGICAL_AND_EXPRESSION) {
        if (!left_val.u.boolean_value) {
            result.u.boolean_value = ORG_FALSE;
            return result;//短路求值
        }
    } else if (op == LOGICAL_OR_EXPRESSION) {
        if (left_val.u.boolean_value) {
            result.u.boolean_value = ORG_TRUE;
            return result;
        }
    } else {
        //debug 不是布尔运算符
    }

    right_val = eval_expression(inter, env, right);
    if (right_val.type != ORG_BOOLEAN_VALUE) {
        //runtime error
        exit(1);
    }

    result.u.boolean_value = right_val.u.boolean_value;
    return result;
}

// 单元取反
ORG_Value org_eval_minus_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *operand) {
    ORG_Value operand_val; //操作数
    ORG_Value result;

    operand_val = eval_expression(inter, env, operand);
    if (operand_val.type == ORG_INT_VALUE) {
        result.type = ORG_INT_VALUE;
        result.u.int_value = - operand_val.u.int_value;
    } else if (operand_val.type == ORG_DOUBLE_VALUE) {
        result.type = ORG_DOUBLE_VALUE;
        result.u.double_value = -operand_val.u.double_value;
    } else {
        //runtime error
        exit(1);
    }
    return result;
}

static LocalEnvironment *alloc_local_environment() {
    LocalEnvironment *ret;
    ret = MEM_malloc(sizeof(LocalEnvironment));
    ret->variable = NULL;
    ret->global_variable = NULL;
    return ret;
}

//处理
static void dispose_local_environment(ORG_Interpreter *inter, LocalEnvironment *env) {
    while (env->variable) {
        Variable *temp;
        temp = env->variable;
        if (env->variable->value.type == ORG_STRING_VALUE) {
            org_release_string(env->variable->value.u.string_value);
        }
        env->variable = temp->next;
        MEM_free(temp);
    }

    while (env->global_variable) {
        GlobalVariableRef *ref;
        ref = env->global_variable;
        env->global_variable = ref->next;
        MEM_free(ref);
    }

    MEM_free(env);
}

static ORG_Value call_native_function(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr, ORG_NativeFunctionPro *proc) {
    //proc 是个函数指针
    ORG_Value value;
    int arg_count;
    ArgumentList *arg_p;
    ORG_Value *args;
    int i;

    for (arg_count = 0, arg_p = expr->u.function_call_expression.argument; arg_p; arg_p = arg_p->next) {
        arg_count++;
    }

    args = MEM_malloc(sizeof(ORG_Value) * arg_count);

    for (i = 0, arg_p = expr->u.function_call_expression.argument; arg_p; arg_p = arg_p->next) {
        args[i] = eval_expression(inter, env, arg_p->expression);
        i++;
    }
    value = proc(inter, arg_count, args);
    for (i = 0; i < arg_count; i++) {
        release_if_string(&args[i]);
    }
    MEM_free(args);

    return value;
}

// 函数调用
static ORG_Value call_origin_function(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr, FunctionDefine *func) {
    ORG_Value value;
    StatementResult result;
    ArgumentList *arg_p;
    ParameterList *param_p;
    LocalEnvironment *local_env;

    //开辟空间用于存放被调用的函数的局部变量
    local_env = alloc_local_environment();
    /**
     * 对参数进行评估,并用于存放到函数的局部变量中
     * arg_p : 指向函数调用的实参链表
     * param_p : 指向函数定义的形参链表
     */
    for (arg_p = expr->u.function_call_expression.argument, param_p = func->u.origin_f.parameter;
            arg_p;
            arg_p = arg_p->next, param_p = param_p->next) {

        ORG_Value arg_val;
        if (param_p == NULL) { // param_p被用尽,说明实参过多
            //runtime error
            exit(1);
        }
        arg_val = eval_expression(inter, env, arg_p->expression);
        org_add_local_variable(local_env, param_p->name, &arg_val);
    }

    if (param_p) { // param_p 剩余,说明实参数量不够
        //runtime error
        exit(1);
    }
    //执行函数内部的语句
    result = org_execute_statement_list(inter, env, func->u.origin_f.block->statement_list);

    if (result.type == RETURN_STATEMENT_RESULT) {
        value = result.u.return_value;
    } else {
        //若函数没有return，则默认有null的返回
        value.type = ORG_NULL_VALUE;
    }
    dispose_local_environment(inter, local_env);
    return value;
}

/**
 * 调用函数时执行
 */
static ORG_Value eval_function_call_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    ORG_Value value;
    FunctionDefine *func;

    char *identifier = expr->u.function_call_expression.identifier;
    func = org_search_function(identifier);
    if (func == NULL) {
        //runtime error
        exit(1);
    }

    switch (func->type) {
        case ORIGIN_FUNCTION_DEF:
            value = call_origin_function(inter, env, expr, func);
            break;
        case NATIVE_FUNCTION_DEF:
            value = call_native_function(inter, env, expr, func->u.native_f.pro);
            break;
        default:
            break;
    }

    return value;
}

// 运行表达式 或者表达式结果
static ORG_Value eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    ORG_Value v;
    switch (expr->type) {
        case BOOLEAN_EXPRESSION:
            v = eval_boolean_expression(expr->u.boolean_value);
            break;
        case INT_EXPRESSION:
            v = eval_int_expression(expr->u.int_value);
            break;
        case DOUBLE_EXPRESSION:
            v = eval_double_expression(expr->u.double_value);
            break;
        case STRING_EXPRESSION:
            v = eval_string_expression(inter, expr->u.string_value);
            break;
        case IDENTIFIER_EXPRESSION:
            v = eval_identifier_expression(inter, env, expr);
            break;
        case ASSIGN_EXPRESSION:
            v = eval_assign_expression(inter, env, expr->u.assign_expression.variable, expr->u.assign_expression.operand);
            break;
        case ADD_EXPRESSION:        /* fail */
        case SUB_EXPRESSION:        /* fail */
        case MUL_EXPRESSION:        /* fail */
        case DIV_EXPRESSION:        /* fail */
        case MOD_EXPRESSION:        /* fail */
        case EQ_EXPRESSION: /* fail */
        case NE_EXPRESSION: /* fail */
        case GT_EXPRESSION: /* fail */
        case GE_EXPRESSION: /* fail */
        case LT_EXPRESSION: /* fail */
        case LE_EXPRESSION:
            v = org_eval_binary_expression(inter, env, expr->type, expr->u.binary_expression.left, expr->u.binary_expression.right);
            break;
        case LOGICAL_AND_EXPRESSION:/* FALLTHRU */
        case LOGICAL_OR_EXPRESSION:
            v = eval_logical_and_or_expression(inter, env, expr->type, expr->u.binary_expression.left, expr->u.binary_expression.right);
            break;
        case MINUS_EXPRESSION:
            v = org_eval_minus_expression(inter, env, expr->u.minus_expression);
            break;
        case FUNCTION_CALL_EXPRESSION:
            v = eval_function_call_expression(inter, env, expr);
            break;
        case NULL_EXPRESSION:
            v = eval_null_expression();
            break;
        case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
        default:
            //DBG_panic(("bad case. type..%d\n", expr->type));
            printf("");
    }
    return v;
}

ORG_Value org_eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    return eval_expression(inter, env, expr);
}








