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
    unsigned int    out;   /* ���׶�ָ�룬�������ԣ���Ҫת��Ϊ������ */
    unsigned int    mask;  /* ������Ԫ�ظ���(size) - 1�������д����ʱ������λ(&)���ȡ��(%)*/
    unsigned int    esize; /* Ԫ�ش�С */
    void            *data; /* ���� */
    void (*lock)(void);    /* �������� */
    void (*unlock)(void);  /* ������� */
};

int FifoInit(struct Fifo *fifo, void *buffer, unsigned int size, size_t esize, lock_fun lock, lock_fun unlock);
void FifoFree(struct Fifo *fifo);
unsigned int FifoIn(struct Fifo *fifo, const void *buf, unsigned int len);
unsigned int FifoOut(struct Fifo *fifo, void *buf, unsigned int len);
unsigned int GetFifoSize(struct Fifo *fifo);
unsigned int GetFreeSize(struct Fifo *fifo);
unsigned int FifoLen(struct Fifo *fifo);

#endif
