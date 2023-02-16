/*
 * Copyright (c) Mr.Lee. 2022. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#ifndef __SYS_CFG_H
#define __SYS_CFG_H

#include "tmr.h"

typedef enum {
    TIMER_TYPE_TASK1,
    TIMER_TYPE_TASK2,
    TIMER_TYPE_TASK3,
    TIMER_TYPE_TASK4,
    TIMER_TYPE_TASK5,
} TaskTimerType;

void Tmr1Hanler(void);
void Tmr2Hanler(void);
void Tmr3Hanler(void);
void Tmr4Hanler(void);
void Tmr5Hanler(void);

HndTmgr OsApiGetTmrgrHnd(TaskTimerType type);
#endif
