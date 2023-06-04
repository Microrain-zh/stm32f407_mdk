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

    for (; ;) {
        rt_uint32_t e;
        if (rt_event_recv(g_thread4Event, TASK4_EVENT_MASK, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e) !=
            RT_EOK) {
            RteRunnableTask4();
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
