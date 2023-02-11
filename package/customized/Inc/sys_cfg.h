/*
 * Copyright (c) Mr.Lee. 2022. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#ifndef __SYS_CFG_H
#define __SYS_CFG_H

typedef enum {
    TIMER_TYPE_TASK1,
    TIMER_TYPE_TASK2,
    TIMER_TYPE_TASK3,
    TIMER_TYPE_TASK4,
    TIMER_TYPE_TASK5,
} TaskTimerType;

void Tmr1Init(void);
void Tmr2Init(void);
void Tmr3Init(void);
void Tmr4Init(void);
void Tmr5Init(void);

#endif
