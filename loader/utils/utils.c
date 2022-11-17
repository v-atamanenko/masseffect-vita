/*
 * utils/utils.c
 *
 * Common helper utilities.
 *
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils.h"
#include "dialog.h"
#include "default_dynlib.h"
#include "patch.h"
#include "glutil.h"
#include "main.h"
#include "logger.h"

#include <psp2/io/stat.h>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/dirent.h>
#include <dirent.h>
#include <psp2/apputil.h>
#include <psp2/power.h>
#include <so_util.h>
#include <FalsoJNI/FalsoJNI.h>
#include <sys/stat.h>
#include <psp2/kernel/clib.h>
#include <psp2/appmgr.h>
#include <sha1.h>
#include <malloc.h>

#pragma ide diagnostic ignored "bugprone-reserved-identifier"

// For check_kubridge
SceUID _vshKernelSearchModuleByName(const char *, int *);

int ret0(void) {
    return 0;
}

__attribute__((unused)) int ret1(void) {
    return 1;
}

int retminus1(void) {
    return -1;
}

int file_exists(const char *path) {
    SceIoStat stat;
    return sceIoGetstat(path, &stat) >= 0;
}

int check_kubridge(void) {
    int search_unk[2];
    return _vshKernelSearchModuleByName("kubridge", search_unk);
}

__attribute__((unused)) int string_ends_with(const char * str, const char * suffix)
{
    int str_len = (int)strlen(str);
    int suffix_len = (int)strlen(suffix);

    return
            (str_len >= suffix_len) &&
            (0 == strcmp(str + (str_len-suffix_len), suffix));
}

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate ms
    return milliseconds;
}

char* strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

char* strreplace(char *target, const char *needle, const char *replacement) {
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
    return target;
}

/* Prepends t into s. Assumes s has enough space allocated
** for the combined string.
*/
void strprepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

inline int8_t is_dir(char* p) {
    DIR* filetest = opendir(p);
    if (filetest != NULL) {
        closedir(filetest);
        return 1;
    }
    return 0;
}

char * get_file_sha1(const char* path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);

    uint8_t sha1[20];
    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, (uint8_t *)buf, size);
    sha1_final(&ctx, (uint8_t *)sha1);
    free(buf);

    char hash[42];
    memset(hash, 0, sizeof(hash));

    int i;
    for (i = 0; i < 20; i++) {
        char string[4];
        sprintf(string, "%02X", sha1[i]);
        strcat(hash, string);
    }

    hash[41] = '\0';
    return strdup(hash);
}

extern volatile int silentLoad;

int soloader_init_all() {
    // Check if we want to start the companion app
    sceAppUtilInit(&(SceAppUtilInitParam){}, &(SceAppUtilBootParam){});
    SceAppUtilAppEventParam eventParam;
    sceClibMemset(&eventParam, 0, sizeof(SceAppUtilAppEventParam));
    sceAppUtilReceiveAppEvent(&eventParam);
    if (eventParam.type == 0x05) {
        char buffer[2048];
        sceAppUtilAppEventParseLiveArea(&eventParam, buffer);
        if (strstr(buffer, "-config"))
            sceAppMgrLoadExec("app0:/companion.bin", NULL, NULL);
    }

    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);

    if (check_kubridge() < 0)
        fatal_error("Error kubridge.skprx is not installed.");
    log_info("check_kubridge() passed.\n");

    if (!file_exists(SO_PATH)) {
        fatal_error("Looks like you haven't installed the data files for this "
                    "port, or they are in an incorrect location. Please make "
                    "sure that you have %s file exactly at that path.", SO_PATH);
    }

    char p[512];
    snprintf(p, sizeof(p), "%spublished/data/level_index.prefabs.sb", DATA_PATH_INT);
    if (!file_exists(p)) {
        fatal_error("Looks like you haven't installed the data files for this "
                    "port, or they are in an incorrect location. Please make "
                    "sure that you put the \"published\" directory into "
                    "%s.", DATA_PATH_INT);
    }

    char* so_hash = get_file_sha1(SO_PATH);
    if (strcmp(so_hash, "EA58B733D3D267AB639431B50539542FAA43F0D0") != 0) {
        fatal_error("Looks like you installed a wrong version of the game that "
                    "doesn't work with this port. Please make sure that you're "
                    "using the Android release v1.0.58. Expected SHA1: "
                    "EA58B733D3D267AB639431B50539542FAA43F0D0, actual SHA1: "
                    "%s.", so_hash);
    }
    free(so_hash);

    if (so_file_load(&so_mod, SO_PATH, LOAD_ADDRESS) < 0)
        fatal_error("Error could not load %s.", SO_PATH);
    logv_info("so_file_load(%s) passed.\n", SO_PATH);

    so_relocate(&so_mod);
    log_info("so_relocate() passed.\n");

    resolve_imports(&so_mod);
    log_info("so_resolve() passed.\n");

    so_patch();
    log_info("so_patch() passed.\n");

    so_flush_caches(&so_mod);
    log_info("so_flush_caches() passed.\n");

    so_initialize(&so_mod);
    log_info("so_initialize() passed.\n");

    gl_preload();
    log_info("gl_preload() passed.\n");

    jni_init();
    log_info("jni_init() passed.\n");

    if (!is_dir("ux0:data/masseffect/assets/var")) {
        mkdir("ux0:data/masseffect/assets/var", 0700);
    }

    if (!is_dir("ux0:data/masseffect/assets/var1")) {
        mkdir("ux0:data/masseffect/assets/var1", 0700);
    }

    if (!is_dir("ux0:data/masseffect/assets/files")) {
        mkdir("ux0:data/masseffect/assets/files", 0700);
    }

    if (!is_dir("ux0:data/masseffect/assets/files/GameSkeleton")) {
        mkdir("ux0:data/masseffect/assets/files/GameSkeleton", 0700);
    }

    return 1;
}
