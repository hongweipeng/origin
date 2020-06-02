/*************************************************************************
	> File Name: origin.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月07日 星期四 10时53分18秒
 ************************************************************************/

#ifndef _ORIGIN_H
#define _ORIGIN_H

#include <stdio.h>
#include "MEM.h"
#include "ORG.h"
#include "ORG_dev.h"

#ifndef DEBUG
#define DEBUG
#endif

#define smaller(a, b)  ((a) < (b) ? (a) : (b))
#define larger(a, b)   ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX (256)
#define LINE_BUF_SIZE   (1024)

//编译时报错的类型
typedef enum {
    PARSE_ERR = 1,
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
    LOGICAL_AND_EXPRESSION,  // &&
    LOGICAL_OR_EXPRESSION,   // ||
    MINUS_EXPRESSION,       // 单目取反
    FUNCTION_CALL_EXPRESSION, //函数调用表达式
    NULL_EXPRESSION,        //null表达式
    BIG_DATA_EXPRESSION,    //大数

    BIT_LEFT_EXPRESSION,    //位运算 左移
    BIT_RIGHT_EXPRESSION,
    BIT_OR_EXPRESSION,
    BIT_AND_EXPRESSION,
    BIT_XOR_EXPRESSION,

    EXPRESSION_TYPE_COUNT_PLUS_1,

} ExpressionType;

/* 一些宏定义 */

//判断是否是运算符 采用宏可减少函数调用,节约时间
#define dkc_is_math_operator(op) \
        ((op) == ADD_EXPRESSION || (op) == SUB_EXPRESSION \
        || (op) == MUL_EXPRESSION || (op) == DIV_EXPRESSION \
        || (op) == MOD_EXPRESSION) || (op) == BIT_LEFT_EXPRESSION\
        || (op) == BIT_RIGHT_EXPRESSION || (op) == BIT_OR_EXPRESSION\
        || (op) == BIT_AND_EXPRESSION || (op) == BIT_XOR_EXPRESSION

//判断是否是判断符
#define dkc_is_compare_operator(op) \
        ((op) == EQ_EXPRESSION || (op) == NE_EXPRESSION \
        || (op) == GT_EXPRESSION || (op) == GE_EXPRESSION \
        || (op) == LT_EXPRESSION || (op) == LE_EXPRESSION)

//判断是否是逻辑操作符
#define dkc_is_logical_operator(op) \
  ((op) == LOGICAL_AND_EXPRESSION || (op) == LOGICAL_OR_EXPRESSION)

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
    EXPRESSION_STATEMENT = 100,
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
    ORIGIN_FUNCTION_DEF = 1,//语言自带的函数
    NATIVE_FUNCTION_DEF,
} FunctionDefineType;

typedef struct FunctionDefine_tag {
    char        *name;
    FunctionDefineType type;
    union {
        struct {
            ParameterList       *parameter;
            Block               *block;
        } origin_f;
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
    int     ref_count;      //引用计数
    char    *string;        // 字符串本身
    ORG_Boolean is_literal; // 是否是字面常量
};

typedef struct {
    ORG_String *string;
} StringPool;

//解释器
struct ORG_Interpreter_tag {
    MEM_Storage         interpreter_storage;    //分析树
    MEM_Storage         execute_storage;        //运行时
    Variable            *variable;              //全局变量列表
    FunctionDefine      *function_list;         //函数定义列表
    StatementList       *statement_list;        //语句列表
    int                 current_line_number;    //编译时当前的行号
};

// create.c
void org_function_define(char *identifier, ParameterList *parameter_list,
                        Block *block);
ParameterList *org_create_parameter(char *identifier);
ParameterList *org_chain_parameter(ParameterList *list, char *identifier);

ArgumentList *org_create_argument_list(Expression *expression);
ArgumentList *org_chain_argument_list(ArgumentList *list, Expression *expr);
StatementList *org_create_statement_list(Statement *statement);
StatementList *org_chain_statement_list(StatementList *list, Statement *statement);

Expression *org_alloc_expression(ExpressionType type);
Expression *org_create_assign_expression(char *variable, Expression *operand);
Expression *org_create_binary_expression(ExpressionType op, Expression *left, Expression *right);
Expression *org_create_minus_expression(Expression *operand);
Expression *org_create_identifier_expression(char *identifier);
Expression *org_create_function_call_expression(char *func_name, ArgumentList *argument);

Expression *org_create_boolean_expression(ORG_Boolean value);
Expression *org_create_null_expression(void);


IdentifierList *org_create_global_identifier(char *identifier);
IdentifierList *org_chain_identifier(IdentifierList *list, char *identifier);

Statement *org_create_global_statement(IdentifierList *identifier_list);
IdentifierList *org_create_global_identifier(char *identifier);
IdentifierList *org_chain_identifier(IdentifierList *list, char *identifier);

//elseif statement
Statement *org_create_if_statement(Expression *condition, Block *then_block,
                                    Elseif *elseif_list, Block *else_block);
Elseif *org_chain_elseif_statement(Expression *expr, Elseif *add);
Elseif *org_create_elsif(Expression *expr, Block *block);
Statement *org_create_while_statement(Expression *condition, Block *block);
Statement *org_create_for_statement(Expression *init, Expression *cond,
                                    Expression *post, Block *block);
Block *org_create_block(StatementList *statement_list);
Statement *org_create_expression_statement(Expression *expression);
Statement *org_create_return_statement(Expression *expression);
Statement *org_create_break_statement(void);
Statement *org_create_continue_statement(void);


/* string.c */
char *org_create_identifier(char *str);
void org_open_string_literal(void);
void org_add_string_literal(int letter);
void org_reset_string_literal_buffer(void);
char *org_close_string_literal(void);
char *org_value_to_string(ORG_Value value);

/* execute.c */
StatementResult org_execute_statement_list(ORG_Interpreter *inter, LocalEnvironment *env, StatementList *list);

/* execute.c */
StatementResult org_execute_statement_list(ORG_Interpreter *inter, LocalEnvironment *env, StatementList *list);

/* eval.c */
ORG_Value org_eval_binary_expression(ORG_Interpreter *inter, LocalEnvironment *env, ExpressionType op,
                                     Expression *left, Expression *right);
ORG_Value org_eval_minus_expression(ORG_Interpreter *inter,
                                    LocalEnvironment *env, Expression *operand);
ORG_Value org_eval_expression(ORG_Interpreter *inter,
                              LocalEnvironment *env, Expression *expr);

/* string_pool.c */
ORG_String *org_literal_to_org_string(ORG_Interpreter *inter, char *str);
void org_refer_string(ORG_String *str);
void org_release_string(ORG_String *str);
ORG_String *org_search_org_string(ORG_Interpreter *inter, char *str);
ORG_String *org_create_origin_string(ORG_Interpreter *inter, char *str);

/* util.c */
ORG_Interpreter *org_get_current_interpreter(void);
void org_set_current_interpreter(ORG_Interpreter *inter);
void *org_malloc(size_t size);
void *org_execute_malloc(ORG_Interpreter *inter, size_t size);
Variable *org_search_local_variable(LocalEnvironment *env, char *identifier);
Variable *org_search_global_variable(ORG_Interpreter *inter, char *identifier);
void org_add_local_variable(LocalEnvironment *env, char *identifier, ORG_Value *value);
ORG_NativeFunctionPro *org_search_native_function(ORG_Interpreter *inter, char *name);
FunctionDefine *org_search_function(char *name);
char *org_get_operator_string(ExpressionType type);

/* error.c */
void org_compile_error(CompileError id, ...);
void org_runtime_error(int line_number, RuntimeError id, ...);

/* native.c */
ORG_Value org_nv_print_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args);
ORG_Value org_nv_fopen_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args);
ORG_Value org_nv_fclose_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args);
ORG_Value org_nv_fgets_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args);
ORG_Value org_nv_fputs_proc(ORG_Interpreter *interpreter, int arg_count, ORG_Value *args);
void org_add_std_fp(ORG_Interpreter *inter);




#endif
