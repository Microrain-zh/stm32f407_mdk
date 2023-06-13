#include "sys_api.h"
#include "rthw.h"

volatile static rt_base_t g_level = 0;

void __DI(void)
{
    /* __disable_irq(); */
    /* __asm("CPSID I"); */
    g_level = rt_hw_interrupt_disable();
}

void __EI(void)
{
    /* __enable_irq(); */
    /* __asm("CPSIE I"); */
    rt_hw_interrupt_enable(g_level);
}

static void RteEventDeQueue(HndTask this)
{
    RteEventQueueType *paraEventQueue = NULL;
    paraEventQueue = this->ramData->eventQueueBuf;
    rt_base_t level;

    /* __DI(); */
    level = rt_hw_interrupt_disable();
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
        /* __EI(); */
        rt_hw_interrupt_enable(level);
        if (this->process != NULL) {
            this->process(paraEventQueue->rteEventData[current].eventId, paraEventQueue->rteEventData[current].paraLen,
                paraEventQueue->rteEventData[current].paraBuf);
        }
        /* __DI(); */
        level = rt_hw_interrupt_disable();
    }
    /* __EI(); */
    rt_hw_interrupt_enable(level);
}

static void RteEventQueueInit(HndTask this)
{
    RteEventQueueType *paraEventQueue = NULL;
    paraEventQueue = this->ramData->eventQueueBuf;

    /* _DI(); */
    rt_base_t level = rt_hw_interrupt_disable();

    paraEventQueue->rteClientPos = 0;
    paraEventQueue->rteServerPos = 0;
    paraEventQueue->rteFree = MAX_QUEUE_DATA;
    paraEventQueue->rteActive = 0;

    /* __EI(); */
    rt_hw_interrupt_enable(level);
}

static SysResult RteEventEnQueue(HndTask this, uint32_t event, uint16_t len, const uint8_t *buf)
{
    RteEventQueueType *paraEventQueue = NULL;
    uint8_t current;
    SysResult ret = SYS_OK;
    rt_base_t level;

    paraEventQueue = this->ramData->eventQueueBuf;
    /* __DI(); */
    level = rt_hw_interrupt_disable();
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
    /* __EI(); */
    rt_hw_interrupt_enable(level);

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
