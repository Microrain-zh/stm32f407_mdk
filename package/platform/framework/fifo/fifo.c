/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#include <string.h>
#include <stdbool.h>
#include "fifo.h"
#include "rthw.h"

#define IS_POWER_OF_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define EINVAL 22 /* Invalid argument */

static unsigned int RounddownPowOfTwo(unsigned int x)
{
    int position = 0;
    int i;
    /* 0会取值成-1，然后取到的位置为32，1左移32位后还是为0 */
    for (i = (x-1); i != 0; ++position)
        i >>= 1;

    return 1UL << position;
}

int FifoInit(struct Fifo *fifo, void *buffer, unsigned int size, size_t esize, lock_fun lock, lock_fun unlock)
{
    size /= esize; /* 缓冲大小换算可元素个数 */

    if (!IS_POWER_OF_2(size)) {
        /* 扩展为2的幂 */
        size = RounddownPowOfTwo(size);
    };

    fifo->in = 0;
    fifo->out = 0;
    fifo->esize = esize;
    fifo->data = buffer;

    if (size < 2) {
        fifo->mask = 0;
        return -EINVAL;
    }
    fifo->mask = size - 1;
    fifo->lock = lock;
    fifo->unlock = unlock;

    return 0;
}

void FifoFree(struct Fifo *fifo)
{
    fifo->in = 0;
    fifo->out = 0;
    fifo->esize = 0;
    fifo->data = NULL;
    fifo->mask = 0;
    fifo->lock = NULL;
    fifo->unlock = NULL;
}

static inline unsigned int FifoUnused(struct Fifo *fifo)
{
    return (fifo->mask + 1) - (fifo->in - fifo->out); /* in和out最大值取2^32-1，无符号溢出回绕也会得到正确值 */
}

static inline unsigned int min(unsigned int x,unsigned int y)
{
    return x < y ? x : y;
}

static void FifoCopyIn(struct Fifo *fifo, const void *src, unsigned int len, unsigned int off)
{
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;

    off &= fifo->mask; /* 求余转位运算计算写索引 */
    if (esize != 1) { /* 元素类型大小不为1， 求对应写索引偏移，缓冲大小，写入长度 */
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off); /* 取写入长度和缓冲剩余大小的最小值 */

    memcpy((unsigned char *)fifo->data + off, (unsigned char *)src, l);
    memcpy((unsigned char *)fifo->data, (unsigned char *)src + l, len - l); /* 写到队尾从队头开始写 */

}

unsigned int FifoIn(struct Fifo *fifo, const void *buf, unsigned int len)
{
    unsigned int l;

    /* fifo->lock(); */
    rt_base_t level = rt_hw_interrupt_disable();
    l = FifoUnused(fifo);
    if (len > l)
        len = l;

    FifoCopyIn(fifo, buf, len, fifo->in);
    fifo->in += len;
    /* fifo->unlock(); */
    rt_hw_interrupt_enable(level);

    return len;
}

static void FifoCopyOut(struct Fifo *fifo, void *dst, unsigned int len, unsigned int off)
{
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;

    /* fifo->lock(); */
    rt_base_t level = rt_hw_interrupt_disable();
    off &= fifo->mask;
    if (esize != 1) {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off);

    memcpy((unsigned char *)dst, (unsigned char *)fifo->data + off, l);
    memcpy((unsigned char *)dst + l, (unsigned char *)fifo->data, len - l);
    /* fifo->unlock(); */
    rt_hw_interrupt_enable(level);
}

/* 读取队列的值，不会取出队列 */
unsigned int FifoOutPeek(struct Fifo *fifo, void *buf, unsigned int len)
{
    unsigned int l;

    l = fifo->in - fifo->out; /* 队列元素个数 */
    if (len > l) /* 读取的元素个数校验 */
        len = l;

    FifoCopyOut(fifo, buf, len, fifo->out);

    return len;
}

unsigned int FifoOut(struct Fifo *fifo, void *buf, unsigned int len)
{
    len = FifoOutPeek(fifo, buf, len);
    fifo->out += len;

    return len;
}

/* 取队列中已经使用的元素个数，不一定等于占用缓冲大小 */
unsigned int FifoLen(struct Fifo *fifo)
{
    return(fifo->in - fifo->out);
}

bool FifoIsEmpty(struct Fifo *fifo)
{
    return (fifo->in == fifo->out);
}

/* 缓冲区的长度如果是n，逻辑地址空间则为0至n-1 ，读写指针相差n，则缓冲区为满 */
bool FifoIsFull(struct Fifo *fifo)
{
    return (FifoLen(fifo) > fifo->mask); /* 写指针 == (读指针 异或 缓冲区长度) */
}

unsigned int GetFifoSize(struct Fifo *fifo)
{
    return (fifo->mask + 1) * (fifo->esize);
}

unsigned int GetFreeSize(struct Fifo *fifo)
{
    return (GetFifoSize(fifo) - (FifoLen(fifo) * fifo->esize));
}
