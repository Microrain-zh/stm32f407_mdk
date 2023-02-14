/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task4.h"
#include <rtthread.h>
#include "sys_cfg.h"

void thread4_entry(void* parameter)
{
    Tmr4Init();

    while (1) {
        Tmr4Hanler();
        rt_thread_delay(5);
    }
}
