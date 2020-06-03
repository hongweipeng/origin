/**
 * Created by admin on 2020/6/3.
 */

#ifndef ORIGIN_BOOLOBJECT_H
#define ORIGIN_BOOLOBJECT_H

#include "ORG_dev.h"
#include "origin.h"
#include "MEM.h"

/* 整型类型*/
typedef struct {
    ORG_Value ob_base;
    ORG_Boolean bool_value;
} ORG_Bool;


extern ORG_Bool _ORG_TrueStruct, _ORG_FalseStruct;

#define ORG_True ((ORG_Value*) &_ORG_TrueStruct)
#define ORG_False ((ORG_Value*) &_ORG_FalseStruct)

#endif
