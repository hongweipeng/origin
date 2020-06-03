/**
 * Created by hongweipeng on 2020/6/3.
 */

#ifndef ORIGIN_INTOBJECT_H
#define ORIGIN_INTOBJECT_H

#include "ORG_dev.h"
#include "origin.h"
#include "MEM.h"

/* 整型类型*/
typedef struct {
    ORG_Value ob_base;
    int digit_value;
} ORG_Int;



/*外部可调用函数*/
ORG_Value * org_int_new(void);
ORG_Value * org_int_from_int(int num);
ORG_Value * binary_int(ORG_Value *left, ORG_Value *right, ExpressionType op);

#endif
