/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "sys_cfg.h"

TMGR_CONSTRUCT(g_task1TmrTmgr);
TMGR_CONSTRUCT(g_task2TmrTmgr);
TMGR_CONSTRUCT(g_task3TmrTmgr);
TMGR_CONSTRUCT(g_task4TmrTmgr);
TMGR_CONSTRUCT(g_task5TmrTmgr);

#define TASK_CYCLE_TIME_TASK1 5
#define TASK_CYCLE_TIME_TASK2 5
#define TASK_CYCLE_TIME_TASK3 10
#define TASK_CYCLE_TIME_TASK4 5
#define TASK_CYCLE_TIME_TASK5 5

HndTmgr OsApiGetTmrgrHnd(TaskTimerType type)
{
    HndTmgr tmgr = NULL;

    switch (type) {
        case TIMER_TYPE_TASK1:
            tmgr = TMGR_OBJ_HANDLE(g_task1TmrTmgr);
            break;
        case TIMER_TYPE_TASK2:
            tmgr = TMGR_OBJ_HANDLE(g_task2TmrTmgr);
            break;
        case TIMER_TYPE_TASK3:
            tmgr = TMGR_OBJ_HANDLE(g_task3TmrTmgr);
            break;
        case TIMER_TYPE_TASK4:
            tmgr = TMGR_OBJ_HANDLE(g_task4TmrTmgr);
            break;
        case TIMER_TYPE_TASK5:
            tmgr = TMGR_OBJ_HANDLE(g_task5TmrTmgr);
            break;
        default:
            break;
    }

    return tmgr;
}

void Tmr1Init(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK1));
}

void Tmr2Init(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK2));
}

void Tmr3Init(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK3));
}

void Tmr4Init(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK4));
}

void Tmr5Init(void)
{
    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK5));
}

void Tmr1Hanler(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_task1TmrTmgr), TASK_CYCLE_TIME_TASK1);
}

void Tmr2Hanler(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_task2TmrTmgr), TASK_CYCLE_TIME_TASK2);
}

void Tmr3Hanler(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_task3TmrTmgr), TASK_CYCLE_TIME_TASK3);
}

void Tmr4Hanler(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_task4TmrTmgr), TASK_CYCLE_TIME_TASK4);
}
void Tmr5Hanler(void)
{
    TmgrHandleTick(TMGR_OBJ_HANDLE(g_task5TmrTmgr), TASK_CYCLE_TIME_TASK5);
}
