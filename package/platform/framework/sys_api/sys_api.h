#ifndef __SYS_API_H
#define __SYS_API_H

#include "sys_cfg.h"
#include "tmr.h"

typedef enum {
    OP_ENTER,
    OP_EVENT,
    OP_EXIT
} BaseOp;

typedef enum {
    SYS_OK,
    SYS_ERR
} SysResult;

enum {
    EVT_NULL
};

typedef void (*OnEventHandlerFunc)(uint16_t len, const uint8_t *buf);

typedef struct {
    uint32_t event;
    OnEventHandlerFunc eventHandler;
} EventFuncItem;

typedef void (*OnEventHandler)(uintptr_t param);

typedef struct {
    uint32_t event;
    OnEventHandler eventHandler;
} EventItem;

typedef struct {
    uint8_t processId;
    uint32_t eventId;
    uint16_t paraLen;
    const uint8_t *paraBuf;
} RteEventDataType;

typedef struct {
    RteEventDataType rteEventData[MAX_QUEUE_DATA];
    uint8_t rteClientPos;
    uint8_t rteServerPos;
    uint8_t rteFree;
    uint8_t rteActive;
} RteEventQueueType;

struct TagTask;
typedef const struct TagTask *HndTask;

typedef void (*FpTaskInitCallback)(void);
typedef void (*FpTaskCycleCallback)(void);
typedef void (*FpTaskProcessCallback)(uint32_t event, uint16_t len, const uint8_t *param);

typedef struct {
    HndTask taskHandle;
} TaskProcessType;

#define TASK_CALLBACK_FUNC(str) void TaskCallback##str(void)
#define TASK_CALLBACK_FUNC_REF(str) TaskCallback##str
#define TASK_OBJ_HANDLE(objName) (HndTask)(&objName)
#define TASK_OBJ_REF(objName) (const struct TagTask *)(&objName)
#define INVALID_TASK_HANDLE NULL

typedef struct {
    RteEventQueueType *eventQueueBuf;
} TaskRamType;

typedef struct TagTask {
    TaskRamType *ramData;
    uint8_t taskId;
    FpTaskInitCallback init;
    FpTaskCycleCallback taskCycle;
    FpTaskProcessCallback process;
} TaskObjType;

#define TASK_CONSTRUCT(objName, TaskId, InitFunc, TaskCycle, ProcessFunc) \
    static RteEventQueueType objName##EventQueueBuf = { 0 }; \
    static TaskRamType objName##Ram = { \
        &objName##EventQueueBuf, \
    }; \
    const struct TagTask objName = { \
        &objName##Ram, \
        TaskId, \
        InitFunc, \
        TaskCycle, \
        ProcessFunc \
    }

void __DI(void);
void __EI(void);
HndTmgr OsApiGetTmrgrHnd(TaskTimerType type);
HndTask SysCfgGetTaskHndById(uint8_t id);
HndTask SysCfgGetTaskHndByIndex(uint8_t index);
void SysApiInitEventQueue(HndTask this);
void SysApiProcessEvent(HndTask this);
SysResult SysApiSendEventDataToProcess(uint8_t processId, uint32_t event, uint16_t paraLen, uintptr_t paraBuf);
void ProcessOnEventFunc(uint32_t event, uint16_t len, const uint8_t *buf, const EventFuncItem *eventFuncTbl,
    uint32_t tblSize);

#endif
