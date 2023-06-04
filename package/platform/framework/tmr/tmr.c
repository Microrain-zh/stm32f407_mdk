/*
 * Copyright (c) Mr.Lee. 2019. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2019-01-15
 */

#include "tmr.h"

static void TmgrStartTimerInsertPoint(const HndTmgr this, const HndTmr tmr, const HndTmr registeredTmr,
    unsigned int *startTimeOutVal)
{
    if ((registeredTmr->ramData->diffTimeOutVal <= *startTimeOutVal) ||
        (registeredTmr->ramData->prevTmr != INVALID_TMR_HANDLE)) {
        /* 插入节点不为链表头部 */
        if (registeredTmr->ramData->diffTimeOutVal <= *startTimeOutVal) {
            /* 节点插入链表尾部 */
            tmr->ramData->prevTmr = registeredTmr;
            tmr->ramData->nextTmr = INVALID_TMR_HANDLE;
            registeredTmr->ramData->nextTmr = tmr;
            *startTimeOutVal -= registeredTmr->ramData->diffTimeOutVal;
        } else {
            /* 插入节点不为链表尾部 */
            tmr->ramData->prevTmr = registeredTmr->ramData->prevTmr;
            tmr->ramData->nextTmr = registeredTmr;
            tmr->ramData->prevTmr->ramData->nextTmr = tmr;
            registeredTmr->ramData->prevTmr = tmr;
            registeredTmr->ramData->diffTimeOutVal -= *startTimeOutVal;
        }
    } else {
        /* 节点插入链表头部 */
        tmr->ramData->prevTmr = INVALID_TMR_HANDLE;
        tmr->ramData->nextTmr = registeredTmr;
        registeredTmr->ramData->prevTmr = tmr;
        registeredTmr->ramData->diffTimeOutVal -= *startTimeOutVal;
        this->firstTmr = tmr;
    }
}

void TmgrInit(const HndTmgr this)
{
    if (this == NULL) {
        return;
    }

    this->firstTmr = INVALID_TMR_HANDLE;
}

void TmgrKillTimer(const HndTmgr this, const HndTmr tmr)
{
    if ((this == NULL) || (tmr == NULL) || (tmr->ramData == NULL)) {
        return;
    }

    switch (tmr->ramData->state) {
        case TMR_STATE_TRIGGERED:
            tmr->ramData->state = TMR_STATE_INACTIVE;
            break;
        case TMR_STATE_ACTIVE:
            if (tmr->ramData->prevTmr != INVALID_TMR_HANDLE) {
                /* 杀死的定时器不在链表头部 */
                tmr->ramData->prevTmr->ramData->nextTmr = tmr->ramData->nextTmr;
            } else {
                /* 杀死的定时器在链表头部 */
                this->firstTmr = tmr->ramData->nextTmr;
            }
            if (tmr->ramData->nextTmr != INVALID_TMR_HANDLE) {
                /* 杀死的定时器不在链表尾部 */
                tmr->ramData->nextTmr->ramData->prevTmr = tmr->ramData->prevTmr;
                tmr->ramData->nextTmr->ramData->diffTimeOutVal += tmr->ramData->diffTimeOutVal;
            }
            tmr->ramData->state = TMR_STATE_INACTIVE;
            break;
        default:
            break;
    }
}

/* 开启条件：startTimeOutVal != 0*/
void TmgrStartTimer(const HndTmgr this, const HndTmr tmr, unsigned int startTimeOutVal, unsigned int cycleTimeOutVal)
{
    HndTmr registeredTmr = INVALID_TMR_HANDLE;

    if ((this == NULL) || (tmr == NULL) || (tmr->ramData == NULL)) {
        return;
    }

    switch (tmr->ramData->state) {
        case TMR_STATE_INACTIVE:
        case TMR_STATE_TRIGGERED:
            registeredTmr = this->firstTmr;
            if (registeredTmr == INVALID_TMR_HANDLE) {
                /* 没有定时器在链表内 */
                tmr->ramData->prevTmr = INVALID_TMR_HANDLE;
                tmr->ramData->nextTmr = INVALID_TMR_HANDLE;
                this->firstTmr = tmr;
            } else {
                /* 寻找插入节点 */
                while((registeredTmr->ramData != NULL) &&
                    (registeredTmr->ramData->diffTimeOutVal <= startTimeOutVal) &&
                    (registeredTmr->ramData->nextTmr != INVALID_TMR_HANDLE)) {
                    startTimeOutVal -= registeredTmr->ramData->diffTimeOutVal;
                    registeredTmr = registeredTmr->ramData->nextTmr;
                }
                TmgrStartTimerInsertPoint(this, tmr, registeredTmr, &startTimeOutVal);
            }
            tmr->ramData->diffTimeOutVal = startTimeOutVal;
            tmr->ramData->cycleTimeOutVal = cycleTimeOutVal;
            tmr->ramData->state = TMR_STATE_ACTIVE;
            break;
        default:
            break;
    }
}

void TmgrRestartTimer(const HndTmgr this, const HndTmr tmr, unsigned int startTimeOutVal, unsigned int cycleTimeOutVal)
{
    if ((this == NULL) || (tmr == NULL)) {
        return;
    }

    TmgrKillTimer(this, tmr);
    TmgrStartTimer(this, tmr, startTimeOutVal, cycleTimeOutVal);
}

void TmgrHandleTick(const HndTmgr this, unsigned int timerTick)
{
    unsigned int timerVal;
    HndTmr tmr = NULL;

    if (this == NULL) {
        return;
    }

    tmr = this->firstTmr;
    if ((tmr != INVALID_TMR_HANDLE) && (tmr->ramData != NULL)) {
        if (tmr->ramData->diffTimeOutVal >= timerTick) {
            tmr->ramData->diffTimeOutVal = tmr->ramData->diffTimeOutVal - timerTick;
        } else {
            tmr->ramData->diffTimeOutVal = TMR_TIMEOUT_TRIGGERED;
        }
        while ((tmr != INVALID_TMR_HANDLE) && (tmr->ramData->diffTimeOutVal == TMR_TIMEOUT_TRIGGERED)) {
            timerVal = tmr->ramData->cycleTimeOutVal;
            TmgrKillTimer(this, tmr);

            if (timerVal == TMR_TIMEOUT_CYCLE_SINGLE_SHOT) {
                /* 单次触发定时器 */
                tmr->ramData->state = TMR_STATE_TRIGGERED;
            } else {
                /* 周期性定时器 */
                TmgrStartTimer(this, tmr, timerVal, timerVal);
            }

            if (tmr->callbackFunc != NULL) {
                (tmr->callbackFunc)();
            }

            tmr = this->firstTmr;
        }
    }
}
