//
// Created by Administrator on 2017/9/4.
//

#ifndef ORIGIN_ORIGIN_H
#define ORIGIN_ORIGIN_H

#include <string.h>
// ================ 基础类型定义 ===============

//定义布尔类型 c语言没有布尔类型，需要自己定义
typedef enum {
    ORG_FALSE = 0,
    ORG_TRUE  = 1
} ORG_Boolean;

//变量类型
typedef enum {
    ORG_BOOLEAN_VALUE = 1, //布尔类型
    ORG_INT_VALUE,          //整型
} ORG_ValueType;

//通过变量类型
typedef struct {
    ORG_ValueType type;
    union {
        ORG_Boolean boolean_value;
        int     int_value;
    } u;
} ORG_Value;

// ================ 表达式定义 ===============

//表达式类型
typedef enum {
    BOOLEAN_EXPRESSION = 1, //布尔类型常量
    INT_EXPRESSION,         //整数型常量

    IDENTIFIER_EXPRESSION,  //变量
    ASSIGN_EXPRESSION,      //赋值表达式
    ADD_EXPRESSION,         //加法表达式
    SUB_EXPRESSION,         //减法表达式
    MUL_EXPRESSION,         //乘法表达式
    DIV_EXPRESSION,         //除法表达式
    MOD_EXPRESSION,         //求余表达式
    EQ_EXPRESSION,          // ==
    NE_EXPRESSION,          // !=
    GT_EXPRESSION,          // >
    GE_EXPRESSION,          // >=
    LT_EXPRESSION,          // <
    LE_EXPRESSION,          // <=
    EXPRESSION_TYPE_COUNT_PLUS_1,
} ExpressionType;
typedef struct Expression_tag Expression;
typedef struct Statement_tag Statement;
//赋值表达式
typedef struct {
    char *variable; //变量名
    Expression *operand;
} AssignExpression;

//二元表达式
typedef struct {
    Expression *left;
    Expression *right;
} BinaryExpression;

//表达式结构
struct Expression_tag {
    ExpressionType type;
    int line_number;
    union {
        ORG_Boolean     boolean_value;
        int             int_value;
        char            *identifier;
        AssignExpression assign_expression;
        BinaryExpression binary_expression;
    } u;
};


// ================ 程序段 ===============


// 语句列表
typedef struct StatementList_tag{
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

//块
typedef struct {
    StatementList *statement_list;
} Block;

//变量名列表
typedef struct IdentifierList_tag {
    char *name;
    struct IdentifierList_tag *next;
} IdentifierList;

//全局段
typedef struct {
    IdentifierList *identifier_list;
} GlobalStatement;

//for
typedef struct {
    Expression *init;
    Expression *condition;
    Expression *post;
    Block      *block;
} ForStatement;

//if块,每个if块都配一个else
typedef struct {
    Expression *condition;
    Block *then_block;
    Block *else_block;
} IfStatement;

typedef struct {
    Expression *exp;
} PrintStatement;

//var tag
typedef struct Variable_tag {
    char        *name;
    ORG_Value   value;
    struct Variable_tag *next;
} Variable;


// 代码段的返回结果
typedef enum {
    NORMAL_STATEMENT_RESULT = 1,
    RETURN_STATEMENT_RESULT,
    BREAK_STATEMENT_RESULT,
} StatementResultType;

//statement type
typedef enum {
    EXPRESSION_STATEMENT = 100,
    GLOBAL_STATEMENT,
    IF_STATEMENT,
    FOR_STATEMENT,
    BREAK_STATEMENT,
    PRINT_STATEMENT,
    STATEMENT_TYPE_CONUT_PLUS_1,
} StatementType;

//statement
struct Statement_tag{
    StatementType   type;
    int             line_number;
    union {
        Expression *expression_s;
        GlobalStatement global_s;
        ForStatement    for_s;
        IfStatement     if_s;
        PrintStatement  p_s;
    } u;
};

typedef struct {
    StatementResultType type;
    union {
        ORG_Value       return_value;
    } u;
} StatementResult;

//解释器
typedef struct {
    Variable            *variable;              //全局变量列表
    StatementList       *statement_list;        //语句列表
    //int                 current_line_number;    //编译时当前的行号
} ORG_Interpreter;

typedef struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {
    Variable    *variable;
    GlobalVariableRef   *global_variable;
} LocalEnvironment;

/* 一些宏定义 */

//判断是否是运算符 采用宏可减少函数调用,节约时间
#define dkc_is_math_operator(op) \
        ((op) == ADD_EXPRESSION || (op) == SUB_EXPRESSION \
        || (op) == MUL_EXPRESSION || (op) == DIV_EXPRESSION \
        || (op) == MOD_EXPRESSION)

//判断是否是判断符
#define dkc_is_compare_operator(op) \
        ((op) == EQ_EXPRESSION || (op) == NE_EXPRESSION \
        || (op) == GT_EXPRESSION || (op) == GE_EXPRESSION \
        || (op) == LT_EXPRESSION || (op) == LE_EXPRESSION)


StatementResult org_execute_statement_list(ORG_Interpreter *inter, LocalEnvironment *env, StatementList *list);
Expression *org_alloc_expression(ExpressionType type);
void ORG_add_global_variable(ORG_Interpreter *inter, char *identifier, ORG_Value *value);




#endif //ORIGIN_ORIGIN_H
