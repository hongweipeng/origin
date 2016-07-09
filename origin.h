/*************************************************************************
	> File Name: origin.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月07日 星期四 10时53分18秒
 ************************************************************************/

#ifndef _ORIGIN_H
#define _ORIGIN_H

#include <stdio.h>
//#include "MEM.h"
#include "ORG.h"
#include "ORG_dev.h"

#define smaller(a, b)  ((a) < (b) ? (a) : (b))
#define larger(a, b)   ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX (256)
#define LINE_BUF_SIZE   (1024)

//编译时报错的类型
typedef enum {
    PAESE_ERR = 1,
    CHARACTER_INVALID_ERR,
}CompileError;


//运行时错误
typedef enum {
    VARIABLE_NOT_FOUND_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    ARGUMENT_TOO_MANT_ERR,
    NOT_BOOLEAN_TYPE_ERR,
} RuntimeError;

//定义错误信息结构
typedef struct {
    char *format;
} MessageFormat;

typedef struct Expression_tag Expression;

//表达式类型
typedef enum {
    BOOLEAN_EXPRESSION = 1, //布尔类型常量
    INT_EXPRESSION,         //整数型常量
    DOUBLE_EXPRESSION,      //实数型常量
    STRING_EXPRESSION,      //字符串型常量
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
    LOGIAL_AND_EXPRESSION,  // &&
    LOGIAL_OR_EXPRESSION,   // ||
    MINUS_EXPRESSION,       // 单目取反
    FUNCTION_CALL_EXPRESSION, //函数调用表达式
    NULL_EXPRESSION,        //null表达式
    EXPRESSION_TYPE_COUNT_PLUS_1,

} ExpressionType;

/* 一些宏定义 */

//判断是否是运算符 采用宏可减少函数调用,节约时间
#define def_is_match_operator(operator) \
        ((operator) == ADD_EXPRESSION || (operator) == SUB_EXPRESSION \
        || (operator) == MUL_EXPRESSION || (operator) == DIV_EXPRESSION \
        || (operator) == MOD_EXPRESSION)

//判断是否是判断符
#define def_is_compare_operator(operator) \
        ((operator) == EQ_EXPRESSION || (operator) == NE_EXPRESSION \
        || (operator) == GT_EXPRESSION || (operator) == GE_EXPRESSION \
        || (operator) == LT_EXPRESSION || (operator) == LE_EXPRESSION)

//参数列表
typedef struct ArgumentList_tag {
    Expression *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

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

//函数调用表达式
typedef struct {
    char *identifier;//函数名
    ArgumentList *argument;//函数列表
} FunctionCallExpression;

//表达式结构
struct Expression_tag {
    ExpressionType type;
    int line_number;
    union {
        ORG_Boolean     boolean_value;
        int             int_value;
        double          double_value;
        char            *string_value;
        char            *identifier;
        AssignExpression assign_expression;
        BinaryExpression binary_expression;
        Expression      *minus_expression;
        FunctionCallExpression function_call_expression;
    } u;
};

typedef struct Statement_tag Statement;

//代码段列
typedef struct StatementList_tag {
    Statement *statement;
    struct Statement_tag *next;
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

//elseif标记
typedef struct Elseif_tag {
    Expression *condition;
    Block *block;
    struct Elseif_tag *next;
} Elseif;

//if块,每个if块都配一个else
typedef struct {
    Expression *condition;
    Block *then_block;
    Elseif *elseif_list;
    Block *else_block;
} IfStatement;

//while
typedef struct {
    Expression *condition;
    Block       *block;
} WhileStatement;

//for
typedef struct {
    Expression *init;
    Expression *condition;
    Expression *post;
    Block      *block;
} ForStatement;

//return
typedef  struct {
    Expression *return_value;
} ReturnStatement;

//statement type
typedef enum {
    EXPRESSION_STATEMENT = 1,
    GLOBAL_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    STATEMENT_TYPE_CONUT_PLUS_1,
} StatementType;

//statement
struct Statement_tag {
    StatementType   type;
    int             line_number;
    union {
        Expression *expression_s;
        GlobalStatement global_s;
        IfStatement     if_s;
        WhileStatement  while_s;
        ForStatement    for_s;
        ReturnStatement return_s;
    } u;
};

//param list
typedef struct ParameterList_tag {
    char        *name;
    struct ParameterList_tag *next;
} ParameterList;

// function type
typedef enum {
    ORIGIN_FUNCTION_DEF = 1,
    NATIVE_FUNCTION_DEF,
} FunctionDefineType;

typedef struct FunctionDefine_tag {
    char        *name;
    FunctionDefineType type;
    union {
        struct {
            ParameterList       *parameter;
            Block               *block;
        } crowbar_f;
        struct {
            ORG_NativeFunctionPro     *pro;
        } native_f;
    } u;
    struct FunctionDefine_tag *next;
} FunctionDefine;

//var tag
typedef struct Variable_tag {
    char        *name;
    ORG_Value   value;
    struct Variable_tag *next;
} Variable;

typedef enum {
    NORMAL_STATEMENT_RESULT = 1,
    RETURN_STATEMENT_RESULT,
    BREAK_STATEMENT_RESULT,
    CONTINUE_STATEMENT_RESULT,
    STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct {
    StatementResultType type;
    union {
        ORG_Value       return_value;
    } u;
} StatementResult;

typedef struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {
    Variable    *variable;
    GlobalVariableRef   *global_variable;
} LocalEnvironment;

struct ORG_String_tag {
    int     ref_count;
    char    *string;
    ORG_Boolean is_literal;
};

typedef struct {
    ORG_String *string;
} StringPool;

struct ORG_Interpreter_tag {
    MEM_Storage         interpreter_storage;
    MEM_Storage         execute_storage;
    Variable            *variable;
    FunctionDefinition  *function_list;
    StatementList       *statement_list;
    int                 current_line_number;
};

// create.c
void org_function_define(char *identifier, ParameterList *parameter_list,
                        Block *block);
ParameterList *org_create_parameter(char *identifier);
ParameterList *org_chain_parameter(ParameterList *list, char *identifier);

ArgumentList *org_create_argument_list(Expression *expression);
ArgumentList *org_chain_argument_list()







#endif
