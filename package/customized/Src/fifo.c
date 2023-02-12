/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2022-12-30
 */

#include <string.h>
#include <stdbool.h>
#include "fifo.h"

#define IS_POWER_OF_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define EINVAL 22 /* Invalid argument */

static unsigned int RounddownPowOfTwo(unsigned int x)
{
    int position = 0;
    int i;
    /* 0��ȡֵ��-1��Ȼ��ȡ����λ��Ϊ32��1����32λ����Ϊ0 */
    for (i = (x-1); i != 0; ++position)
        i >>= 1;

    return 1UL << position;
}

int FifoInit(struct Fifo *fifo, void *buffer, unsigned int size, size_t esize, lock_fun lock, lock_fun unlock)
{
    size /= esize; /* �����С�����Ԫ�ظ��� */

    if (!IS_POWER_OF_2(size)) {
        /* ��չΪ2���� */
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
    return (fifo->mask + 1) - (fifo->in - fifo->out); /* in��out���ֵȡ2^32-1���޷����������Ҳ��õ���ȷֵ */
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

    off &= fifo->mask; /* ����תλ�������д���� */
    if (esize != 1) { /* Ԫ�����ʹ�С��Ϊ1�� ���Ӧд����ƫ�ƣ������С��д�볤�� */
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off); /* ȡд�볤�Ⱥͻ���ʣ���С����Сֵ */

    memcpy((unsigned char *)fifo->data + off, (unsigned char *)src, l);
    memcpy((unsigned char *)fifo->data, (unsigned char *)src + l, len - l); /* д����β�Ӷ�ͷ��ʼд */

}

unsigned int FifoIn(struct Fifo *fifo, const void *buf, unsigned int len)
{
    unsigned int l;

    l = FifoUnused(fifo);
    if (len > l)
        len = l;

    FifoCopyIn(fifo, buf, len, fifo->in);
    fifo->in += len;
    return len;
}

static void FifoCopyOut(struct Fifo *fifo, void *dst, unsigned int len, unsigned int off)
{
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;

    off &= fifo->mask;
    if (esize != 1) {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off);

    memcpy((unsigned char *)dst, (unsigned char *)fifo->data + off, l);
    memcpy((unsigned char *)dst + l, (unsigned char *)fifo->data, len - l);
}

/* ��ȡ���е�ֵ������ȡ������ */
unsigned int FifoOutPeek(struct Fifo *fifo, void *buf, unsigned int len)
{
    unsigned int l;

    l = fifo->in - fifo->out; /* ����Ԫ�ظ��� */
    if (len > l) /* ��ȡ��Ԫ�ظ���У�� */
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

/* ȡ�������Ѿ�ʹ�õ�Ԫ�ظ�������һ������ռ�û����С */
unsigned int FifoLen(struct Fifo *fifo)
{
    return(fifo->in - fifo->out);
}

bool FifoIsEmpty(struct Fifo *fifo)
{
    return (fifo->in == fifo->out);
}

/* �������ĳ��������n���߼���ַ�ռ���Ϊ0��n-1 ����дָ�����n���򻺳���Ϊ�� */
bool FifoIsFull(struct Fifo *fifo)
{
    return (FifoLen(fifo) > fifo->mask); /* дָ�� == (��ָ�� ��� ����������) */
}

unsigned int GetFifoSize(struct Fifo *fifo)
{
    return (fifo->mask + 1) * (fifo->esize);
}

unsigned int GetFreeSize(struct Fifo *fifo)
{
    return (GetFifoSize(fifo) - (FifoLen(fifo) * fifo->esize));
}
