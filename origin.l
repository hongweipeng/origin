%{

    //词法分析器
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
    
    /*
    解析一个文件完毕后会调用yywrap,返回表示1结束,返回0表示继续
    */
    int yywrap(void) {
        return 1;
    }
%}


%%



%%
