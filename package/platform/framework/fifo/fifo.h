/*
 * Copyright (c) Mr.Lee. 2022. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>

typedef void (*lock_fun)(void);

struct Fifo {
    unsigned int    in;
    unsigned int    out;   /* 队首读指针，镜像特性，需要转化为读索引 */
    unsigned int    mask;  /* 缓冲区元素个数(size) - 1，计算读写索引时可以用位(&)替代取余(%)*/
    unsigned int    esize; /* 元素大小 */
    void            *data; /* 数据 */
    void (*lock)(void);    /* 互斥上锁 */
    void (*unlock)(void);  /* 互斥解锁 */
};

int FifoInit(struct Fifo *fifo, void *buffer, unsigned int size, size_t esize, lock_fun lock, lock_fun unlock);
void FifoFree(struct Fifo *fifo);
unsigned int FifoIn(struct Fifo *fifo, const void *buf, unsigned int len);
unsigned int FifoOut(struct Fifo *fifo, void *buf, unsigned int len);
unsigned int GetFifoSize(struct Fifo *fifo);
unsigned int GetFreeSize(struct Fifo *fifo);
unsigned int FifoLen(struct Fifo *fifo);

#endif
