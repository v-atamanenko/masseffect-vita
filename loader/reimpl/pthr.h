/*
 * pthr.h
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

#ifndef SOLOADER_PTHR_H
#define SOLOADER_PTHR_H

#include <pthread.h>
#include <semaphore.h>

int pthread_attr_destroy_soloader(pthread_attr_t **attr);

__attribute__((unused)) int pthread_condattr_init_soloader(
        pthread_condattr_t **attr);

__attribute__((unused)) int pthread_condattr_destroy_soloader(
        pthread_condattr_t **attr);

int pthread_cond_init_soloader(pthread_cond_t **cond,
                               const pthread_condattr_t **attr);

int pthread_cond_destroy_soloader(pthread_cond_t **cond);

int pthread_cond_signal_soloader(pthread_cond_t **cond);

int pthread_cond_timedwait_soloader(pthread_cond_t **cond,
                                    pthread_mutex_t **mutex,
                                    struct timespec *abstime);

int pthread_create_soloader(pthread_t **thread,
                            const pthread_attr_t **attr,
                            void *(*start)(void *),
                            void *param);

int pthread_mutexattr_init_soloader(pthread_mutexattr_t **attr);

int pthread_mutexattr_settype_soloader(pthread_mutexattr_t **attr, int type);

int pthread_mutexattr_destroy_soloader(pthread_mutexattr_t **attr);

int pthread_mutexattr_setpshared_soloader(pthread_mutexattr_t **attr, int pshared);

int pthread_mutex_destroy_soloader(pthread_mutex_t **mutex);

int pthread_mutex_init_soloader(pthread_mutex_t **mutex,
                                const pthread_mutexattr_t **attr);

int pthread_mutex_lock_soloader(pthread_mutex_t **mutex);

int pthread_mutex_trylock_soloader(pthread_mutex_t **mutex);

int pthread_mutex_unlock_soloader(pthread_mutex_t **mutex);

int pthread_join_soloader(const pthread_t *thread, void **value_ptr);

int pthread_cond_wait_soloader(pthread_cond_t **cond, pthread_mutex_t **mutex);

int pthread_cond_broadcast_soloader(pthread_cond_t **cond);

int pthread_attr_getstack_soloader(const pthread_attr_t **attr,
                                   void **stackaddr,
                                   size_t *stacksize);

int pthread_getattr_np_soloader(pthread_t *thread, pthread_attr_t *attr);

int pthread_attr_init_soloader(pthread_attr_t **attr);

int pthread_attr_setdetachstate_soloader(pthread_attr_t **attr, int state);

int pthread_attr_setstacksize_soloader(pthread_attr_t **attr, size_t stacksize);

int pthread_setschedparam_soloader(const pthread_t *thread, int policy,
                                   const struct sched_param *param);

int pthread_getschedparam_soloader(const pthread_t *thread, int *policy,
                                   struct sched_param *param);

int pthread_detach_soloader(const pthread_t *thread);

int pthread_equal_soloader(const pthread_t *t1, const pthread_t *t2);

pthread_t *pthread_self_soloader();

int pthread_setname_np_soloader(const pthread_t *thread, const char* thread_name);

int pthread_attr_setstack_soloader(pthread_attr_t **attr,
                                   void *stackaddr,
                                   size_t stacksize);

int pthread_attr_setschedparam_soloader(pthread_attr_t **attr,
                                        const struct sched_param *param);

int sem_destroy_soloader(int * sem);

int sem_getvalue_soloader (int * sem, int * sval);

int sem_init_soloader (int * sem, int pshared, unsigned int value);

int sem_post_soloader (int * sem);

int sem_timedwait_soloader (int * sem, const struct timespec * abstime);

int sem_trywait_soloader (int * sem);

int sem_wait_soloader (int * sem);

#endif // SOLOADER_PTHR_H
