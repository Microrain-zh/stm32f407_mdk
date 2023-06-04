/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "platform.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "sys_api.h"
#include "key_detect.h"

void thread4_entry(void *parameter)
{
    SysApiTaskInitById(PLATFORM_TASK_ID);

    for (; ;) {
        RteRunnableTask4();
        rt_thread_delay(5);
    }
}

void PlatformTaskInit(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_PLATFORM));

    ButtonNavigation(OP_ENTER, 0, NULL);
}

void PlatformTaskCycle(void)
{

}

void PlatformTaskProcess(uint32_t event, uint16_t len, const uint8_t *param)
{

}
