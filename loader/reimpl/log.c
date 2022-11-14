/*
 * reimpl/log.c
 *
 * Implementations for different Android logging functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "log.h"

#include <stdio.h>
#include "utils/logger.h"

int android_log_write(int prio, const char *tag, const char *msg) {
    logv_debug("[LOG][%i] %s: %s", prio, tag, msg);
    return 0;
}

int android_log_print(int prio, const char *tag, const char *fmt, ...) {
    va_list list;
    static char string[0x8000];

    va_start(list, fmt);
    vsprintf(string, fmt, list);
    va_end(list);

    logv_debug("[LOG][%i] %s: %s", prio, tag, string);
    return 0;
}

int android_log_vprint(int pri, const char *tag, const char *fmt, va_list lst) {
    static char string[0x8000];

    vsprintf(string, fmt, lst);
    va_end(lst);

    logv_debug("[LOGV][%i] %s: %s", pri, tag, string);
    return 0;
}
