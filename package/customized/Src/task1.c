/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task1.h"
#include <rtthread.h>
#include "sys_cfg.h"

void thread1_entry(void* parameter)
{
    while (1) {
        Tmr1Hanler();
        rt_thread_delay(5);
    }
}
