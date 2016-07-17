%{
#include <stdio.h>
#include "origin.h"
#define YYDEBUG 1
int yyerror(char const *str) {
    extern char *yytext;
    char *near_token;

    if (yytext[0] == '\0') {
        near_token = "EOF";
    } else {
        near_token = yytext;
    }
    //crb_compile_error(PARSE_ERR,STRING_MESSAGE_ARGUMENT, "token", near_token,MESSAGE_ARGUMENT_END);

    return 0;
}
%}
%union {
    char                *identifier;
    ParameterList       *parameter_list;
    ArgumentList        *argument_list;
    Expression          *expression;
    Statement           *statement;
    StatementList       *statement_list;
    Block               *block;
    Elseif              *elseif;
    IdentifierList      *identifier_list;
}
%token <expression>     INT_LITERAL
%token <expression>     DOUBLE_LITERAL
%token <expression>     STRING_LITERAL
%token <identifier>     IDENTIFIER
%token FUNCTION IF ELSE ELSEIF WHILE FOR RETURN_O BREAK CONTINUE NULL_O
        LP RP LC RC SEMICOLON COMMA ASSIGN LOGICAL_AND LOGICAL_OR
        EQ NE GT GE LT LE ADD SUB MUL DIV MOD TRUE_O FALSE_O GLOBAL_O
%type   <parameter_list> parameter_list
%type   <argument_list> argument_list
%type   <expression> expression expression_opt
        logical_and_expression logical_or_expression
        equality_expression relational_expression
        additive_expression multiplicative_expression
        unary_expression primary_expression
%type   <statement> statement global_statement
        if_statement while_statement for_statement
        return_statement break_statement continue_statement
%type   <statement_list> statement_list
%type   <block> block
%type   <elseif> elseif elseif_list
%type   <identifier_list> identifier_list
%%
translation_unit
        : definition_or_statement
        | translation_unit definition_or_statement
        ;
definition_or_statement
        : function_definition
        | statement
        {
            ORG_Interpreter *inter = org_get_current_interpreter();

            inter->statement_list = org_chain_statement_list(inter->statement_list, $1);
        }
        ;
function_definition
        : FUNCTION IDENTIFIER LP parameter_list RP block
        {
            org_function_define($2, $4, $6);
        }
        | FUNCTION IDENTIFIER LP RP block
        {
            org_function_define($2, NULL, $5);
        }
        ;
parameter_list
        : IDENTIFIER
        {
            $$ = org_create_parameter($1);
        }
        | parameter_list COMMA IDENTIFIER
        {
            $$ = org_chain_parameter($1, $3);
        }
        ;
argument_list
        : expression
        {
            $$ = org_create_argument_list($1);
        }
        | argument_list COMMA expression
        {
            $$ = org_chain_argument_list($1, $3);
        }
        ;
statement_list
        : statement
        {
            $$ = org_create_statement_list($1);
        }
        | statement_list statement
        {
            $$ = org_chain_statement_list($1, $2);
        }
        ;
expression
        : logical_or_expression
        | IDENTIFIER ASSIGN expression
        {
            $$ = org_create_assign_expression($1, $3);
        }
        ;
logical_or_expression
        : logical_and_expression
        | logical_or_expression LOGICAL_OR logical_and_expression
        {
            $$ = org_create_binary_expression(LOGICAL_OR_EXPRESSION, $1, $3);
        }
        ;
logical_and_expression
        : equality_expression
        | logical_and_expression LOGICAL_AND equality_expression
        {
            $$ = org_create_binary_expression(LOGICAL_AND_EXPRESSION, $1, $3);
        }
        ;
equality_expression
        : relational_expression
        | equality_expression EQ relational_expression
        {
            $$ = org_create_binary_expression(EQ_EXPRESSION, $1, $3);
        }
        | equality_expression NE relational_expression
        {
            $$ = org_create_binary_expression(NE_EXPRESSION, $1, $3);
        }
        ;
relational_expression
        : additive_expression
        | relational_expression GT additive_expression
        {
            $$ = org_create_binary_expression(GT_EXPRESSION, $1, $3);
        }
        | relational_expression GE additive_expression
        {
            $$ = org_create_binary_expression(GE_EXPRESSION, $1, $3);
        }
        | relational_expression LT additive_expression
        {
            $$ = org_create_binary_expression(LT_EXPRESSION, $1, $3);
        }
        | relational_expression LE additive_expression
        {
            $$ = org_create_binary_expression(LE_EXPRESSION, $1, $3);
        }
        ;
additive_expression
        : multiplicative_expression
        | additive_expression ADD multiplicative_expression
        {
            $$ = org_create_binary_expression(ADD_EXPRESSION, $1, $3);
        }
        | additive_expression SUB multiplicative_expression
        {
            $$ = org_create_binary_expression(SUB_EXPRESSION, $1, $3);
        }
        ;
multiplicative_expression
        : unary_expression
        | multiplicative_expression MUL unary_expression
        {
            $$ = org_create_binary_expression(MUL_EXPRESSION, $1, $3);
        }
        | multiplicative_expression DIV unary_expression
        {
            $$ = org_create_binary_expression(DIV_EXPRESSION, $1, $3);
        }
        | multiplicative_expression MOD unary_expression
        {
            $$ = org_create_binary_expression(MOD_EXPRESSION, $1, $3);
        }
        ;
unary_expression
        : primary_expression
        | SUB unary_expression
        {
            $$ = org_create_minus_expression($2);
        }
        ;
primary_expression
        : IDENTIFIER LP argument_list RP
        {
            $$ = org_create_function_call_expression($1, $3);
        }
        | IDENTIFIER LP RP
        {
            $$ = org_create_function_call_expression($1, NULL);
        }
        | LP expression RP
        {
            $$ = $2;
        }
        | IDENTIFIER
        {
            $$ = org_create_identifier_expression($1);
        }
        | INT_LITERAL
        | DOUBLE_LITERAL
        | STRING_LITERAL
        | TRUE_O
        {
            $$ = org_create_boolean_expression(ORG_TRUE);
        }
        | FALSE_O
        {
            $$ = org_create_boolean_expression(ORG_FALSE);
        }
        | NULL_O
        {
            $$ = org_create_null_expression();
        }
        ;
statement
        : expression SEMICOLON
        {
          $$ = org_create_expression_statement($1);
        }
        | global_statement
        | if_statement
        | while_statement
        | for_statement
        | return_statement
        | break_statement
        | continue_statement
        ;
global_statement
        : GLOBAL_O identifier_list SEMICOLON
        {
            $$ = org_create_global_statement($2);
        }
        ;
identifier_list
        : IDENTIFIER
        {
            $$ = org_create_global_identifier($1);
        }
        | identifier_list COMMA IDENTIFIER
        {
            $$ = org_chain_identifier($1, $3);
        }
        ;
if_statement
        : IF LP expression RP block
        {
            $$ = org_create_if_statement($3, $5, NULL, NULL);
        }
        | IF LP expression RP block ELSE block
        {
            $$ = org_create_if_statement($3, $5, NULL, $7);
        }
        | IF LP expression RP block elseif_list
        {
            $$ = org_create_if_statement($3, $5, $6, NULL);
        }
        | IF LP expression RP block elseif_list ELSE block
        {
            $$ = org_create_if_statement($3, $5, $6, $8);
        }
        ;
elseif_list
        : elseif
        | elseif_list elseif
        {
            $$ = org_chain_elseif_list($1, $2);
        }
        ;
elseif
        : ELSEIF LP expression RP block
        {
            $$ = org_create_elseif($3, $5);
        }
        ;
while_statement
        : WHILE LP expression RP block
        {
            $$ = org_create_while_statement($3, $5);
        }
        ;
for_statement
        : FOR LP expression_opt SEMICOLON expression_opt SEMICOLON
          expression_opt RP block
        {
            $$ = org_create_for_statement($3, $5, $7, $9);
        }
        ;
expression_opt
        : /* empty */
        {
            $$ = NULL;
        }
        | expression
        ;
return_statement
        : RETURN_O expression_opt SEMICOLON
        {
            $$ = org_create_return_statement($2);
        }
        ;
break_statement
        : BREAK SEMICOLON
        {
            $$ = org_create_break_statement();
        }
        ;
continue_statement
        : CONTINUE SEMICOLON
        {
            $$ = org_create_continue_statement();
        }
        ;
block
        : LC statement_list RC
        {
            $$ = org_create_block($2);
        }
        | LC RC
        {
            $$ = org_create_block(NULL);
        }
        ;
%%