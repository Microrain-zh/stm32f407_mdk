/*
 * Copyright (c) Mr.Lee. 2019. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2019-01-15
 */

#include "key_detect.h"
#include "rtthread.h"
#include "multi_button.h"
#include "main.h"
#include "tmr.h"
#include "sys_cfg.h"
#include "elog.h"

#define KEY_DETECT_START_TIME 5
#define KEY_DETECT_CYCLE_TIME 5

TMR_CALLBACK_FUNC(KeyDetect);
TMR_CONSTRUCT(g_keyDetect, TMR_CALLBACK_FUNC_REF(KeyDetect));

typedef enum {
    KEY0,
    KEY1,
    KEY2,
    KEY_UP,
    KEY_MAX
} ButtonId;

Button g_key0;
Button g_key1;
Button g_key2;
Button g_keyUp;

static uint8_t ReadButtonGPIO(uint8_t buttonId)
{
    // you can share the GPIO read function with multiple Buttons
    switch(buttonId)
    {
        case KEY0:
            return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY0_Pin);
        case KEY1:
            return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY1_Pin);
        case KEY2:
            return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
        case KEY_UP:
            return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
        default:
            elog_info(BUTTON, "key id out of range %u\r\n", buttonId);
            return 0;
    }
}

static void KeyPressDownHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u press down invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u press down\r\n", keyId);
    }
}

static void KeyPressUpHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u press up invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u press up\r\n", keyId);
    }
}

static void KeyPressRepeatHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u press repeat invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u press repeat\r\n", keyId);
    }
}

static void KeySingleClickHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u signal click invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u signal click\r\n", keyId);
    }
}

static void KeyDoubleClickHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u double click invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u double click\r\n", keyId);
    }
}

static void KeyLongPressStartHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u long press start invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u long press start\r\n", keyId);
    }
}

static void KeyLongPressHoldHandler(void *handle)
{
    uint8_t keyId = ((Button *)handle)->button_id;

    if (keyId >= KEY_MAX) {
        elog_info(BUTTON, "key id %u long press hold invalid\r\n", keyId);
    } else {
        elog_info(BUTTON, "key id %u long press hold\r\n", keyId);
    }
}

void KeyDetectInit(void)
{
    const Button *tmp[KEY_MAX] = {&g_key0, &g_key1, &g_key2, &g_keyUp};
    
    button_init(&g_key0, ReadButtonGPIO, 0, KEY0);
    button_init(&g_key1, ReadButtonGPIO, 0, KEY1);
    button_init(&g_key2, ReadButtonGPIO, 0, KEY2);
    button_init(&g_keyUp, ReadButtonGPIO, 1, KEY_UP);

    for (uint8_t i=0; i<KEY_MAX; i++) {
        button_attach((Button *)tmp[i], PRESS_DOWN,       KeyPressDownHandler);
        button_attach((Button *)tmp[i], PRESS_UP,         KeyPressUpHandler);
        button_attach((Button *)tmp[i], PRESS_REPEAT,     KeyPressRepeatHandler);
        button_attach((Button *)tmp[i], SINGLE_CLICK,     KeySingleClickHandler);
        button_attach((Button *)tmp[i], DOUBLE_CLICK,     KeyDoubleClickHandler);
        button_attach((Button *)tmp[i], LONG_PRESS_START, KeyLongPressStartHandler);
        button_attach((Button *)tmp[i], LONG_PRESS_HOLD,  KeyLongPressHoldHandler);
    }

    for (uint8_t i=0; i<KEY_MAX; i++) {
        button_start((Button *)tmp[i]);
    }

     TmgrStartTimer(OsApiGetTmrgrHnd(TIMER_TYPE_PLATFORM), TMR_OBJ_HANDLE(g_keyDetect), KEY_DETECT_START_TIME,
        KEY_DETECT_CYCLE_TIME);
}

void ButtonNavigation(BaseOp op, uint32_t event, const uint8_t *para)
{
    switch (op) {
        case OP_ENTER:
            KeyDetectInit();
            break;

        case OP_EVENT:
            /* LogOnEvent(event, (uintptr_t buf)); */
            /* LogOnEventFunc(event, len, buf, g_tab, tabSize); */
            break;

        case OP_EXIT:
            /* LogDeinit(); */
            break;

        default:
            break;
    }
}

TMR_CALLBACK_FUNC(KeyDetect)
{
    /* handle */
    button_ticks();
}
