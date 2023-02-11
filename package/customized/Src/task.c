/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "task.h"
#include <rtthread.h>

//#include "sys_cfg.h"
//#include "tmr.h"

//#define KEY_EVENT_START_TIME 5
//#define KEY_EVENT_CYCLE_TIME 5

//TMR_CALLBACK_FUNC(KeyEvent);
//TMR_CONSTRUCT(g_keyEvent, TMR_CALLBACK_FUNC_REF(KeyEvent));

//void Task1Init(void)
//{
//    TmgrInit(OsApiGetTmrgrHnd(TIMER_TYPE_TASK1));

//    TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_TASK1), TMR_OBJ_HANDLE(g_keyEvent), KEY_EVENT_START_TIME,
//        KEY_EVENT_CYCLE_TIME);
//}

//TMR_CALLBACK_FUNC(KeyEvent)
//{
//    /* handle */
//}

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

void thread1_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("task1 %d\n", i);
        }

        (void)rt_thread_yield();
    }
}

void thread2_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("task2 %d\n", i);
        }

        (void)rt_thread_yield();
    }
}

void thread3_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("task3 %d\n", i);
        }

        (void)rt_thread_yield();
    }
}

void thread4_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("task4 %d\n", i);
        }

        (void)rt_thread_yield();
    }
}

void thread5_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("task5 %d\n", i);
        }

        (void)rt_thread_yield();
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
    result = rt_thread_init(&thread1, "thread1", thread1_entry, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), 5, 5);
    if (result == RT_EOK) {
        rt_thread_startup(&thread1);
    } else {
        rt_kprintf("thread1 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread2, "thread2", thread2_entry, RT_NULL, &thread2_stack[0], sizeof(thread2_stack), 10, 5);
    if (result == RT_EOK) {
        rt_thread_startup(&thread2);
    } else {
        rt_kprintf("thread2 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread3, "thread3", thread3_entry, RT_NULL, &thread3_stack[0], sizeof(thread3_stack), 15, 5);
    if (result == RT_EOK) {
        rt_thread_startup(&thread3);
    } else {
        rt_kprintf("thread3 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread4, "thread4", thread4_entry, RT_NULL, &thread4_stack[0], sizeof(thread4_stack), 20, 5);
    if (result == RT_EOK) {
        rt_thread_startup(&thread4);
    } else {
        rt_kprintf("thread4 init fail %u\r\n", result);
    }

    result = rt_thread_init(&thread5, "thread5", thread5_entry, RT_NULL, &thread5_stack[0], sizeof(thread5_stack), 25, 5);
    if (result == RT_EOK) {
        rt_thread_startup(&thread5);
    } else {
        rt_kprintf("thread5 init fail %u\r\n", result);
    }

    return 0;
}
INIT_APP_EXPORT(thread_sample_init);
