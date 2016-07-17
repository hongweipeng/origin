/*************************************************************************
	> File Name: main.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 16时01分01秒
 ************************************************************************/

#include <stdio.h>
#include "ORG.h"
#include "MEM.h"
#include "origin.h"

int main(int argc, char **argv) {
    ORG_Interpreter     *interpreter;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }
    interpreter = ORG_create_interpreter();
    ORG_compile(interpreter, fp);
    ORG_interpret(interpreter);
    ORG_dispose_interpreter(interpreter);

    MEM_dump_blocks(stdout);

    return 0;
}

