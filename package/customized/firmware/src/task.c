/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task.h"
#include "sys_cfg.h"
#include "sys_api.h"
#include "system.h"
#include "nad.h"
#include "can.h"
#include "platform.h"
#include "custom.h"
#include "elog.h"

static struct rt_thread thread1;
static rt_uint8_t thread1_stack[512];

static struct rt_thread thread2;
static rt_uint8_t thread2_stack[512];

static struct rt_thread thread3;
static rt_uint8_t thread3_stack[512];

static struct rt_thread thread4;
static rt_uint8_t thread4_stack[512];

static struct rt_thread thread5;
static rt_uint8_t thread5_stack[512];

#define TASK1_PRIORITY          5
#define TASK2_PRIORITY          20
#define TASK3_PRIORITY          10
#define TASK4_PRIORITY          15
#define TASK5_PRIORITY          25

#define TASK1_TICK_PER_SECOND   5
#define TASK2_TICK_PER_SECOND   5
#define TASK3_TICK_PER_SECOND   5
#define TASK4_TICK_PER_SECOND   5
#define TASK5_TICK_PER_SECOND   5


rt_event_t g_thread1Event;
rt_event_t g_thread2Event;
rt_event_t g_thread3Event;
rt_event_t g_thread4Event;
rt_event_t g_thread5Event;

static void TaskEventInit(void)
{
    rt_err_t ret;

    ret = rt_event_init(g_thread1Event, "task1Mask", RT_IPC_FLAG_PRIO);
    if (ret != RT_EOK) {
        rt_kprintf("task1 init event failed %u\r\n", ret);
    }

    ret = rt_event_init(g_thread2Event, "task2Mask", RT_IPC_FLAG_PRIO);
    if (ret != RT_EOK) {
        rt_kprintf("task2 init event failed\r\n", ret);
    }

    ret = rt_event_init(g_thread3Event, "task3Mask", RT_IPC_FLAG_PRIO);
    if (ret != RT_EOK) {
        rt_kprintf("task3 init event failed\r\n", ret);
    }

    ret = rt_event_init(g_thread4Event, "task4Mask", RT_IPC_FLAG_PRIO);
    if (ret != RT_EOK) {
        rt_kprintf("task4 init event failed\r\n", ret);
    }

    ret = rt_event_init(g_thread5Event, "task5Mask", RT_IPC_FLAG_PRIO);
    if (ret != RT_EOK) {
        rt_kprintf("task5 init event failed\r\n", ret);
    }
}


/* 线程例程初始化 */
static int thread_sample_init(void)
{
     rt_err_t result;

    /* 初始化线程 1 */
    /* 线程的入口是 thread1_entry，参数是 RT_NULL
     * 线程栈是 thread1_stack
     * 优先级是 5，时间片是 5 个 OS Tick
     */
    result = rt_thread_init(&thread1, "thread1", thread1_entry, RT_NULL, &thread1_stack[0], sizeof(thread1_stack),
        TASK1_PRIORITY, TASK1_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread1);
    } else {
        rt_kprintf(SYSTEM, "thread1 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread2, "thread2", thread2_entry, RT_NULL, &thread2_stack[0], sizeof(thread2_stack),
        TASK2_PRIORITY, TASK2_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread2);
    } else {
        rt_kprintf(SYSTEM, "thread2 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread3, "thread3", thread3_entry, RT_NULL, &thread3_stack[0], sizeof(thread3_stack),
        TASK3_PRIORITY, TASK3_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread3);
    } else {
        rt_kprintf(SYSTEM, "thread3 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread4, "thread4", thread4_entry, RT_NULL, &thread4_stack[0], sizeof(thread4_stack),
        TASK4_PRIORITY, TASK4_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread4);
    } else {
        rt_kprintf(SYSTEM, "thread4 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread5, "thread5", thread5_entry, RT_NULL, &thread5_stack[0], sizeof(thread5_stack),
        TASK5_PRIORITY, TASK5_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread5);
    } else {
        rt_kprintf(SYSTEM, "thread5 init fail %u\r\n", result);
    }

    TaskEventInit();

    return 0;
}
INIT_APP_EXPORT(thread_sample_init);
