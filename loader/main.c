/*
 * main.c
 *
 * ARMv7 Shared Libraries loader. Mass Effect Infiltrator edition.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2022 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "main.h"

#include <pthread.h>
#include <kubridge.h>
#include <psp2/sysmodule.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <string.h>

#include "FalsoJNI/FalsoJNI.h"
#include "android/EAAudioCore.h"
#include "reimpl/controls.h"
#include "utils/glutil.h"
#include "utils/logger.h"
#include "utils/dialog.h"
#include "utils/settings.h"

__attribute__((unused)) int _newlib_heap_size_user = MEMORY_NEWLIB_MB * 1024 * 1024;

so_module so_mod;

KuKernelExceptionHandler nextHandler;

void UndefInstrHandler(KuKernelExceptionContext *exceptionContext)
{
    printf("Caugt exception type 0x%x, ignoring.\n",exceptionContext->exceptionType);
    exceptionContext->pc += 4;
}

int wantQuit = 0;
volatile int savesBroke = 0;
volatile int silentLoad = 0;

int main(int argc, char*argv[]) {
    loadSettings();
    soloader_init_all();

    for (int i = 0; i < argc; ++i)
        if (strstr(argv[i], "-silent")) silentLoad = 1;


    // Running the .so in a thread with enlarged stack size.
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 256*1024);
    pthread_create(&t, &attr, game_thread, NULL);
    pthread_join(t, NULL);
}

void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init)(JNIEnv* env, jobject* obj, AudioTrack audioTrack, int i, int i2, int i3);
void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release)(JNIEnv* env);

extern int* waitForOrientation;



void* game_thread() {
    int (*JNI_OnLoad)(JavaVM* jvm) = (void*)so_symbol(&so_mod,"JNI_OnLoad");
    void (*NativeOnCreate)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_MainActivity_NativeOnCreate");
    void (*NativeOnSurfaceCreated)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_AndroidRenderer_NativeOnSurfaceCreated");
    void (*EASPHandler_initJNI)(JNIEnv* jniEnv, jobject * obj) = (void*)so_symbol(&so_mod,"Java_com_ea_easp_EASPHandler_initJNI");
    void (*NativeOnDrawFrame)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_AndroidRenderer_NativeOnDrawFrame");
    void (*initEAThread)(JNIEnv* env) = (void*)so_symbol(&so_mod,"Java_com_ea_EAThread_EAThread_Init");
    void (*initEAIO)(JNIEnv* env, void* unused, jobject * assets, jstring * FilesDirAbsolutePath, jstring* ExternalStorageAbsolutePath) = (void*)so_symbol(&so_mod,"Java_com_ea_EAIO_EAIO_StartupNativeImpl");
    void (*initEAMIO)(JNIEnv* env) = (void*)so_symbol(&so_mod,"Java_com_ea_EAMIO_StorageDirectory_StartupNativeImpl");

    Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init = (void*)so_symbol(&so_mod,"Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init");
    Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release = (void*)so_symbol(&so_mod,"Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release");

    gl_init();
    log_info("gl_init() passed");

    JNI_OnLoad(&jvm);
    log_info("JNI_OnLoad() passed");

    EASPHandler_initJNI(&jni, (jobject *) 0x42424242);
    log_info("EASPHandler_initJNI() passed");

    initEAThread(&jni);
    log_info("initEAThread() passed");

    initEAIO(&jni, (void *) 0x69696969, (jobject *) 0x42424242, (jstring *) DATA_PATH, (jstring *) DATA_PATH);
    log_info("initEAIO() passed");

    initEAMIO(&jni);
    log_info("initEAMIO() passed");

    NativeOnCreate();
    log_info("NativeOnCreate() passed");

    EAAudioCore__Startup();
    log_info("EAAudioCore__Startup() passed");

    controls_init();
    log_info("controls_init() passed.\n");

    NativeOnSurfaceCreated();
    log_info("NativeOnSurfaceCreated() passed.\n");

    if (silentLoad) runSilentStartHelper();
    while (1) {
        NativeOnDrawFrame();
        if (silentLoad) {
            drawFakeLoadingScreen();
        }
        gl_swap();
        controls_poll();

        if (*waitForOrientation == 1) { *waitForOrientation = 0; }
        if (wantQuit) break;
        if (savesBroke) {
            for (int i = 0; i < 3; ++i) {
                drawFakeLoadingScreen();
                gl_swap();
            }
            char argp[512];
            sprintf(argp, "-silent");
            int ret = sceAppMgrLoadExec("app0:/eboot.bin", (char * const*)((const char*[]){argp, 0}), NULL);
            fprintf("ret1 0x%x\n", ret);
            while(1) sceKernelDelayThread(10000);
        }
    }
}
