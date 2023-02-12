/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task.h"
#include <rtthread.h>
#include "sys_cfg.h"
#include "key_detect.h"

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
#define TASK2_PRIORITY          10
#define TASK3_PRIORITY          15
#define TASK4_PRIORITY          20
#define TASK5_PRIORITY          25
#define TASK1_TICK_PER_SECOND   5
#define TASK2_TICK_PER_SECOND   5
#define TASK3_TICK_PER_SECOND   10
#define TASK4_TICK_PER_SECOND   5
#define TASK5_TICK_PER_SECOND   5

void thread1_entry(void* parameter)
{
    Tmr1Init();

    while (1) {
        Tmr1Hanler();
        rt_thread_delay(5);
    }
}

void thread2_entry(void* parameter)
{
    Tmr2Init();

    while (1) {
        Tmr2Hanler();
        rt_thread_delay(5);
    }
}

void thread3_entry(void* parameter)
{
    Tmr3Init();

    while (1) {
        Tmr3Hanler();
        rt_thread_delay(5);
    }
}

void thread4_entry(void* parameter)
{
    Tmr4Init();

    while (1) {
        Tmr4Hanler();
        rt_thread_delay(5);
    }
}

void thread5_entry(void* parameter)
{
    Tmr5Init();
    KeyDetectInit();

    while (1) {
        Tmr5Hanler();
        rt_thread_delay(5);
    }
}

/* 线程例程初始化 */
static int thread_sample_init(void)
{
     rt_err_t result;

    /* 初始化线程 1 */
    /* 线程的入口是 thread1_entry，参数是 RT_NULL
     * 线程栈是 thread1_stack
     * 优先级是 200，时间片是 10 个 OS Tick
     */
    result = rt_thread_init(&thread1, "thread1", thread1_entry, RT_NULL, &thread1_stack[0], sizeof(thread1_stack),
        TASK1_PRIORITY, TASK1_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread1);
    } else {
        rt_kprintf("thread1 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread2, "thread2", thread2_entry, RT_NULL, &thread2_stack[0], sizeof(thread2_stack),
        TASK2_PRIORITY, TASK2_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread2);
    } else {
        rt_kprintf("thread2 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread3, "thread3", thread3_entry, RT_NULL, &thread3_stack[0], sizeof(thread3_stack),
        TASK3_PRIORITY, TASK3_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread3);
    } else {
        rt_kprintf("thread3 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread4, "thread4", thread4_entry, RT_NULL, &thread4_stack[0], sizeof(thread4_stack),
        TASK4_PRIORITY, TASK4_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread4);
    } else {
        rt_kprintf("thread4 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread5, "thread5", thread5_entry, RT_NULL, &thread5_stack[0], sizeof(thread5_stack),
        TASK5_PRIORITY, TASK5_TICK_PER_SECOND);
    if (result == RT_EOK) {
        rt_thread_startup(&thread5);
    } else {
        rt_kprintf("thread5 init fail %u\r\n", result);
    }

    return 0;
}
INIT_APP_EXPORT(thread_sample_init);

