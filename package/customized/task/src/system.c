/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "system.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "sys_api.h"
#include "elog.h"
#include "task.h"

#define SYSTEM_START_TIME 5
#define SYSTEM_CYCLE_TIME 5

TMR_CALLBACK_FUNC(System);
TMR_CONSTRUCT(g_System, TMR_CALLBACK_FUNC_REF(System));

TMR_CALLBACK_FUNC(System)
{
    /* handle */
     elog_info(SYSTEM, "system\r\n");
}

void thread1_entry(void *parameter)
{
    SysApiTaskInitById(SYSTEM_TASK_ID);

    for (; ;) {
        rt_uint32_t e;
        if (rt_event_recv(g_thread1Event, TASK1_EVENT_MASK, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e) !=
            RT_EOK) {
            RteRunnableTask1();
        }
    }
}

void SystemTaskInit(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_SYSTEM));

     TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_SYSTEM), TMR_OBJ_HANDLE(g_System), SYSTEM_START_TIME,
        SYSTEM_CYCLE_TIME);
}

void SystemTaskCycle(void)
{

}

void SystemTaskProcess(uint32_t event, uint16_t len, const uint8_t *param)
{

}
