#include <malloc.h>
#include "origin.h"


static ORG_Interpreter *st_current_interperter; // 全局解释器

ORG_Interpreter *org_get_current_interpreter(void) {
    return st_current_interperter;
}

void org_set_current_interpreter(ORG_Interpreter *inter) {
    st_current_interperter = inter;
}

void *org_execute_malloc(ORG_Interpreter *inter, size_t size) {
    void *p;
    p = malloc(size);
    return p;
}

//申请存放表达式的地址空间
Expression *org_alloc_expression(ExpressionType type) {
    Expression *exp;
    exp = (Expression *)malloc(sizeof(Expression));
    exp->type = type;
    //exp->line_number = org_get_current_interpreter()->current_line_number;

    return exp;
}

//申请语句内存空间
static Statement *org_alloc_statement(StatementType type) {
    Statement *st;
    st = (Statement *) malloc(sizeof(Statement));
    st->type = type;
    //st->line_number = org_get_current_interpreter()->current_line_number;

    return st;
}

//创建赋值表达式
Expression *org_create_assign_expression(char *variable, Expression *operand) {
    Expression *exp;
    exp = org_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.variable = variable;
    exp->u.assign_expression.operand = operand;
    return exp;
}


//创建二元表达式
Expression *org_create_binary_expression(ExpressionType op, Expression *left, Expression *right) {
//    if (left->type == INT_EXPRESSION && right->type == INT_EXPRESSION ) {
//        ORG_Value value;
//        /* Overwriting left hand expression. */
//        value = org_eval_binary_expression(org_get_current_interpreter(), NULL, op, left, right);
//        *left = convert_value_to_expression(&value);
//        return left;
//    } else {
        Expression *exp;
        exp = org_alloc_expression(op);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
    //}
}

//变量表达式
Expression *org_create_identifier_expression(char *identifier) {
    Expression *exp;
    exp = org_alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier = identifier;
    return exp;
}

// 创建表达式语句
Statement *org_create_expression_statement(Expression *expression) {
    Statement *st;
    st = org_alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression_s = expression;
    return st;
}

//全局语句
Statement *org_create_global_statement(IdentifierList *identifier_list) {
    Statement *st;
    st = org_alloc_statement(GLOBAL_STATEMENT);
    st->u.global_s.identifier_list = identifier_list;

    return st;
}

Statement *org_create_print_statement(Expression *p) {
    Statement *st;
    st = org_alloc_statement(PRINT_STATEMENT);
    st->u.p_s.exp = p;

    return st;
}

//if语句
Statement *org_create_if_statement(Expression *condition, Block *then_block,
                                   Block *else_block) {

    Statement *st;
    st = org_alloc_statement(IF_STATEMENT);

    st->u.if_s.condition = condition;
    st->u.if_s.then_block = then_block;
    st->u.if_s.else_block = else_block;

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

// break 语句
Statement *org_create_break_statement(void) {
    Statement *st;
    st = org_alloc_statement(BREAK_STATEMENT);
    return st;
}

// 创建块段
Block *org_create_block(StatementList *statement_list) {
    Block *block;
    block = (Block *)malloc(sizeof(Block));
    block->statement_list = statement_list;
    return block;
}

//创建全局变量
IdentifierList *org_create_global_identifier(char *identifier) {
    IdentifierList *i_list;
    i_list = (IdentifierList *)malloc(sizeof(IdentifierList));
    i_list->name = identifier;
    i_list->next = NULL;

    return i_list;
}

//添加全局变量
void ORG_add_global_variable(ORG_Interpreter *inter, char *identifier, ORG_Value *value) {
    Variable *new_variable;
    new_variable = org_execute_malloc(inter, sizeof(Variable));
    new_variable->name = org_execute_malloc(inter, strlen(identifier) + 1);
    strcpy(new_variable->name, identifier);
    new_variable->value = *value;
    new_variable->next = inter->variable;
    inter->variable = new_variable;
}

//创建语句 数组
StatementList *org_create_statement_list(Statement *statement) {
    StatementList *sl;
    sl = (StatementList *)malloc(sizeof(StatementList));
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


