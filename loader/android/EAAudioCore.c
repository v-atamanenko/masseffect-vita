/*
 * android/EAAudioCore.c
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdio.h>
#include <psp2/audioout.h>
#include <FalsoJNI/FalsoJNI_ImplBridge.h>
#include "EAAudioCore.h"
#include "utils/logger.h"

extern void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init)(JNIEnv* env, jobject* obj, void * audioTrack, int i, int i2, int i3);
extern void (*Java_com_ea_EAAudioCore_AndroidEAAudioCore_Release)(JNIEnv* env);
extern volatile int silentLoad;

int audio_port = -1;

int EAAudioCore_AudioTrack_write(int id, va_list args) {
    void* _buf = va_arg(args, void*);
    int32_t offs = va_arg(args, int32_t);
    int32_t len = va_arg(args, int32_t);

    JavaDynArray * jda = jda_find(_buf);
    if (!jda) {
        log_error("Provided buffer is not a valid JDA.");
        return 0;
    }

    jshort * buf = jda->array;

    if (audio_port == -1) {
        audio_port = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, 512, 44100, SCE_AUDIO_OUT_MODE_STEREO);
        logv_info("got audio_port %i\n", audio_port);
    }

    if (!silentLoad) sceAudioOutOutput(audio_port, buf);

    return len;
}

void EAAudioCore_AudioTrack_play(int id, va_list args) {
    log_info("AudioTrack_play");
}

void EAAudioCore_AudioTrack_stop(int id, va_list args) {
    log_info("AudioTrack_stop");
}

void EAAudioCore__Startup() {
    Java_com_ea_EAAudioCore_AndroidEAAudioCore_Init(&jni, (void*)0x42424242, (void*)0x69696969, 512*1024, 2, 44100);
}
