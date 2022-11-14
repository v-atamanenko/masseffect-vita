/*
 * reimpl/sys.c
 *
 * Implementations and wrappers for system-related functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <sys/errno.h>
#include <psp2/kernel/threadmgr.h>

#include "sys.h"
#include "utils/utils.h"

int clock_gettime_soloader(__attribute__((unused)) int c, struct timespec *t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv)
        return rv;
    t->tv_sec = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}

int nanosleep_soloader (const struct timespec *rqtp,
        __attribute__((unused)) struct timespec *rmtp) {
    if (!rqtp) {
        errno = EFAULT;
        return -1;
    }

    if (rqtp->tv_sec < 0 || rqtp->tv_nsec < 0 || rqtp->tv_nsec > 999999999) {
        errno = EINVAL;
        return -1;
    }

    const uint32_t us = rqtp->tv_sec * 1000000 + (rqtp->tv_nsec+999) / 1000;

    sceKernelDelayThread(us);
    return 0;
}
