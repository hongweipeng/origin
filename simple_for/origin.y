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
    Expression          *expression;
    Statement           *statement;
    StatementList       *statement_list;
    Block               *block;
    IdentifierList      *identifier_list;
}
%token <expression>     INT_LITERAL
%token <identifier>     IDENTIFIER
%token  IF BREAK FOR
        LP RP LC RC SEMICOLON ASSIGN
        EQ NE GT GE LT LE ADD SUB MUL DIV MOD
        PRINT ELSE
%type   <expression> expression
        additive_expression multiplicative_expression
        logical_expression primary_expression expression_opt

%type   <statement> statement global_statement for_statement if_statement
        break_statement
%type   <statement_list> statement_list

%type   <block> block
%type   <identifier_list> identifier_list
%%

translation_unit
        : definition_or_statement
        | translation_unit definition_or_statement
        ;

definition_or_statement
        : statement
        {
            ORG_Interpreter *inter = org_get_current_interpreter();

            inter->statement_list = org_chain_statement_list(inter->statement_list, $1);
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
        : logical_expression
        | IDENTIFIER ASSIGN expression
        {
            $$ = org_create_assign_expression($1, $3);
        }
        ;
logical_expression
        : additive_expression
        | logical_expression GT additive_expression
        {
            $$ = org_create_binary_expression(GT_EXPRESSION, $1, $3);
        }
        | logical_expression GE additive_expression
        {
            $$ = org_create_binary_expression(GE_EXPRESSION, $1, $3);
        }
        | logical_expression LT additive_expression
        {
            $$ = org_create_binary_expression(LT_EXPRESSION, $1, $3);
        }
        | logical_expression LE additive_expression
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
        : primary_expression
        | multiplicative_expression MUL primary_expression
        {
            $$ = org_create_binary_expression(MUL_EXPRESSION, $1, $3);
        }
        | multiplicative_expression DIV primary_expression
        {
            $$ = org_create_binary_expression(DIV_EXPRESSION, $1, $3);
        }
        ;
primary_expression
        : LP expression RP
        {
          $$ = $2;
        }
        | IDENTIFIER
        {
          $$ = org_create_identifier_expression($1);
        }
        | INT_LITERAL
        ;

statement
        : expression SEMICOLON
        {
          $$ = org_create_expression_statement($1);
        }
        | PRINT LP expression RP SEMICOLON
        {
            $$ = org_create_print_statement($3);
        }
        | global_statement
        | if_statement
        | for_statement
        | break_statement
        ;
global_statement
        : identifier_list SEMICOLON
        {
            $$ = org_create_global_statement($1);
        }
        ;
identifier_list
        : IDENTIFIER
        {
            $$ = org_create_global_identifier($1);
        }
        ;
if_statement
        : IF LP expression RP block
        {
            $$ = org_create_if_statement($3, $5, NULL);
        }
        | IF LP expression RP block ELSE block
        {
            $$ = org_create_if_statement($3, $5, $7);
        }
        ;
for_statement
        : FOR LP expression_opt SEMICOLON expression_opt SEMICOLON
          expression_opt RP block
        {
            $$ = org_create_for_statement($3, $5, $7, $9);
        }
        ;
break_statement
        : BREAK SEMICOLON
        {
            $$ = org_create_break_statement();
        }
        ;
expression_opt
        : /* empty */
        {
            $$ = NULL;
        }
        | expression
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



