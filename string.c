/*************************************************************************
	> File Name: string.c
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 10时29分03秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "origin.h"
#include "ORG_dev.h"

#define STRING_ALLOC_SIZE  (256)

static char *st_string_literal_buffer = NULL;
static int st_string_literal_buff_size = 0;
static int st_string_buffer_alloc_size = 0;

void org_open_string_literal(void) {
    st_string_literal_buff_size = 0;
}

void org_add_string_literal(int letter) {
    if (st_string_literal_buff_size == st_string_buffer_alloc_size) {
        st_string_buffer_alloc_size += STRING_ALLOC_SIZE;

        st_string_literal_buffer = MEM_realloc(st_string_literal_buffer, st_string_buffer_alloc_size);
    }
    st_string_literal_buffer[st_string_literal_buff_size] = letter;
    st_string_literal_buff_size ++;
}

void org_reset_string_literal_buffer(void) {
    MEM_free(st_string_literal_buffer);
    st_string_literal_buffer = NULL;
    st_string_literal_buff_size = 0;
    st_string_buffer_alloc_size = 0;
}

char *org_close_string_literal(void) {
    char *new_str;
    new_str = org_malloc(st_string_literal_buff_size + 1);
    //memcpy strcpy 用memcpy避免字符串中\0不被复制
    memcpy(new_str, st_string_literal_buffer, st_string_literal_buff_size);
    new_str[st_string_literal_buff_size] = '\0';
    return new_str;

}

char *org_create_identifier(char *str) {
    char *new_str;
    new_str = org_malloc(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}

char *org_value_to_string(ORG_Value value){
    char *str;
    size_t size;
    char buf[LINE_BUF_SIZE];
    if (value.type == ORG_INT_VALUE) {
        sprintf(buf, "%d", value.u.int_value);
    }else if(value.type == ORG_BIGNUM_VALUE) {
        size = mpi_msb(&value.u.big_num);
        str = malloc(size);
        mpi_write_string( &value.u.big_num, 10, str, &size );
        return str;
    }

    return buf;
}



