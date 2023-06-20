/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "nad.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "sys_api.h"
#include "elog.h"
#include "task.h"

#define NAD_START_TIME 5
#define NAD_CYCLE_TIME 5

TMR_CALLBACK_FUNC(Nad);
TMR_CONSTRUCT(g_Nad, TMR_CALLBACK_FUNC_REF(Nad));

TMR_CALLBACK_FUNC(Nad)
{
    /* handle */
     elog_info(SYSTEM, "nad\r\n");
}

void thread2_entry(void *parameter)
{
    SysApiTaskInitById(NAD_TASK_ID);
    static uint32_t a2  = 0;

    for (; ;) {
        rt_uint32_t b;
        if (rt_event_recv(GetTaskEventSetObj(NAD_TASK_ID), TASK2_EVENT_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
            RT_WAITING_FOREVER, &b) == RT_EOK) {
        //     RteRunnableTask2();
        a2++;
            rt_thread_mdelay(5);
        }
    }
}

void NadTaskInit(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_NAD));

     TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_NAD), TMR_OBJ_HANDLE(g_Nad), NAD_START_TIME,
        NAD_CYCLE_TIME);
}

void NadTaskCycle(void)
{

}

void NadTaskProcess(uint32_t event, uint16_t len, const uint8_t *param)
{

}
