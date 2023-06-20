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
#include "task.h"

void thread4_entry(void *parameter)
{
    SysApiTaskInitById(PLATFORM_TASK_ID);
    static uint32_t a4  = 0;

    for (; ;) {
        rt_uint32_t d;
        if (rt_event_recv(GetTaskEventSetObj(PLATFORM_TASK_ID), TASK4_EVENT_MASK,
            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &d) == RT_EOK) {
            RteRunnableTask4();
            a4++;
            rt_thread_delay(5);
        }
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
