/*
 * main.c
 *
 * ARMv7 Shared Libraries loader. Dead Space edition.
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
#include <string.h>

#include <psp2/apputil.h>
#include <psp2/power.h>
#include <sys/unistd.h>

#include "default_dynlib.h"
#include "utils/glutil.h"
#include "FalsoJNI/FalsoJNI.h"
#include "patch.h"
#include "utils/dialog.h"

#include "android/EAAudioCore.h"
#include "reimpl/controls.h"
#include "VFPVector/vfp_vector.h"

// Disable IDE complaints about _identifiers and unused variables
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

int _newlib_heap_size_user = MEMORY_NEWLIB_MB * 1024 * 1024;

so_module so_mod;

int main() {
    SceAppUtilInitParam init_param;
    SceAppUtilBootParam boot_param;
    memset(&init_param, 0, sizeof(SceAppUtilInitParam));
    memset(&boot_param, 0, sizeof(SceAppUtilBootParam));
    sceAppUtilInit(&init_param, &boot_param);

    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);

    if (check_kubridge() < 0)
        fatal_error("Error kubridge.skprx is not installed.");
    debugPrintf("check_kubridge() passed.\n");

    if (so_file_load(&so_mod, SO_PATH, LOAD_ADDRESS) < 0)
        fatal_error("Error could not load %s.", SO_PATH);
    debugPrintf("so_file_load(%s) passed.\n", SO_PATH);

    so_relocate(&so_mod);
    debugPrintf("so_relocate() passed.\n");

    resolve_imports(&so_mod);
    debugPrintf("so_resolve() passed.\n");

    so_patch();
    debugPrintf("so_patch() passed.\n");

    so_flush_caches(&so_mod);
    debugPrintf("so_flush_caches() passed.\n");

    so_initialize(&so_mod);
    debugPrintf("so_initialize() passed.\n");

    gl_preload();
    debugPrintf("gl_preload() passed.\n");

    jni_init();
    debugPrintf("jni_init() passed.\n");

    // Running the .so in a thread with enlarged stack size.
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1*1024*1024);
    pthread_create(&t, &attr, deadspace_main, NULL);
    pthread_join(t, NULL);
}

void (*Java_com_ea_EAIO_EAIO_Startup)(JNIEnv*, void*, jobject);

void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init)(JNIEnv* env, jobject* obj, AudioTrack audioTrack, int i, int i2, int i3);
void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release)(JNIEnv* env);

so_module *so_find_module_by_addr(uintptr_t addr) {
    return &so_mod;
}

extern int* _ZN2EA12GameSkeleton18waitForOrientationE;
extern int * _ZL15creatingCounter;
_Noreturn void *deadspace_main() {
    Java_com_ea_EAIO_EAIO_Startup = (void*)so_symbol(&so_mod,"Java_com_ea_EAIO_EAIO_Startup");
    int (*JNI_OnLoad)(JavaVM* jvm) = (void*)so_symbol(&so_mod,"JNI_OnLoad");
    void (*NativeOnCreate)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_MainActivity_NativeOnCreate");
    void (*NativeOnSurfaceCreated)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_AndroidRenderer_NativeOnSurfaceCreated");
    void (*Java_com_ea_easp_EASPHandler_initJNI)(JNIEnv* jniEnv, jobject obj) = (void*)so_symbol(&so_mod,"Java_com_ea_easp_EASPHandler_initJNI");
    void (*NativeOnDeviceOrientationChange)(JNIEnv* jniEnv, jobject obj, int orientation) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_DeviceOrientationHandlerAndroidDelegate_NativeOnDeviceOrientationChange");
    void (*NativeOnDrawFrame)(void) = (void*)so_symbol(&so_mod,"Java_com_ea_blast_AndroidRenderer_NativeOnDrawFrame");

    Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init = (void*)so_symbol(&so_mod,"Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init");
    Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release = (void*)so_symbol(&so_mod,"Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release");


    gl_init();
    debugPrintf("gl_init() passed.\n");

    JNI_OnLoad(&jvm);
    debugPrintf("JNI_OnLoad() passed.\n");



    void (*initEAThread)(JNIEnv* env) = (void*)so_symbol(&so_mod,"Java_com_ea_EAThread_EAThread_Init");
    void (*initEAIO)(JNIEnv* env, void* unused, jobject * assets, jstring * FilesDirAbsolutePath, jstring* ExternalStorageAbsolutePath) = (void*)so_symbol(&so_mod,"Java_com_ea_EAIO_EAIO_StartupNativeImpl");
    void (*initEAMIO)(JNIEnv* env) = (void*)so_symbol(&so_mod,"Java_com_ea_EAMIO_StorageDirectory_StartupNativeImpl");

    initEAThread(&jni);
    debugPrintf("initEAThread() passed.\n");
    initEAIO(&jni, 0x69696969, 0x42424242, "ux0:data/masseffect/", "ux0:data/masseffect/");
    debugPrintf("initEAIO() passed.\n");
    initEAMIO(&jni);
    debugPrintf("initEAMIO() passed.\n");

    Java_com_ea_easp_EASPHandler_initJNI(&jni, 0x42424242);
    debugPrintf("Java_com_ea_easp_EASPHandler_initJNI() passed.\n");

    EAAudioCore__Startup();
    debugPrintf("EAAudioCore__Startup() passed.\n");

    NativeOnCreate();
    debugPrintf("Java_com_ea_blast_MainActivity_NativeOnCreate() passed.\n");

    controls_init();


    NativeOnSurfaceCreated();
    debugPrintf("Java_com_ea_blast_AndroidRenderer_NativeOnSurfaceCreated() passed.\n");

    NativeOnDeviceOrientationChange(&jni, (jobject)0x42424242, 6);

    // 2 4 3 1 8 16

    //NativeOnVisibilityChanged(&jni, (void*)0x42424242, 600, 1);
    //debugPrintf("Java_com_ea_blast_KeyboardAndroid_NativeOnVisibilityChanged() passed.\n");

    while (1) {
        NativeOnDrawFrame();
        *_ZN2EA12GameSkeleton18waitForOrientationE = 0;
        gl_swap();
        controls_poll();
    }
}
