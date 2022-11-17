/*
 * utils/utils.h
 *
 * Common helper utilities.
 *
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_UTILS_H
#define SOLOADER_UTILS_H

#include <sys/types.h>
#include "config.h"

int ret0(void);

__attribute__((unused)) int ret1(void);

int retminus1(void);

int file_exists(const char *path);

char * get_file_sha1(const char* path);

int check_kubridge(void);

__attribute__((unused)) int string_ends_with(const char * str, const char * suffix);

void strprepend(char* s, const char* t);

__attribute__((unused)) inline int string_starts_with(const char *pre,
                                                      const char *str) {
    char cp;
    char cs;

    if (!*pre)
        return 1;

    while ((cp = *pre++) && (cs = *str++))
    {
        if (cp != cs)
            return 0;
    }

    if (!cs)
        return 0;

    return 1;
}

long long current_timestamp();

char *strremove(char *str, const char *sub);

char* strreplace(char *target, const char *needle, const char *replacement);

int8_t is_dir(char* p);

int soloader_init_all();

#endif // SOLOADER_UTILS_H
