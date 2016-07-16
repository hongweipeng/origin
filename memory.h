/*************************************************************************
	> File Name: memory.h
	> Author: weapon
	> Mail: hongweichen8888@sina.com
	> Created Time: 2016年07月16日 星期六 11时52分43秒
 ************************************************************************/

#ifndef _MEMORY_H
#define _MEMORY_H

#include "MEM.h"

typedef union Header_tag Header;

struct MEM_Controller_tag {
    FILE                *error_fp;
    MEM_ErrorHandler    error_headler;
    MEM_FailMode        fail_mode;
    Header              *block_header;
};


#endif
