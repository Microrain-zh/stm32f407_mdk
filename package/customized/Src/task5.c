/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task5.h"
#include <rtthread.h>
#include "sys_cfg.h"

void thread5_entry(void* parameter)
{
    while (1) {
        Tmr5Hanler();
        rt_thread_delay(5);
    }
}
