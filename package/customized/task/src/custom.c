/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "custom.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "sys_api.h"
#include "elog.h"
#include "task.h"

#define CUSTOM_START_TIME 5
#define CUSTOM_CYCLE_TIME 5

TMR_CALLBACK_FUNC(Custom);
TMR_CONSTRUCT(g_Custom, TMR_CALLBACK_FUNC_REF(Custom));

TMR_CALLBACK_FUNC(Custom)
{
    /* handle */
     elog_info(SYSTEM, "custom\r\n");
}

void thread5_entry(void *parameter)
{
    SysApiTaskInitById(CUSTOM_TASK_ID);
    static uint32_t a5  = 0;

    for (; ;) {
        rt_uint32_t e;
        if (rt_event_recv(GetTaskEventSetObj(CUSTOM_TASK_ID), TASK5_EVENT_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
            RT_WAITING_FOREVER, &e) == RT_EOK) {
            RteRunnableTask5();
            a5++;
            // rt_thread_delay(5);
        }
    }
}

void CustomTaskInit(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_CUSTOM));

     TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_CUSTOM), TMR_OBJ_HANDLE(g_Custom), CUSTOM_START_TIME,
        CUSTOM_CYCLE_TIME);
}

void CustomTaskCycle(void)
{

}

void CustomTaskProcess(uint32_t event, uint16_t len, const uint8_t *param)
{

}
