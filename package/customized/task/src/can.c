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
     elog_info(SYSTEM, "nad\r\n");
}

void thread3_entry(void *  parameter)
{
    SysApiTaskInitById(CAN_TASK_ID);

    for (; ;) {
        rt_uint32_t e;
        if (rt_event_recv(g_thread3Event, TASK3_EVENT_MASK, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e) !=
            RT_EOK) {
            RteRunnableTask3();
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
