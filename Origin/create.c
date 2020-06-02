/*************************************************************************
	> File Name: create.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月09日 星期六 23时39分01秒
 ************************************************************************/

#include "origin.h"

//创建函数
void org_function_define(char *identifier, ParameterList *parameter_list, Block *block) {
    FunctionDefine *fun;
    ORG_Interpreter *inter;

    if (org_search_function(identifier)) {
        //如果函数名不存在
        //org_compile_error(FUNCTION);
        return;
    }
    inter = org_get_current_interpreter();

    fun = org_malloc(sizeof(FunctionDefine));
    fun->name = identifier;
    fun->type = ORIGIN_FUNCTION_DEF;
    fun->u.origin_f.parameter = parameter_list;
    fun->u.origin_f.block = block;
    fun->next = inter->function_list;
    inter->function_list = fun;
}

//创建参数
ParameterList *org_create_parameter(char *identifier) {
    ParameterList *p;
    p = org_malloc(sizeof(ParameterList));
    p->name = identifier;
    p->next = NULL;
    return p;
}

ParameterList *org_chain_parameter(ParameterList *list, char *identifier) {
    ParameterList *pos;
    for (pos = list; pos->next != NULL; pos = pos->next);

    pos->next = org_create_parameter(identifier);
    return list;
}

//创建参数列表
ArgumentList *org_create_argument_list(Expression *expression) {
    ArgumentList *al;
    al = org_malloc(sizeof(ArgumentList));
    al->expression = expression;
    al->next = NULL;
    return al;
}

ArgumentList *org_chain_argument_list(ArgumentList *list, Expression *expr) {
    ArgumentList *pos;
    for (pos = list; pos->next != NULL; pos = pos->next);

    pos->next = org_create_argument_list(expr);
    return list;
}

//创建语句 数组
StatementList *org_create_statement_list(Statement *statement) {
    StatementList *sl;
    sl = org_malloc(sizeof(StatementList));
    sl->statement = statement;
    sl->next = NULL;
    return sl;
}

StatementList *org_chain_statement_list(StatementList *list, Statement *statement) {
    StatementList *pos;

    if (list == NULL) {
        return org_create_statement_list(statement);
    }

    for (pos = list; pos->next != NULL; pos = pos->next);

    pos->next = org_create_statement_list(statement);
    return list;
}

//申请存放表达式的地址空间
Expression *org_alloc_expression(ExpressionType type) {
    Expression *exp;
    exp = org_malloc(sizeof(Expression));
    exp->type = type;
    exp->line_number = org_get_current_interpreter()->current_line_number;

    return exp;
}

//创建赋值表达式
Expression *org_create_assign_expression(char *variable, Expression *operand) {
    Expression *exp;

    exp = org_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.variable = variable;
    exp->u.assign_expression.operand = operand;

    return exp;
}

//表达式换算 把任意类型换成表达式形式
static Expression convert_value_to_expression(ORG_Value *v) {
    Expression expr;

    if (v->type == ORG_INT_VALUE) {
        expr.type = INT_EXPRESSION;
        expr.u.int_value = v->u.int_value;
    }else if (v->type == ORG_DOUBLE_VALUE) {
        expr.type = DOUBLE_EXPRESSION;
        expr.u.double_value = v->u.double_value;
    } else {
        //error
        //DBG_assert(v->type = ORG_BOOLEAN_VALUE)
        expr.type = BOOLEAN_EXPRESSION;
        expr.u.boolean_value = v->u.boolean_value;
    }

    return expr;

}

//创建二元表达式
Expression *org_create_binary_expression(ExpressionType op, Expression *left, Expression *right) {
    if ((left->type == INT_EXPRESSION || left->type == DOUBLE_EXPRESSION)
            && (right->type == INT_EXPRESSION || right->type == DOUBLE_EXPRESSION)) {

        ORG_Value *value;
        /* Overwriting left hand expression. */
        value = org_eval_binary_expression(org_get_current_interpreter(), NULL, op, left, right);
        *left = convert_value_to_expression(value);
        return left;
    } else {
        Expression *exp;
        exp = org_alloc_expression(op);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
    }
}

//创建 单元取反 表达式
Expression *org_create_minus_expression(Expression *operand) {
    if (operand->type == INT_EXPRESSION || operand->type == DOUBLE_EXPRESSION) {
        ORG_Value *v;
        v = org_eval_minus_expression(org_get_current_interpreter(), NULL, operand);
        /* Notice! Overwriting operand expression. */
        *operand = convert_value_to_expression(v);
        return operand;
    } else {
        Expression      *exp;
        exp = org_alloc_expression(MINUS_EXPRESSION);
        exp->u.minus_expression = operand;
        return exp;
    }
}

//变量表达式
Expression *org_create_identifier_expression(char *identifier) {
    Expression *exp;
    exp = org_alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier = identifier;
    return exp;
}

//函数表达式
Expression *org_create_function_call_expression(char *fun_name, ArgumentList *argument) {
    Expression *exp;
    exp = org_alloc_expression(FUNCTION_CALL_EXPRESSION);
    exp->u.function_call_expression.identifier = fun_name;
    exp->u.function_call_expression.argument = argument;

    return exp;
}

// 布尔类型表达式
Expression *org_create_boolean_expression(ORG_Boolean value) {
    Expression *exp;
    exp = org_alloc_expression(BOOLEAN_EXPRESSION);
    exp->u.boolean_value = value;
    return exp;
}

// null 表达式
Expression *org_create_null_expression(void) {
    Expression *exp;
    exp = org_alloc_expression(NULL_EXPRESSION);
    return exp;
}

/* statement */
//申请语句内存空间
static Statement *org_alloc_statement(StatementType type) {
    Statement *st;
    st = org_malloc(sizeof(Statement));
    st->type = type;
    st->line_number = org_get_current_interpreter()->current_line_number;

    return st;
}

//全局语句
Statement *org_create_global_statement(IdentifierList *identifier_list) {
    Statement *st;
    st = org_alloc_statement(GLOBAL_STATEMENT);
    st->u.global_s.identifier_list = identifier_list;

    return st;
}

//创建全局变量
IdentifierList *org_create_global_identifier(char *identifier) {
    IdentifierList *i_list;
    i_list = org_malloc(sizeof(IdentifierList));
    i_list->name = identifier;
    i_list->next = NULL;

    return i_list;
}

IdentifierList *org_chain_identifier(IdentifierList *list, char *identifier) {
    IdentifierList *pos;
    for (pos = list; pos->next != NULL; pos = pos->next) ;

    pos->next = org_create_global_identifier(identifier);
}

//if语句
Statement *org_create_if_statement(Expression *condition, Block *then_block,
                                   Elseif *elseif_list, Block *else_block) {

    Statement *st;
    st = org_alloc_statement(IF_STATEMENT);

    st->u.if_s.condition = condition;
    st->u.if_s.then_block = then_block;
    st->u.if_s.elseif_list = elseif_list;
    st->u.if_s.else_block = else_block;

    return st;
}

//elseif 语句
Elseif *org_create_elseif(Expression *condition, Block *block) {
    Elseif *ei;
    ei = org_malloc(sizeof(Elseif));
    ei->condition = condition;
    ei->block = block;
    ei->next = NULL;

    return ei;
}

Elseif *org_chain_elseif_list(Elseif *list, Elseif *add) {
    Elseif *pos;
    for (pos = list; pos->next != NULL; pos = pos->next) ;

    pos->next = add;
    return list;
}

//while 语句
Statement *org_create_while_statement(Expression *condition, Block *block) {
    Statement *st;
    st = org_alloc_statement(WHILE_STATEMENT);
    st->u.while_s.condition = condition;
    st->u.while_s.block = block;

    return st;
}

// for 语句
Statement *org_create_for_statement(Expression *init, Expression *condition, Expression *post, Block *block) {
    Statement *st;
    st = org_alloc_statement(FOR_STATEMENT);
    st->u.for_s.init = init;
    st->u.for_s.condition = condition;
    st->u.for_s.post = post;
    st->u.for_s.block = block;

    return st;
}

// 创建块段
Block *org_create_block(StatementList *statement_list) {
    Block *block;
    block = org_malloc(sizeof(Block));
    block->statement_list = statement_list;
    return block;
}

// 创建表达式语句
Statement *org_create_expression_statement(Expression *expression) {
    Statement *st;
    st = org_alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression_s = expression;
    return st;
}

// return 语句
Statement *org_create_return_statement(Expression *expression) {
    Statement *st;
    st = org_alloc_statement(RETURN_STATEMENT);
    st->u.return_s.return_value = expression;
    return st;
}

// break 语句
Statement *org_create_break_statement(void) {
    Statement *st;
    st = org_alloc_statement(BREAK_STATEMENT);
    return st;
}

// continue 语句
Statement *org_create_continue_statement(void) {
    Statement *st;
    st = org_alloc_statement(CONTINUE_STATEMENT);
    return st;
}
