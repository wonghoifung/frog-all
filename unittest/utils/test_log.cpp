//
//  test_log.c
//  console
//
//  Created by huanghaifeng on 14-10-31.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//
#if 0
#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

int main(int, char*[])
{
    const char* s = "abcdefghijklmnopqrstuvwxyz1234567890";

    frog::utils::logger::ref().init("frog", "/Users/huanghaifeng/Desktop/frog-utils/");
    logstr("hello %s\n", "world");
    logstr("%s", s);
    loghex(s, strlen(s));
    
    int size(0);
    int cnt(0);
    while (true)
    {
        ++cnt;
        logstr("%s%d", s, cnt);
        loghex(s, strlen(s));
        size += strlen(s);
        if (size > (50 * 1024 * 1024))
        { break; }
    }
    
    return 0;
}
#endif
