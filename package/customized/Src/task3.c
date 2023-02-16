/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task3.h"
#include <rtthread.h>
#include "sys_cfg.h"

void thread3_entry(void* parameter)
{
    while (1) {
        Tmr3Hanler();
        rt_thread_delay(10);
    }
}
