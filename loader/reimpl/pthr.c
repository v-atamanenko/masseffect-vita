/*
 * pthr.c
 *
 * Wrapper for vitasdk/newlib pthread functions to work with
 * Android's pthread struct which is different
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 * Copyright (C) 2022 GrapheneCt
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "pthr.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <psp2/kernel/clib.h>
#include <string.h>
#include <psp2/kernel/threadmgr.h>

#define  MUTEX_TYPE_NORMAL     0x0000
#define  MUTEX_TYPE_RECURSIVE  0x4000
#define  MUTEX_TYPE_ERRORCHECK 0x8000

static pthread_t s_pthreadSelfRet;

static void init_static_mutex(pthread_mutex_t **mutex)
{
    pthread_mutex_t *mtxMem = NULL;

    switch ((int)*mutex) {
        case MUTEX_TYPE_NORMAL: {
            pthread_mutex_t initTmpNormal = PTHREAD_MUTEX_INITIALIZER;
            mtxMem = calloc(1, sizeof(pthread_mutex_t));
            sceClibMemcpy(mtxMem, &initTmpNormal, sizeof(pthread_mutex_t));
            *mutex = mtxMem;
            break;
        }
        case MUTEX_TYPE_RECURSIVE: {
            pthread_mutex_t initTmpRec = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
            mtxMem = calloc(1, sizeof(pthread_mutex_t));
            sceClibMemcpy(mtxMem, &initTmpRec, sizeof(pthread_mutex_t));
            *mutex = mtxMem;
            break;
        }
        case MUTEX_TYPE_ERRORCHECK: {
            pthread_mutex_t initTmpErr = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
            mtxMem = calloc(1, sizeof(pthread_mutex_t));
            sceClibMemcpy(mtxMem, &initTmpErr, sizeof(pthread_mutex_t));
            *mutex = mtxMem;
            break;
        }
        default:
            break;
    }
}

static void init_static_cond(pthread_cond_t **cond)
{
    if (*cond == NULL) {
        pthread_cond_t initTmp = PTHREAD_COND_INITIALIZER;
        pthread_cond_t *condMem = calloc(1, sizeof(pthread_cond_t));
        sceClibMemcpy(condMem, &initTmp, sizeof(pthread_cond_t));
        *cond = condMem;
    }
}

static void init_static_sem(sem_t **sema)
{
    if (*sema == NULL) {
        sem_t initTmp;
        sem_t *semaMem = calloc(1, sizeof(sem_t));
        sceClibMemcpy(semaMem, &initTmp, sizeof(sem_t));
        *sema = semaMem;
    }
}

int pthread_attr_destroy_soloader(pthread_attr_t **attr)
{
    int ret = pthread_attr_destroy(*attr);
    free(*attr);
    return ret;
}

int pthread_attr_getstack_soloader(const pthread_attr_t **attr,
                                   void **stackaddr,
                                   size_t *stacksize)
{
    return pthread_attr_getstack(*attr, stackaddr, stacksize);
}

__attribute__((unused)) int pthread_condattr_init_soloader(pthread_condattr_t **attr)
{
    *attr = calloc(1, sizeof(pthread_condattr_t));

    return pthread_condattr_init(*attr);
}

__attribute__((unused)) int pthread_condattr_destroy_soloader(pthread_condattr_t **attr)
{
    int ret = pthread_condattr_destroy(*attr);
    free(*attr);
    return ret;
}

int pthread_cond_init_soloader(pthread_cond_t **cond,
                               const pthread_condattr_t **attr)
{
    *cond = calloc(1, sizeof(pthread_cond_t));

    if (attr != NULL)
        return pthread_cond_init(*cond, *attr);
    else
        return pthread_cond_init(*cond, NULL);
}

int pthread_cond_destroy_soloader(pthread_cond_t **cond)
{
    int ret = pthread_cond_destroy(*cond);
    free(*cond);
    return ret;
}

int pthread_cond_signal_soloader(pthread_cond_t **cond)
{
    init_static_cond(cond);
    return pthread_cond_signal(*cond);
}

int pthread_cond_timedwait_soloader(pthread_cond_t **cond,
                                    pthread_mutex_t **mutex,
                                    struct timespec *abstime)
{
    init_static_cond(cond);
    init_static_mutex(mutex);
    return pthread_cond_timedwait(*cond, *mutex, abstime);
}

int pthread_create_soloader(pthread_t **thread,
                            const pthread_attr_t **attr,
                            void *(*start)(void *),
                            void *param)
{
    *thread = calloc(1, sizeof(pthread_t));

    printf("pthread_create start 0x%x\n", (int)start);

    if (attr != NULL) {
        pthread_attr_setstacksize(*attr, 64 * 1024);
        return pthread_create(*thread, *attr, start, param);
    } else {
        pthread_attr_t attrr;
        pthread_attr_init(&attrr);
        pthread_attr_setstacksize(&attrr, 64 * 1024);
        return pthread_create(*thread, &attrr, start, param);
    }

}

int pthread_mutexattr_init_soloader(pthread_mutexattr_t **attr)
{
    *attr = calloc(1, sizeof(pthread_mutexattr_t));

    return pthread_mutexattr_init(*attr);
}

int pthread_mutexattr_settype_soloader(pthread_mutexattr_t **attr, int type)
{
    return pthread_mutexattr_settype(*attr, type);
}

int pthread_mutexattr_setpshared_soloader(pthread_mutexattr_t **attr, int pshared)
{
    return pthread_mutexattr_setpshared(*attr, pshared);
}

int pthread_mutexattr_destroy_soloader(pthread_mutexattr_t **attr)
{
    int ret = pthread_mutexattr_destroy(*attr);
    free(*attr);
    return ret;
}

int pthread_mutex_destroy_soloader(pthread_mutex_t **mutex)
{
    int ret = pthread_mutex_destroy(*mutex);
    free(*mutex);
    return ret;
}

int pthread_mutex_init_soloader(pthread_mutex_t **mutex,
                                const pthread_mutexattr_t **attr)
{
    *mutex = calloc(1, sizeof(pthread_mutex_t));

    if (attr != NULL)
        return pthread_mutex_init(*mutex, *attr);
    else
        return pthread_mutex_init(*mutex, NULL);
}

int pthread_mutex_lock_soloader(pthread_mutex_t **mutex)
{
    init_static_mutex(mutex);
    return pthread_mutex_lock(*mutex);
}

int pthread_mutex_trylock_soloader(pthread_mutex_t **mutex)
{
    init_static_mutex(mutex);
    return pthread_mutex_trylock(*mutex);
}

int pthread_mutex_unlock_soloader(pthread_mutex_t **mutex)
{
    return pthread_mutex_unlock(*mutex);
}

int pthread_join_soloader(const pthread_t *thread, void **value_ptr)
{
    return pthread_join(*thread, value_ptr);
}

int pthread_cond_wait_soloader(pthread_cond_t **cond, pthread_mutex_t **mutex)
{
    return pthread_cond_wait(*cond, *mutex);
}

int pthread_cond_broadcast_soloader(pthread_cond_t **cond)
{
    return pthread_cond_broadcast(*cond);
}

int pthread_attr_init_soloader(pthread_attr_t **attr)
{
    *attr = calloc(1, sizeof(pthread_attr_t));

    return pthread_attr_init(*attr);
}

int pthread_attr_setdetachstate_soloader(pthread_attr_t **attr, int state)
{
    return pthread_attr_setdetachstate(*attr, state);
}

int pthread_attr_setstacksize_soloader(pthread_attr_t **attr, size_t stacksize)
{
    return pthread_attr_setstacksize(*attr, stacksize);
}

int pthread_attr_setschedparam_soloader(pthread_attr_t **attr,
                                        const struct sched_param *param)
{
    return pthread_attr_setschedparam(*attr, param);
}

int pthread_attr_setstack_soloader(pthread_attr_t **attr,
                                   void *stackaddr,
                                   size_t stacksize)
{
    return pthread_attr_setstack(*attr, stackaddr, stacksize);
}

int pthread_setschedparam_soloader(const pthread_t *thread, int policy,
                                   const struct sched_param *param)
{
    return pthread_setschedparam(*thread, policy, param);
}

int pthread_getschedparam_soloader(const pthread_t *thread, int *policy,
                                   struct sched_param *param)
{
    return pthread_getschedparam(*thread, policy, param);
}

int pthread_detach_soloader(const pthread_t *thread)
{
    return pthread_detach(*thread);
}

int pthread_getattr_np_soloader(pthread_t* thread, pthread_attr_t *attr) {
    fprintf(stderr, "[WARNING!] Not implemented: pthread_getattr_np\n");
    return 0;
}

int pthread_equal_soloader(const pthread_t *t1, const pthread_t *t2)
{
    return pthread_equal(*t1, *t2);
}

pthread_t *pthread_self_soloader()
{
    s_pthreadSelfRet = pthread_self();
    return &s_pthreadSelfRet;
}

#ifndef MAX_TASK_COMM_LEN
#define MAX_TASK_COMM_LEN 16
#endif

int pthread_setname_np_soloader(const pthread_t *thread, const char* thread_name) {
    if (thread == 0 || thread_name == NULL) {
        return EINVAL;
    }
    size_t thread_name_len = strlen(thread_name);
    if (thread_name_len >= MAX_TASK_COMM_LEN) {
        return ERANGE;
    }

    // TODO: Implement the actual name setting if possible
    fprintf(stderr, "PTHR: pthread_setname_np with name %s\n", thread_name);

    return 0;
}

int sem_destroy_soloader(int * uid) {
    if (sceKernelDeleteSema(*uid) < 0)
        return -1;
    return 0;
}

int sem_getvalue_soloader (int * uid, int * sval) {
    SceKernelSemaInfo info;
    info.size = sizeof(SceKernelSemaInfo);

    if (!sceKernelGetSemaInfo(*uid, &info)) return -1;
    if (!sval) sval = malloc(sizeof(int32_t));
    *sval = info.currentCount;
    return 0;
}

int sem_init_soloader (int * uid, int pshared, unsigned int value) {
    *uid = sceKernelCreateSema("sema", 0, value, 0x7fffffff, NULL);
    if (*uid < 0)
        return -1;
    return 0;
}

int sem_post_soloader (int * uid) {
    if (sceKernelSignalSema(*uid, 1) < 0)
        return -1;
    return 0;
}

int sem_timedwait_soloader (int * uid, const struct timespec * abstime) {
    uint timeout = 1000;
    if (sceKernelWaitSema(*uid, 1, &timeout) >= 0)
        return 0;
    if (!abstime) return -1;
    long long now = current_timestamp() * 1000; // us
    long long _timeout = abstime->tv_sec * 1000 * 1000 + abstime->tv_nsec / 1000; // us
    if (_timeout-now >= 0) return -1;
    uint timeout_real = _timeout - now;
    if (sceKernelWaitSema(*uid, 1, &timeout_real) < 0)
        return -1;
    return 0;
}

int sem_trywait_soloader (int * uid) {
    uint timeout = 1000;
    if (sceKernelWaitSema(*uid, 1, &timeout) < 0)
        return -1;
    return 0;
}

int sem_wait_soloader (int * uid) {
    if (sceKernelWaitSema(*uid, 1, NULL) < 0)
        return -1;
    return 0;
}
