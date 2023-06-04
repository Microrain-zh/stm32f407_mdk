#include "sys_api.h"

void __DI(void)
{
    /* __disable_irq(); */
    __asm("CPSID I");
}

void __EI(void)
{
    /* __enable_irq(); */
    __asm("CPSIE I");
}

static void RteEventDeQueue(HndTask this)
{
    RteEventQueueType *paraEventQueue = NULL;
    paraEventQueue = this->ramData->eventQueueBuf;

    __DI();
    while (paraEventQueue->rteActive > 0) {

        uint8_t current;
        paraEventQueue->rteActive--;
        current = paraEventQueue->rteServerPos;
        if (current < MAX_QUEUE_DATA - 1) {
            ++current;
        } else {
            current = 0;
        }
        paraEventQueue->rteServerPos = current;
        __EI();
        if (this->process != NULL) {
            this->process(paraEventQueue->rteEventData[current].eventId, paraEventQueue->rteEventData[current].paraLen,
                paraEventQueue->rteEventData[current].paraBuf);
        }
        __DI();
    }
    __EI();
}

static void RteEventQueueInit(HndTask this)
{
    RteEventQueueType *paraEventQueue = NULL;
    paraEventQueue = this->ramData->eventQueueBuf;

    __DI();

    paraEventQueue->rteClientPos = 0;
    paraEventQueue->rteServerPos = 0;
    paraEventQueue->rteFree = MAX_QUEUE_DATA;
    paraEventQueue->rteActive = 0;

    __EI();
}

static SysResult RteEventEnQueue(HndTask this, uint32_t event, uint16_t len, const uint8_t *buf)
{
    RteEventQueueType *paraEventQueue = NULL;
    uint8_t current;
    SysResult ret = SYS_OK;

    paraEventQueue = this->ramData->eventQueueBuf;
    __DI();
    if (paraEventQueue->rteFree > 0) {
        paraEventQueue->rteFree--;
        paraEventQueue->rteActive++;

        current = paraEventQueue->rteClientPos;
        if (current < MAX_QUEUE_DATA - 1) {
            ++current;
        } else {
            current = 0;
        }
        paraEventQueue->rteClientPos = current;
        paraEventQueue->rteEventData[current].eventId = event;
        paraEventQueue->rteEventData[current].paraLen = len;
        paraEventQueue->rteEventData[current].paraBuf = buf;
        paraEventQueue->rteFree++;
    } else {
        ret = SYS_ERR;
    }
    __EI();

    return ret;
}

static SysResult SendEventDataToProcessBroadcast(uint8_t processId, uint32_t event, uint16_t paraLen, uintptr_t paraBuf)
{
    uint8_t i;
    HndTask taskHandle = NULL;
    uint8_t numOfTask = SysCfgNumOfTask();

    for (i = 0; i < numOfTask; i++) {
        taskHandle = SysCfgGetTaskHndByIndex(i);
        if (taskHandle != NULL) {
            RteEventEnQueue(taskHandle, event, paraLen, (const uint8_t *)paraBuf);
        }
    }

    return SYS_OK;
}

void SysApiInitEventQueue(HndTask this)
{
    if (this != NULL) {
        RteEventQueueInit(this);
    }
}

void SysApiProcessEvent(HndTask this)
{
    if (this != NULL) {
        RteEventDeQueue(this);
    }
}

SysResult SysApiSendEventDataToProcess(uint8_t processId, uint32_t event, uint16_t paraLen, uintptr_t paraBuf)
{
    SysResult ret = SYS_ERR;
    HndTask taskHandle = NULL;

    if ((event != EVT_NULL) && (((paraLen != 0) && ((void *)paraBuf != NULL)) || (paraLen == 0))) {
        if (processId == BORADCAST_PROCESS_ID) {
            SendEventDataToProcessBroadcast(processId, event, paraLen, paraBuf);
        } else {
            taskHandle = SysCfgGetTaskHndById(processId);
            if (taskHandle != NULL) {
                RteEventEnQueue(taskHandle, event, paraLen, (const uint8_t *)paraBuf);
            }
        }
    }

    return ret;
}

void ProcessOnEventFunc(uint32_t event, uint16_t len, const uint8_t *buf, const EventFuncItem *eventFuncTbl,
    uint32_t tblSize)
{
    uint32_t i;
    OnEventHandlerFunc func = NULL;

    if ((event == EVT_NULL) || (eventFuncTbl == NULL) || (tblSize == 0)) {
        return;
    }

    for (i = 0; i < tblSize; i++) {
        if (event == eventFuncTbl[i].event) {
            func = eventFuncTbl[i].eventHandler;
            if (func != NULL) {
                func(len, buf);
            }
            break;
        }
    }
}
