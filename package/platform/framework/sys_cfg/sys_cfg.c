/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "sys_cfg.h"
#include "sys_api.h"
#include "stm32f4xx_hal.h"
#include "rtthread.h"
#include "key_detect.h"
#include "elog.h"

#define TASK_NUM sizeof(g_taskHandleTable) / sizeof(HndTask)

TASK_CONSTRUCT(System, SYSTEM_TASK_ID, SystemTaskInit, SystemTaskCycle, SystemTaskProcess);
TASK_CONSTRUCT(Nad, NAD_TASK_ID, NadTaskInit, NadTaskCycle, NadTaskProcess);
TASK_CONSTRUCT(Can, CAN_TASK_ID, CanTaskInit, CanTaskCycle, CanTaskProcess);
TASK_CONSTRUCT(Platform, PLATFORM_TASK_ID, PlatformTaskInit, PlatformTaskCycle, PlatformTaskProcess);
TASK_CONSTRUCT(Custom, CUSTOM_TASK_ID, CustomTaskInit, CustomTaskCycle, CustomTaskProcess);

static const HndTask g_taskHandleTable[] = {
    TASK_OBJ_HANDLE(System),
    TASK_OBJ_HANDLE(Nad),
    TASK_OBJ_HANDLE(Can),
    TASK_OBJ_HANDLE(Platform),
    TASK_OBJ_HANDLE(Custom)
};

TMGR_CONSTRUCT(g_systemTmrMgr);
TMGR_CONSTRUCT(g_canTmrMgr);
TMGR_CONSTRUCT(g_nadTmrMgr);
TMGR_CONSTRUCT(g_platformTmrMgr);
TMGR_CONSTRUCT(g_customTmrMgr);

static void SysApiEventQueueInit(void)
{
    uint8_t i;
    HndTask taskHandle = NULL;

    for (i = 0; i < TASK_NUM; i++) {
        taskHandle = SysCfgGetTaskHndByIndex(i);
        if (taskHandle != NULL) {
            SysApiInitEventQueue(taskHandle);
        }
    }
}

uint32_t SysApiGetSystickCount(void)
{
    return rt_tick_get();
}

void SysApiInit(void)
{
    SysApiEventQueueInit();
}

void SysApiTaskInit(void)
{
    uint8_t i;
    HndTask taskHandle = NULL;

    for (i = 0; i < TASK_NUM; i++) {
        taskHandle = SysCfgGetTaskHndByIndex(i);
        if ((taskHandle != NULL) && (taskHandle->init != NULL)) {
            taskHandle->init();
        }
    }
}

void SysApiTaskInitById(uint8_t id)
{
    HndTask taskHandle = NULL;

    taskHandle = SysCfgGetTaskHndById(id);
    if (taskHandle != NULL) {
        taskHandle->init();
    }
}

HndTask SysCfgGetTaskHndById(uint8_t id)
{
    uint8_t i;
    for (i = 0; i < TASK_NUM; i++) {
        if (id == g_taskHandleTable[i]->taskId) {
            return g_taskHandleTable[i];
        }
    }
    return NULL;
}

HndTask SysCfgGetTaskHndByIndex(uint8_t index)
{
    if (index < TASK_NUM) {
        return g_taskHandleTable[index];
    }
    return NULL;
}

uint8_t SysCfgNumOfTask(void)
{
    return TASK_NUM;
}

TMR_CALLBACK_FUNC(Task2Log);
TMR_CONSTRUCT(g_task2Log, TMR_CALLBACK_FUNC_REF(Task2Log));

TMR_CALLBACK_FUNC(Task3Log);
TMR_CONSTRUCT(g_task3Log, TMR_CALLBACK_FUNC_REF(Task3Log));

TMR_CALLBACK_FUNC(Task4Log);
TMR_CONSTRUCT(g_task4Log, TMR_CALLBACK_FUNC_REF(Task4Log));

TMR_CALLBACK_FUNC(Task5Log);
TMR_CONSTRUCT(g_task5Log, TMR_CALLBACK_FUNC_REF(Task5Log));

TMR_CALLBACK_FUNC(Task1Log)
{
    elog_info(SYSTEM, "task1\r\n");
}

TMR_CALLBACK_FUNC(Task2Log)
{
    /* handle */
    elog_info(SYSTEM, "task2\r\n");
}

TMR_CALLBACK_FUNC(Task3Log)
{
    /* handle */
    elog_info(SYSTEM, "task3\r\n");
}

TMR_CALLBACK_FUNC(Task4Log)
{
    /* handle */
    elog_info(SYSTEM, "task4\r\n");
}

TMR_CALLBACK_FUNC(Task5Log)
{
    /* handle */
    elog_info(SYSTEM, "task5\r\n");
}

HndTmgr OsApiGetTmrgrHnd(TaskTimerType type)
{
    HndTmgr tmgr = NULL;

    switch (type) {
        case TIMER_TYPE_SYSTEM:
            tmgr = TMGR_OBJ_HANDLE(g_systemTmrMgr);
            break;
        case TIMER_TYPE_NAD:
            tmgr = TMGR_OBJ_HANDLE(g_canTmrMgr);
            break;
        case TIMER_TYPE_CAN:
            tmgr = TMGR_OBJ_HANDLE(g_nadTmrMgr);
            break;
        case TIMER_TYPE_PLATFORM:
            tmgr = TMGR_OBJ_HANDLE(g_platformTmrMgr);
            break;
        case TIMER_TYPE_CUSTOM:
            tmgr = TMGR_OBJ_HANDLE(g_customTmrMgr);
            break;
        default:
            break;
    }

    return tmgr;
}

void RteRunnableTask1(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_systemTmrMgr), TASK_CYCLE_TIME_SYSTEM);
    SysApiProcessEvent(SysCfgGetTaskHndById(SYSTEM_TASK_ID));
}

void RteRunnableTask2(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_nadTmrMgr), TIMER_TYPE_NAD);
    SysApiProcessEvent(SysCfgGetTaskHndById(NAD_TASK_ID));
}

void RteRunnableTask3(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_canTmrMgr), TASK_CYCLE_TIME_CAN);
    SysApiProcessEvent(SysCfgGetTaskHndById(CAN_TASK_ID));
}

void RteRunnableTask4(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_platformTmrMgr), TASK_CYCLE_TIME_PLATFORM);
    SysApiProcessEvent(SysCfgGetTaskHndById(PLATFORM_TASK_ID));
}

void RteRunnableTask5(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_customTmrMgr), TASK_CYCLE_TIME_CUSTOM);
    SysApiProcessEvent(SysCfgGetTaskHndById(CUSTOM_TASK_ID));
}
