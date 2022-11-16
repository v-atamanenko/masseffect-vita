/*
 * utils/logger.c
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
#include "logger.h"

#define COLOR_RED     "\x1B[31m"
#define COLOR_ORANGE  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"

#define COLOR_END     "\033[0m"

pthread_mutex_t * _log_mutex = NULL;

void _log_lock() {
    if (_log_mutex == NULL) {
        pthread_mutex_t initTmpNormal;
        _log_mutex = malloc(sizeof(pthread_mutex_t));
        memcpy(_log_mutex, &initTmpNormal, sizeof(pthread_mutex_t));

        if (pthread_mutex_init(_log_mutex, NULL) != 0) {
            _log_mutex = NULL;
            return;
        }
    }

    pthread_mutex_lock(_log_mutex);
}

void _log_unlock() {
    if (_log_mutex) {
        pthread_mutex_unlock(_log_mutex);
    }
}

int _log_info(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    _log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[INFO] %s%s\n", COLOR_BLUE, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
    _log_unlock();
#endif
    return 0;
}

int _log_warn(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    _log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[WARNING][%s:%d][%s] %s%s\n", COLOR_ORANGE, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
    _log_unlock();
#endif
    return 0;
}

int _log_debug(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
#ifdef DEBUG_SOLOADER
    _log_lock();
    char fmt_real[4096];
    snprintf(fmt_real, 2047, "[DEBUG][%s:%d][%s] %s\n", fname, lineno, fxname, fmt);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_real, list);
    va_end(list);
    _log_unlock();
#endif
    return 0;
}

int _log_error(const char *fname, int lineno, const char *fxname, const char* fmt, ...) {
    _log_lock();
    char fmt_colored[2048];
    snprintf(fmt_colored, 2047, "%s[ERROR][%s:%d][%s] %s%s\n", COLOR_RED, fname, lineno, fxname, fmt, COLOR_END);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt_colored, list);
    va_end(list);
    _log_unlock();
    return 0;
}
