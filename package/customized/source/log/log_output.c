/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2023-01-15
 */

#include "log_output.h"
#include "sys_api.h"
#include "elog.h"

/* TMR_CALLBACK_FUNC(LogOutput); */
/* TMR_CONSTRUCT(g_logOutput, TMR_CALLBACK_FUNC_REF(LogOutput)); */

/* LogOnEvent(uint32_t event, uintptr_t *para); */
/* LogOnEventFunc(uint32_t event, uint16_t len, const uint8_t *para, g_tab, tabSize); */

static void LogInit(void)
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_start();
}

static void LogDeinit(void)
{

}

void LogNavigation(BaseOp op, uint32_t event, uint16_t len, const uint8_t *para)
{
    switch (op) {
        case OP_ENTER:
            LogInit();
            break;

        case OP_EVENT:
            /* LogOnEvent(event, (uintptr_t buf)); */
            /* LogOnEventFunc(event, len, buf, g_tab, tabSize); */
            break;

        case OP_EXIT:
            LogDeinit();
            break;

        default:
            break;
   }
}
