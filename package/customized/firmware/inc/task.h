/*
 * Copyright (c) Mr.Lee 2023 All rights reserved.
 * Description:
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#ifndef __TASK_H
#define __TASK_H

#include <stdio.h>
#include <rtthread.h>

#define TASK1_EVENT_MASK 0x01
#define TASK2_EVENT_MASK 0x02
#define TASK3_EVENT_MASK 0x04
#define TASK4_EVENT_MASK 0x08
#define TASK5_EVENT_MASK 0x10

rt_event_t GetTaskEventSetObj(uint8_t index);

#endif

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
