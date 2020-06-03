#include "intobject.h"
#include "boolobject.h"

static ORG_Int * _int_new_imp() {
    ORG_Int *return_value = (ORG_Int *) org_malloc(sizeof(ORG_Int));
    ((ORG_Value *)return_value)->type = ORG_INT_VALUE;
    return return_value;
}

/**
 * 从 ORG_Value 中获得int值
 * @param value
 * @return
 */
static int int_from_org_value(ORG_Value *value) {
    return ((ORG_Int *)value)->digit_value;
}

/**
 * 比较
 * 当 a == b: 返回 0
 * 当 a > b: 返回 正数
 * 当 a < b: 返回 负数
 * todo: 处理溢出
 * @param a
 * @param b
 * @return
 */
static int int_compare(ORG_Int *a, ORG_Int *b) {
    int sign = a->digit_value - b->digit_value;
    return sign;
}

/**
 * 提供给外部调用的API
 * @return
 */
ORG_Value * org_int_new() {
    ORG_Int *v = _int_new_imp();
    /*设置默认值*/
    v->digit_value = 0;
    return (ORG_Value *) v;
}

ORG_Value * org_int_from_int(int num) {
    ORG_Int *v = _int_new_imp();
    v->digit_value = num;
    ((ORG_Value *)v)->u.int_value = num;   /* todo: 后续移除*/
    return (ORG_Value *) v;
}

/**
 * 整型的二元操作, 加减乘除 移位 与 或 异或
 * @param left
 * @param right
 * @param op
 * @return
 */
ORG_Value * binary_int(ORG_Value *left, ORG_Value *right, ExpressionType op) {
    int left_int = int_from_org_value(left);
    int right_int = int_from_org_value(right);
    int result = 0;

    if (op == ADD_EXPRESSION) {
        result = left_int + right_int;
    } else if (op == SUB_EXPRESSION) {
        result = left_int - right_int;
    } else if (op == MUL_EXPRESSION) {
        result = left_int * right_int;
    } else if (op == DIV_EXPRESSION) {
        result = left_int / right_int;
    } else if (op == MOD_EXPRESSION) {
        result = left_int % right_int;
    } else if (op == BIT_LEFT_EXPRESSION) {
        result = left_int << right_int;
    } else if (op == BIT_RIGHT_EXPRESSION) {
        result = left_int >> right_int;
    } else if (op == BIT_OR_EXPRESSION) {
        result = left_int | right_int;
    } else if (op == BIT_AND_EXPRESSION) {
        result = left_int & right_int;
    } else if (op == BIT_XOR_EXPRESSION) {
        result = left_int ^ right_int;
    } else {
        /*不支持的操作*/
    }
    return org_int_from_int(result);
}

/**
 * 整型的比较
 * @param left
 * @param right
 * @param op
 * @return
 */
ORG_Value * binary_int_compare(ORG_Value *left, ORG_Value *right, ExpressionType op) {
    int sign = int_compare((ORG_Int *) left, (ORG_Int *) right);
    int tmp = 0;

    if (op == EQ_EXPRESSION) {
        tmp = sign == 0;
    } else if (op == NE_EXPRESSION) {
        tmp = sign != 0;
    } else if (op == GT_EXPRESSION) {
        tmp = sign > 0;
    } else if (op == GE_EXPRESSION) {
        tmp = sign >= 0;
    } else if (op == LT_EXPRESSION) {
        tmp = sign < 0;
    } else if (op == LE_EXPRESSION) {
        tmp = sign <= 0;
    } else {
        /*不支持的操作*/
    }

    return tmp ? ORG_True : ORG_False;;
}


