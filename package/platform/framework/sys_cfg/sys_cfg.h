/*
 * Copyright (c) Mr.Lee. 2022. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#include <stdint.h>

#ifndef __SYS_CFG_H
#define __SYS_CFG_H

#define MAX_QUEUE_DATA 32
#define BORADCAST_PROCESS_ID 0xFF

#define SYSTEM_TASK_ID 0
#define NAD_TASK_ID 1
#define CAN_TASK_ID 2
#define PLATFORM_TASK_ID 3
#define CUSTOM_TASK_ID 4
#define PROCESS_MAX_ID 5

#define TASK_CYCLE_TIME_SYSTEM 5
#define TASK_CYCLE_TIME_NAD 5
#define TASK_CYCLE_TIME_CAN 5
#define TASK_CYCLE_TIME_PLATFORM 10
#define TASK_CYCLE_TIME_CUSTOM 5

#include "tmr.h"

typedef enum {
    TIMER_TYPE_SYSTEM,
    TIMER_TYPE_NAD,
    TIMER_TYPE_CAN,
    TIMER_TYPE_PLATFORM,
    TIMER_TYPE_CUSTOM
} TaskTimerType;

uint32_t SysApiGetSystickCount(void);
void SysApiInit(void);
void SysApiTaskInit(void);
void SysApiTaskInitById(uint8_t id);
uint8_t SysCfgNumOfTask(void);

void SystemTaskInit(void);
void SystemTaskCycle(void);
void SystemTaskProcess(uint32_t event, uint16_t len, const uint8_t *param);

void CanTaskInit(void);
void CanTaskCycle(void);
void CanTaskProcess(uint32_t event, uint16_t len, const uint8_t *param);

void NadTaskInit(void);
void NadTaskCycle(void);
void NadTaskProcess(uint32_t event, uint16_t len, const uint8_t *param);

void PlatformTaskInit(void);
void PlatformTaskCycle(void);
void PlatformTaskProcess(uint32_t event, uint16_t len, const uint8_t *param);

void CustomTaskInit(void);
void CustomTaskCycle(void);
void CustomTaskProcess(uint32_t event, uint16_t len, const uint8_t *param);

void RteRunnableTask1(void);
void RteRunnableTask2(void);
void RteRunnableTask3(void);
void RteRunnableTask4(void);
void RteRunnableTask5(void);

#endif
