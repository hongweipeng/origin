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
//#include "CRB.h"
//#include "CRB_dev.h"

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
} RuntimeError;











#endif
