/*************************************************************************
	> File Name: execute.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月10日 星期日 15时02分15秒
 ************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "origin.h"

static StatementResult execute_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement);

static StatementResult execute_expression_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    ORG_Value value;
    result.type = NORMAL_STATEMENT_RESULT;

    value = org_eval_expression(inter, env, statement->u.expression_s);

    if (value.type == ORG_STRING_VALUE) {
        org_release_string(value.u.string_value);
    }

    return result;
}

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
            if (!strcmp(ref_pos->variable->name), pos->name) {
                goto NEXT_IDENTIFIER;
            }
        }

        variable = org_search_global_variable(inter, pos->name);
        if (variable == NULL) {
            //runtime error
            printf("runtime error");
            exit(1);
        }

        new_ref = MEM_malloc(sizeof(GlobalVariableRef));
        new_ref->variable = variable;
        new_ref->next = env->global_variable;
        env->global_variable = new_ref;


        NEXT_IDENTIFIER:
            ;
    }

    return result;

}

static StatementResult execute_elseif(ORG_Interpreter *inter, LocalEnvironment *env, Elseif *elseif_list, ORG_Boolean *executed) {
    StatementResult result;
    ORG_Value cond;
    Elseif *pos;
    *executed = ORG_FALSE;
    result.type = NORMAL_STATEMENT_RESULT;

    for (pos = elseif_list; pos; pos = pos->next) {
        cond = org_eval_expression(inter, env, pos->condition);
        if (cond.type != ORG_BOOLEAN_VALUE) {
            //runtime error
            printf("runtime error");
            exit(1);
        }

        if (cond.u.boolean_value) {
            result = org_execute_statement_list(inter, env, pos->block->statement_list);
            *executed = ORG_TRUE;
            if (result.type != NORMAL_STATEMENT_RESULT)
                goto FUNC_END;
        }
    }
    FUNC_END:
        return result;
}

static StatementResult execute_if_statement(ORG_Interpreter *inter, LocalEnvironment *env, Statement *statement) {
    StatementResult result;
    ORG_Value cond;
    result.type = NORMAL_STATEMENT_RESULT;
    cond = org_eval_expression(inter, env, statement->u.if_s.condition);
    if (cond != ORG_BOOLEAN_VALUE) {
        //runtime error
        printf("runtime error");
        exit(1);
    }

    if (cond.u.boolean_value) {
        result = org_execute_statement_list(inter, env, statement->u.if_s.then_block->statement_list);
    } else {
        ORG_Boolean elseif_executed;
        result = execute_elseif(inter, env, statement->u.if_s.elseif_list, &elseif_executed);

        if (result.type != NORMAL_STATEMENT_RESULT) {
            return result;
        }
        if (!elseif_executed && statement->u.if_s.else_block) {
            result = org_execute_statement_list(inter, env, statement->u.if_s.else_block->statement_list);
        }
    }
    return result;
}

static StatementResult execute_while_statement(ORG_Interpreter *inter, LocalEnvironment *env,
                        Statement *statement)
{
    StatementResult result;
    ORG_Value   cond;

    result.type = NORMAL_STATEMENT_RESULT;
    for (;;) {
        cond = crb_eval_expression(inter, env, statement->u.while_s.condition);
        if (cond.type != ORG_BOOLEAN_VALUE) {
            crb_runtime_error(statement->u.while_s.condition->line_number,
                              NOT_BOOLEAN_TYPE_ERR, MESSAGE_ARGUMENT_END);
        }

        if (!cond.u.boolean_value)
            break;

        result = crb_execute_statement_list(inter, env, statement->u.while_s.block->statement_list);

        if (result.type == RETURN_STATEMENT_RESULT) {
            break;
        } else if (result.type == BREAK_STATEMENT_RESULT) {
            result.type = NORMAL_STATEMENT_RESULT;
            break;
        }
    }

    return result;
}
