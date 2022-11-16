/*
 * android/java.io.InputStream.c
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/dirent.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <sys/unistd.h>
#include <FalsoJNI/FalsoJNI_ImplBridge.h>
#include "utils/utils.h"

#include "java.io.InputStream.h"
#include "utils/logger.h"

FILE* f = NULL;
int fd = -1;
pthread_mutex_t* inputStream_mutex = NULL;

size_t listSize = 0;
size_t listAllocedSize = 0;
char** list = NULL;
int calls_count = 0;

void inputStream_lock() {
    if (inputStream_mutex == NULL) {
        pthread_mutex_t initTmpNormal;
        inputStream_mutex = malloc(sizeof(pthread_mutex_t));
        memcpy(inputStream_mutex, &initTmpNormal, sizeof(pthread_mutex_t));

        if (pthread_mutex_init(inputStream_mutex, NULL) != 0) {
            log_error("Failed to allocate inputStream mutex");
            inputStream_mutex = NULL;
            return;
        }
    }

    pthread_mutex_lock(inputStream_mutex);
}

void inputStream_unlock() {
    pthread_mutex_unlock(inputStream_mutex);
}

// public int read(byte[] b, int off, int len)
// https://docs.oracle.com/javase/7/docs/api/java/io/InputStream.html#read()
jint InputStream_read(jmethodID id, va_list args) {
    //logv_debug("JNI: Method Call: InputStream_read() / id: %i", id);
    // Imporant note: there are also versions of read() with two and one arg;
    // here we assume that only the full version with 3 args is used, which is
    // dangerous.

    void* _b = va_arg(args, char*);
    int off = va_arg(args, int);
    int len = va_arg(args, int);

    JavaDynArray * arr = jda_find(_b);
    if (!arr) {
        log_error("[java.io.InputStream.read()] Provided buffer is not a valid JDA.");
        return 0;
    }

    char * b = arr->array;

    if (!f) {
        if (fd == -1) {
            log_error("[java.io.InputStream.read()] File descriptor is NULL.");
            return -1;
        }
    }

    if (len <= 0) {
        return 0;
    }

    size_t bytes_read;
    if (!f) {
        bytes_read = read(fd, b+off, len);
    } else {
        bytes_read = fread(b+off, 1, len, f);
    }

    if (bytes_read <= 0) {
        return -1;
    }

    return (int)bytes_read;
}

// public void close ()
// https://developer.android.com/reference/android/content/res/AssetManager#close()
void InputStream_close(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: InputStream_close() / id: %i", id);

    if (f) {
        fclose(f);
        f = NULL;
    }

    if (fd > -1) {
        close(fd);
        fd = -1;
    }

    inputStream_unlock();
}

// public long skip(long n)
// https://docs.oracle.com/javase/7/docs/api/java/io/InputStream.html#skip(long)
jlong InputStream_skip(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: InputStream_skip() / id: %i", id);
    int64_t off = va_arg(args, int64_t);

    if (!f) {
        log_error("[java.io.InputStream.skip()] File descriptor is NULL.");
        return -1;
    }

    if (fseek(f, off, SEEK_CUR) < 0 ) {
        return -1;
    }

    // NOTE: According to Java docs, this function must return the actual
    // number of bytes skipped. For efficiency, let's not count that.
    return 1;
}

// public InputStream open (String fileName)
// https://developer.android.com/reference/android/content/res/AssetManager#open(java.lang.String)
jobject InputStream_open(jmethodID id, va_list args) {
    // This method is supposed to initialize the asset to be opened, but
    // since we don't need that, let's just return a dummy string that can
    // be freed later.
    logv_debug("JNI: Method Call: InputStream_open() / id: %i", id);
    inputStream_lock();
    return strdup("nop");
}

// public AssetFileDescriptor openFd (String fileName)
// https://developer.android.com/reference/android/content/res/AssetManager#openFd(java.lang.String)
jint InputStream_openFd(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: InputStream_openFd() / id: %i", id);
    const char* fileName = va_arg(args, const char*);

    char temp[1024];
    if (strstr(fileName, "splash.png")) {
        sprintf(temp, "app0:/data/splash.png");
    } else {
        sprintf(temp, DATA_PATH_INT"%s", fileName);
    }

    logv_debug("[java.io.InputStream] InputStream_openFd(\"%s\")", temp);
    fd = open(temp, O_RDONLY);
    return fd;
}

// public String[] list (String path)
// https://developer.android.com/reference/android/content/res/AssetManager#list(java.lang.String)
jobject InputStream_list(jmethodID id, va_list args) {
    const char* path_tmp = va_arg(args, const char*);

    logv_debug("JNI: Method Call: InputStream_list() / id: %i / path: \"%s\" (0x%x)", id, path_tmp, path_tmp);

    char* path = malloc(512 * sizeof(char));
    snprintf(path, 512, DATA_PATH_INT"%s", path_tmp);

    //pthread_mutex_lock(&mut);

    char** paths_stack = malloc(sizeof(char*) * 1024);

    int i = 0;
    paths_stack[i++] = path;

    int first_run = 1;

    while (i > 0) {
        path = paths_stack[--i];

        DIR *dir;
        struct dirent *entry;

        calls_count++;

        if (!(dir = opendir(path))) {
            free(path);
            continue;
        }

        while ((entry = readdir(dir)) != NULL) {
            char* p = malloc(1000 * sizeof(char));

            if (first_run) {
                snprintf(p, 999, "%s%s", path, entry->d_name);
            } else {
                snprintf(p, 999, "%s/%s", path, entry->d_name);
            }

            if (!is_dir(p)) {
                if (listSize == 0) {
                    list = malloc(sizeof(char*) * 1734);
                    listAllocedSize = 1733;
                } else {
                    if (listSize >= listAllocedSize) {
                        list = realloc(list, (sizeof(char*) * (listSize+100)));
                        listAllocedSize = listSize + 99;
                    }
                }
                list[listSize] = strdup(p);
                listSize++;
                free(p); // crash here
            } else {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                paths_stack[i++] = p;
            }
        }
        closedir(dir);
        free(path);

        if (first_run) {
            first_run = 0;
        }
    }

    JavaDynArray * jda = jda_alloc((jsize)listSize, FIELD_TYPE_OBJECT);
    if (!jda) {
        log_error("Could not allocate array");
        for (int u = 0; u < listSize; ++u) free(list[u]);
        free(list);
        return NULL;
    }

    char** list_ret = jda->array;

    for (int u = 0; u < listSize; ++u) {
        list_ret[u] = strdup(list[u]);
        free(list[u]);
    }

    free(list);
    listSize = 0;

    //pthread_mutex_unlock(&mut);
    return (jobject)list_ret;
}

// public long getLength ()
// https://developer.android.com/reference/android/content/res/AssetFileDescriptor#getLength()
jlong InputStream_getLength(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: InputStream_getLength() / id: %i / fd: %i", id, fd);

    if (fd == -1) {
        log_error("[java.io.InputStream.getLength()] Error: fd is invalid");
        return -1;
    }

    size_t sz = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);

    return (jlong)sz;
}
