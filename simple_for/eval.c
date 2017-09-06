#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "origin.h"

static  ORG_Value eval_int_expression(int int_value) {
    ORG_Value v;
    v.type = ORG_INT_VALUE;
    v.u.int_value = int_value;
    return v;
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
    return  v;
}
static ORG_Value eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr);

//布尔表达式
static ORG_Value eval_boolean_expression(ORG_Boolean boolean_value) {
    ORG_Value v;
    v.type = ORG_BOOLEAN_VALUE;
    v.u.boolean_value = boolean_value;
    return v;
}

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
        left->value = v;
    } else { // 没有找到变量,注册新的变量
        if (env != NULL) {
            // 函数内的布局变量
            //org_add_local_variable(env, identifier, &v);
        } else {
            // 函数外的全局变量
            ORG_add_global_variable(inter, identifier, &v);
        }
    }
    return v;
}

void eval_binary_int(ORG_Interpreter *inter, ExpressionType op,
                            int left, int right, ORG_Value *result, int line_number) {

    if (dkc_is_math_operator(op)) {
        result->type = ORG_INT_VALUE;
    } else if (dkc_is_compare_operator(op)) {
        result->type = ORG_BOOLEAN_VALUE;
    } else {
        printf("op=%d", op);
        //debug
    }

    switch (op) {
        case BOOLEAN_EXPRESSION:
        case INT_EXPRESSION:
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
        case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
        default:
            //printf("");
            //DBG_panic(("bad case...%d", operator));
            break;
    }
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
    //printf("%s\n", right_val.type);
    if (left_val.type == ORG_INT_VALUE && right_val.type == ORG_INT_VALUE) { //  1 + 2
        eval_binary_int(inter, op, left_val.u.int_value, right_val.u.int_value, &result, left->line_number);
    } else {
        //char *op_str = org_get_operator_string(op);
        //runtime error

        exit(1);
    }

    return result;

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
        case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
        default:
            printf("bad case. type..%d\n", expr->type);
            break;
    }
    return v;
}


ORG_Value org_eval_expression(ORG_Interpreter *inter, LocalEnvironment *env, Expression *expr) {
    return eval_expression(inter, env, expr);
}

//执行表达式语句
static StatementResult execute_expression_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    ORG_Value value;
    result.type = NORMAL_STATEMENT_RESULT;

    value = org_eval_expression(inter, env, statement->u.expression_s);
    result.u.return_value = value;
    return result;
}

//执行if语句
static StatementResult execute_if_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    ORG_Value cond;
    result.type = NORMAL_STATEMENT_RESULT;
    cond = org_eval_expression(inter, env, statement->u.if_s.condition);
    if (cond.type != ORG_BOOLEAN_VALUE) {
        //runtime error
        printf("runtime error");
        exit(1);
    }

    if (cond.u.boolean_value) {
        result = org_execute_statement_list(inter, env, statement->u.if_s.then_block->statement_list);
    } else {
        if (statement->u.if_s.else_block) {
            result = org_execute_statement_list(inter, env, statement->u.if_s.else_block->statement_list);
        }
    }
    return result;
}

static StatementResult execute_break_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    result.type = BREAK_STATEMENT_RESULT;
    return result;
}

static StatementResult execute_for_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    ORG_Value cond;
    result.type = NORMAL_STATEMENT_RESULT;
    if (statement->u.for_s.init) {
        org_eval_expression(inter, env, statement->u.for_s.init);
    }
    for (;;) {
        //for的判断循环条件
        if (statement->u.for_s.condition) {
            cond = org_eval_expression(inter, env, statement->u.for_s.condition);
            if (cond.type != ORG_BOOLEAN_VALUE) {
                printf("runtime error\n");
                exit(1);
            }
            if (!cond.u.boolean_value) {
                break;
            }
        }
        result = org_execute_statement_list(inter, env, statement->u.for_s.block->statement_list);
        if (result.type == RETURN_STATEMENT_RESULT) {
            break;
        } else if (result.type == BREAK_STATEMENT_RESULT) {
            result.type = NORMAL_STATEMENT_RESULT;
            break;
        }

        if (statement->u.for_s.post) {
            org_eval_expression(inter, env, statement->u.for_s.post);
        }
    }
    return result;
}

//执行全局语句
static StatementResult execute_global_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    IdentifierList *pos;
    StatementResult result;
    result.type = NORMAL_STATEMENT_RESULT;
    if (env == NULL) {
        //runtime error
        printf("runtime error");
        exit(1);
    }
    for (pos = statement->u.global_s.identifier_list; pos; pos = pos->next) {
        GlobalVariableRef *ref_pos;
        GlobalVariableRef *new_ref;
        Variable *variable;
        for (ref_pos = env->global_variable; ref_pos; ref_pos = ref_pos->next) {
            if (!strcmp(ref_pos->variable->name, pos->name)) {
                goto NEXT_IDENTIFIER;
            }
        }

        variable = org_search_global_variable(inter, pos->name);
        if (variable == NULL) {
            //runtime error
            printf("runtime error");
            exit(1);
        }

        new_ref = (GlobalVariableRef *) malloc(sizeof(GlobalVariableRef));
        new_ref->variable = variable;
        new_ref->next = env->global_variable;
        env->global_variable = new_ref;

        NEXT_IDENTIFIER:
        ;
    }
    return result;
}

static StatementResult execute_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    result.type = NORMAL_STATEMENT_RESULT;
    ORG_Value v;
    switch (statement->type) {
        case EXPRESSION_STATEMENT:
            result = execute_expression_statement(inter, env, statement);
            break;
        case GLOBAL_STATEMENT:
            result = execute_global_statement(inter, env, statement);
            break;
        case IF_STATEMENT:
            result = execute_if_statement(inter, env, statement);
            break;
        case FOR_STATEMENT:
            result = execute_for_statement(inter, env, statement);
            break;
        case BREAK_STATEMENT:
            result = execute_break_statement(inter, env, statement);
            break;
        case PRINT_STATEMENT:
            v = eval_expression(inter, env, statement->u.p_s.exp);
            printf("%d", v.u.int_value);
            printf("\n");
            break;
        case STATEMENT_TYPE_CONUT_PLUS_1:
            break;
        default:
            printf("bad case");
    }
    return result;
}


//开放,所以以org开头
StatementResult org_execute_statement_list(ORG_Interpreter *inter, LocalEnvironment *env, StatementList *list) {
    StatementList *pos;
    StatementResult result;
    result.type = NORMAL_STATEMENT_RESULT;

    for (pos = list; pos; pos = pos->next) {
        result = execute_statement(inter, env, pos->statement);
        if (result.type != NORMAL_STATEMENT_RESULT) {
            return result;
        }
    }
    return result;
}
