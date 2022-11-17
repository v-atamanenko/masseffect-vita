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
#include <pthread.h>
#include <malloc.h>
#include <string.h>

#include "FalsoJNI_Logger.h"
#include "FalsoJNI.h"

pthread_mutex_t * _fjni_log_mutex = NULL;

#define COLOR_RED     "\x1B[31m"
#define COLOR_ORANGE  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

#define COLOR_END     "\033[0m"


void _fjni_log_lock() {
    if (_fjni_log_mutex == NULL) {
        pthread_mutex_t initTmpNormal;
        _fjni_log_mutex = malloc(sizeof(pthread_mutex_t));
        memcpy(_fjni_log_mutex, &initTmpNormal, sizeof(pthread_mutex_t));

        if (pthread_mutex_init(_fjni_log_mutex, NULL) != 0) {
            _fjni_log_mutex = NULL;
            return;
        }
    }

    pthread_mutex_lock(_fjni_log_mutex);
}

void _fjni_log_unlock() {
    if (_fjni_log_mutex) {
        pthread_mutex_unlock(_fjni_log_mutex);
    }
}

int _fjni_log_info(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_INFO
    _fjni_log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[INFO] %s%s\n", COLOR_BLUE, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt_colored, list);
    va_end(list);
    _fjni_log_unlock();
#endif
    return 0;
}

int _fjni_log_warn(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_WARN
    _fjni_log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[WARNING][%s:%d][%s] %s%s\n", COLOR_ORANGE, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt_colored, list);
    va_end(list);
    _fjni_log_unlock();
#endif
    return 0;
}

int _fjni_log_debug(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_ALL
    _fjni_log_lock();
    char fmt_real[4096];
    snprintf(fmt_real, 2047, "[DEBUG][%s:%d][%s] %s\n", fname, lineno, fxname, fmt);

    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt_real, list);
    va_end(list);
    _fjni_log_unlock();
#endif
    return 0;
}

int _fjni_log_error(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#if FALSOJNI_DEBUGLEVEL <= FALSOJNI_DEBUG_ERROR
    _fjni_log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[ERROR][%s:%d][%s] %s%s\n", COLOR_RED, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt_colored, list);
    va_end(list);
    _fjni_log_unlock();
#endif
    return 0;
}
