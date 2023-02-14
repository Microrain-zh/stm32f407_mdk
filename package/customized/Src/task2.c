/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task2.h"
#include <rtthread.h>
#include "sys_cfg.h"

void thread2_entry(void* parameter)
{
    Tmr2Init();

    while (1) {
        Tmr2Hanler();
        rt_thread_delay(5);
    }
}
