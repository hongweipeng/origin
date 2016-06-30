//toekn 结构体:
#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED


typedef enum {
	BAD_TOKEN, //错误标记
	NUMBER_TOKEN, //数字标识
	ADD_OPERATOR_TOKEN, // + 号标识
	SUB_OPERATOR_TOKEN, // - 号标识
	MUL_OPERATOR_TOKEN, // * 号标识
	DIV_OPERATOR_TOKEN, // / 号标识
	END_OF_LINE_TOKEN,
}TokenKind;

#define MAX_TOKEN_SIZE (100)

typedef struct {
    TokenKind kind;
    double value;
    char str[MAX_TOKEN_SIZE];
}Token;

void set_line(char *line);
void get_goken(char *token);

#endif

