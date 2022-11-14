/*
 * FalsoJNI_Logger.c
 *
 * Fake Java Native Interface, providing JavaVM and JNIEnv objects.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdarg.h>
#include "FalsoJNI_Logger.h"
#include "FalsoJNI.h"

#define COLOR_RED     "\x1B[31m"
#define COLOR_ORANGE  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

#define COLOR_END     "\033[0m"

int _fjni_log_info(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_INFO
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[INFO] %s%s\n", COLOR_BLUE, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
#endif
    return 0;
}

int _fjni_log_warn(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_WARN
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[WARNING][%s:%d][%s] %s%s\n", COLOR_ORANGE, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
#endif
    return 0;
}

int _fjni_log_debug(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_ALL
    char fmt_real[4096];
    snprintf(fmt_real, 2047, "[DEBUG][%s:%d][%s] %s\n", fname, lineno, fxname, fmt);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_real, list);
    va_end(list);
#endif
    return 0;
}

int _fjni_log_error(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_ERROR
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[ERROR][%s:%d][%s] %s%s\n", COLOR_RED, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
#endif
    return 0;
}
