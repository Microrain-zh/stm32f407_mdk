/*
 * Copyright (c) Mr.Lee. 2019. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2019-01-15
 */

#ifndef __TMR_H
#define __TMR_H

#include <stddef.h>

struct TagTmr;
typedef const struct TagTmr *HndTmr;
typedef void (*FpTmrCallback)(void);

#define TMR_STATE_INACTIVE   0

#define TMR_STATE_TRIGGERED  1
#define TMR_STATE_ACTIVE     2

#define TMR_CALLBACK_FUNC(str) void TmrCallback##str(void)
#define TMR_CALLBACK_FUNC_REF(str) TmrCallback##str

#define USE_EXTERN_TMR_OBJ(objName) extern const struct TagTmr objName

#define TMR_OBJ_HANDLE(objName) (HndTmr)(&objName)
#define TMR_OBJ_REF(objName) (const struct TagTmr *)(&objName)

#define INVALID_TMR_HANDLE NULL
#define TMR_TIMEOUT_TRIGGERED 0
#define TMR_TIMEOUT_CYCLE_SINGLE_SHOT TMR_TIMEOUT_TRIGGERED

typedef struct {
    unsigned char state;
    unsigned int diffTimeOutVal;
    unsigned int cycleTimeOutVal;
    HndTmr prevTmr;
    HndTmr nextTmr;
} TmrRam;

typedef struct TagTmr {
    TmrRam *ramData;
    FpTmrCallback callbackFunc;
} Tmr;

/* TMGR timer Manager */
struct TagTmgr;
typedef struct TagTmgr *HndTmgr;
typedef enum {
    TMGR_ERR_TMR_NO_ERROR,
    TMGR_ERR_TMR_INACTIVE,
    TMGR_ERR_TMR_TRIGGERED,
    TMGR_ERR_TMR_ACTIVE,
    TMGR_ERR_TMR_UNKNOWN_STATE
} TmgrResult;

#define USE_EXTERN_TMGR_OBJ(objName) extern struct TagTmgr objName

#define TMGR_OBJ_HANDLE(objName) (HndTmgr)(&objName)
#define TMGR_OBJ_REF(objName) (struct TagTmgr *)(&objName)

#define INVALID_TMGR_HANDLE NULL
 
typedef struct TagTmgr {
    HndTmr firstTmr;
} Tmgr;

/* TMR对象构造器 */
#define TMR_CONSTRUCT(objName, callbackFunc) \
        TmrRam objName##Ram = { \
        TMR_STATE_INACTIVE, TMR_TIMEOUT_TRIGGERED, TMR_TIMEOUT_CYCLE_SINGLE_SHOT, (HndTmr)NULL, (HndTmr)NULL}; \
    const struct TagTmr objName = {&objName##Ram, callbackFunc}

/* TMGR对象构造器 */
#define TMGR_CONSTRUCT(objName) struct TagTmgr objName = {(HndTmr)NULL}

/* 初始化TMR头指针为空 */
void TmgrInit(const HndTmgr this);
/* 从TMR链表中删除指定的TMR */
void TmgrKillTimer(const HndTmgr this, const HndTmr tmr);
/* 开启指定TMR，如果TMR处于开启状态直接返回 */
void TmgrStartTimer(const HndTmgr this, const HndTmr tmr, unsigned int startTimeOutVal, unsigned int cycleTimeOutVal);
/* 先关闭再开启TMR，TMR的时间重新设置 */
void TmgrRestartTimer(const HndTmgr this, const HndTmr tmr, unsigned int startTimeOutVal, unsigned int cycleTimeOutVal);
/* 系统时间到达后对TMR链表进行处理 */
void TmgrHandleTick(const HndTmgr this, unsigned int timerTick);

#endif
