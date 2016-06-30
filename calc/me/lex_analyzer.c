/*************************************************************************
	> File Name: lex_analyzer.c
	> Author: weapon 
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年06月30日 星期四 22时56分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

static char *st_line;
static int st_line_pos;

//保存符合数值特征的值
typedef enum {
    INIT_STATUS, //初始状态
    IN_INT_PART_STATUS, // 0~9整数状态
    DOT_STATUS, // 小数点
    IN_FRAC_PART_STATUS, //小数状态 
} LexerStatus;


//负责将记号分割出来,是词法分析器的核心部分
void get_token(Token *token) {
    int out_pos = 0;
    LexerStatus status = INIT_STATUS;
    char current_char;

    token->kind = BAD_TOKEN;
    while (st_line[st_line_pos] != '\0') {
        current_char = st_line[st_line_pos];
        
        //处理数字
        if ((status == IN_INT_PART_STATUS || status == IN_FRAC_PART_STATUS)
            && !isdigit(current_char) && current_char != '.') {
                token->kind = NUMBER_TOKEN;
                sscanf(token->str, "%lf", &token->value);
                return;
        }
        
        //处理空白符
        if (isspace(current_char)) {
            if (current_char == '\n') {
                token->kind = END_OF_LINE_TOKEN;
                return;
            }
            st_line_pos++;
            continue;
        }

        //处理长度过长
        if (out_pos >= MAX_TOKEN_SIZE - 1) {
            fprintf(stderr, "token too long\n");
            exit(1);
        }

        token->str[out_pos] = st_line[st_line_pos];
        st_line_pos++;
        out_pos++;
        token->str[out_pos] = '\0';

        // + - * / 都只占一个字符长度,一旦扫描到则可以立即返回
        if (current_char == '+') {
            token->kind = ADD_OPERATOR_TOKEN;
            return;
        }else if (current_char == '-') {
            token->kind = SUB_OPERATOR_TOKEN;
            return;
        }else if (current_char == '*') {
            token->kind = MUL_OPERATOR_TOKEN;
            return;
        }else if (current_char == '/') {
            token->kind = DIV_OPERATOR_TOKEN;
            return;
        }else if (isdigit(current_char)) {
            if (status == INIT_STATUS) {
                status = IN_INT_PART_STATUS;
            }else if (status == DOT_STATUS) {
                status = IN_FRAC_PART_STATUS;
            }
        }else if (current_char == '.') {
            if (status == IN_INT_PART_STATUS) {
                status = DOT_STATUS;
            }
        }else {
            fprintf(stderr, "bad character(%c)\n", current_char);
            exit(1);
        }
    }
}

void set_line(char *line) {
    st_line = line;
    st_line_pos = 0;
}

//测试驱动代码
/*
void parse_line(char *buf) {
    Token token;

    set_line(buf);

    for(;;) {
        get_token(&token);
        if(token.kind == END_OF_LINE_TOKEN) {
            break;
        }else {
            printf("kind..%d, str..%s\n", token.kind, token.str);
        }
    }
}

int main(int argc, char **atgv) {
    char buf[1024];
    while (fgets(buf, 1024, stdin) != NULL) {
        parse_line(buf);
    }
    return 0;
}*/
