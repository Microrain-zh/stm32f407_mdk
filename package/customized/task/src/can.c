/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "can.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "sys_api.h"
#include "elog.h"
#include "task.h"

#define CAN_START_TIME 5
#define CAN_CYCLE_TIME 5

TMR_CALLBACK_FUNC(Can);
TMR_CONSTRUCT(g_Can, TMR_CALLBACK_FUNC_REF(Can));

TMR_CALLBACK_FUNC(Can)
{
    /* handle */
     elog_info(SYSTEM, "can\r\n");
}

void thread3_entry(void *parameter)
{
    SysApiTaskInitById(CAN_TASK_ID);
    static uint32_t a3  = 0;

    for (; ;) {
        rt_uint32_t c;
        if (rt_event_recv(GetTaskEventSetObj(CAN_TASK_ID), TASK3_EVENT_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
            RT_WAITING_FOREVER, &c) == RT_EOK) {
            RteRunnableTask3();
            a3++;
            rt_thread_delay(5);
        }
    }
}

void CanTaskInit(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_CAN));

     TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_CAN), TMR_OBJ_HANDLE(g_Can), CAN_START_TIME,
        CAN_CYCLE_TIME);
}

void CanTaskCycle(void)
{

}

void CanTaskProcess(uint32_t event, uint16_t len, const uint8_t *param)
{

}
