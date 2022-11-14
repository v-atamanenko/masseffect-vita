/*
 * FalsoJNI_Logger.h
 *
 * Fake Java Native Interface, providing JavaVM and JNIEnv objects.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

#ifndef FALSOJNI_LOGGER_H
#define FALSOJNI_LOGGER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define fjni_logv_info(fmt, ...)   _fjni_log_info(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define fjni_logv_warn(fmt, ...)   _fjni_log_warn(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define fjni_logv_dbg(fmt, ...)  _fjni_log_debug(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define fjni_logv_err(fmt, ...)  _fjni_log_error(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)

#define fjni_log_info(fmt)     _fjni_log_info(__FILE__, __LINE__, __func__, fmt)
#define fjni_log_warn(fmt)     _fjni_log_warn(__FILE__, __LINE__, __func__, fmt)
#define fjni_log_dbg(fmt)     _fjni_log_debug(__FILE__, __LINE__, __func__, fmt)
#define fjni_log_err(fmt)     _fjni_log_error(__FILE__, __LINE__, __func__, fmt)

int _fjni_log_info(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
int _fjni_log_warn(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
int _fjni_log_debug(const char *fname, int lineno, const char *fxname, const char* fmt, ...);
int _fjni_log_error(const char *fname, int lineno, const char *fxname, const char* fmt, ...);

#ifdef __cplusplus
};
#endif

#endif // FALSOJNI_LOGGER_H
