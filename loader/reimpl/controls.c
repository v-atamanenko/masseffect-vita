/*
 * reimpl/controls.c
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "main.h"
#include "controls.h"

#include <FalsoJNI/FalsoJNI.h>
#include <psp2/kernel/threadmgr.h>
#include <pthread.h>
#include "utils/settings.h"

#define L_OUTER_DEADZONE 0.96f
#define R_OUTER_DEADZONE 0.98f

int lastX[SCE_TOUCH_MAX_REPORT] = {-1, -1, -1, -1, -1, -1, -1, -1};
int lastY[SCE_TOUCH_MAX_REPORT] = {-1, -1, -1, -1, -1, -1, -1, -1};

void (*NativeOnPointerEvent)(JNIEnv *env, jobject obj, int rawEvent, int moduleId, int eventPointerId, float eventX, float eventY);
void (*NativeOnKeyDown)(JNIEnv *env, jobject obj, int moduleId, int androidKey, int altPressed);
void (*NativeOnKeyUp)(JNIEnv *env, jobject obj, int moduleId, int androidKey, int altPressed);
void (*NativeOnAcceleration)(JNIEnv *env, jobject obj, float f1, float f2, float f3);
void (*NativeDispatchGenericMotionEvent)(JNIEnv *env, jobject obj, float left_x, float left_y, float right_x, float right_y, int action);

void controls_init() {
    // Enable analog sticks and touchscreen
    sceCtrlSetSamplingModeExt(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);

    NativeOnPointerEvent = (void*)so_symbol(&so_mod,"Java_com_ea_blast_TouchSurfaceAndroid_NativeOnPointerEvent");
    NativeOnKeyDown = (void*)so_symbol(&so_mod,"Java_com_ea_blast_KeyboardAndroid_NativeOnKeyDown");
    NativeOnKeyUp = (void*)so_symbol(&so_mod,"Java_com_ea_blast_KeyboardAndroid_NativeOnKeyUp");
    NativeOnAcceleration = (void*)so_symbol(&so_mod,"Java_com_ea_blast_AccelerometerAndroidDelegate_NativeOnAcceleration");
    NativeDispatchGenericMotionEvent = (void*)so_symbol(&so_mod,"Java_com_ea_games_meinfiltrator_1gamepad_MassEffectActivity_NativeDispatchGenericMotionEvent");

    sceMotionStartSampling();
}

void controls_poll() {
    pollTouch();
    pollPad();
    //pollAccel();
}

void pollTouch() {
    SceTouchData touch;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
    for (int i = 0; i < SCE_TOUCH_MAX_REPORT; i++) {
        if (i < touch.reportNum) {
            int x, y;
            x = (int)((float)touch.report[i].x * 960.f / 1920.0f);
            y = (int)((float)touch.report[i].y * 544.f / 1088.0f);

            if (lastX[i] == -1 || lastY[i] == -1) {
                NativeOnPointerEvent(&jni, (void*)0x42424242, kIdRawPointerDown, kModuleTypeIdTouchScreen, i, (float)x, (float)y);
            }
            else if (lastX[i] != x || lastY[i] != y) {
                NativeOnPointerEvent(&jni, (void*)0x42424242, kIdRawPointerMove, kModuleTypeIdTouchScreen, i, (float)x, (float)y);
            }

            lastX[i] = x;
            lastY[i] = y;
        } else {
            if (lastX[i] != -1 || lastY[i] != -1) {
                NativeOnPointerEvent(&jni, (void*)0x42424242, kIdRawPointerUp, kModuleTypeIdTouchScreen, i, (float)lastX[i], (float)lastY[i]);
                lastX[i] = -1;
                lastY[i] = -1;
            }
        }
    }
}

static ButtonMapping mapping[] = {
        { SCE_CTRL_UP,        AKEYCODE_DPAD_UP },
        { SCE_CTRL_DOWN,      AKEYCODE_DPAD_DOWN },
        { SCE_CTRL_LEFT,      AKEYCODE_DPAD_LEFT },
        { SCE_CTRL_RIGHT,     AKEYCODE_DPAD_RIGHT },
        { SCE_CTRL_CROSS,     AKEYCODE_BUTTON_A },
        { SCE_CTRL_CIRCLE,    AKEYCODE_BUTTON_B },
        { SCE_CTRL_SQUARE,    AKEYCODE_B },
        { SCE_CTRL_TRIANGLE,  AKEYCODE_X },
        { SCE_CTRL_L1,        AKEYCODE_BUTTON_L1 },
        { SCE_CTRL_R1,        AKEYCODE_BUTTON_R1 },
        { SCE_CTRL_START,     AKEYCODE_BUTTON_START },
        { SCE_CTRL_SELECT,    AKEYCODE_BUTTON_SELECT },
};

uint32_t old_buttons = 0, current_buttons = 0, pressed_buttons = 0, released_buttons = 0;

float lastLx = 0.0f, lastLy = 0.0f, lastRx = 0.0f, lastRy = 0.0f;
float lx = 0.0f, ly = 0.0f, rx = 0.0f, ry = 0.0f;

extern volatile int silentLoad;

void silentStartHelper() {
    sceKernelDelayThread(30000 * 1000); // from first NativeOnDrawFrame to first controls prompt

    NativeOnKeyDown(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);
    sceKernelDelayThread(10000);
    NativeOnKeyUp(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);

    sceKernelDelayThread(3036 * 1000); // from first controls prompt to active "PLAY" button

    NativeOnKeyDown(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);
    sceKernelDelayThread(10000);
    NativeOnKeyUp(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);

    sceKernelDelayThread(2100 * 1000); // from "PLAY" button to "RESUME" button appearance

    NativeOnKeyDown(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);
    sceKernelDelayThread(10000);
    NativeOnKeyUp(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);

    sceKernelDelayThread(700000); // Extra click in a second, just in case

    NativeOnKeyDown(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);
    sceKernelDelayThread(10000);
    NativeOnKeyUp(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);

    sceKernelDelayThread(700000); // And another one

    NativeOnKeyDown(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);
    sceKernelDelayThread(10000);
    NativeOnKeyUp(&jni, (void *) 0x42424242, 600, AKEYCODE_BUTTON_A, 1);

    silentLoad = 0;
}

void runSilentStartHelper() {
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 64*1024);
    pthread_create(&t, &attr, silentStartHelper, NULL);
    pthread_detach(t);
}

void pollPad() {
    SceCtrlData pad;
    sceCtrlPeekBufferPositiveExt2(0, &pad, 1);

    { // Gamepad buttons
        old_buttons = current_buttons;
        current_buttons = pad.buttons;
        pressed_buttons = current_buttons & ~old_buttons;
        released_buttons = ~current_buttons & old_buttons;

        for (int i = 0; i < sizeof(mapping) / sizeof(ButtonMapping); i++) {
            if (pressed_buttons & mapping[i].sce_button) {
                NativeOnKeyDown(&jni, (void *) 0x42424242, 600, mapping[i].android_button, 1);
            }
            if (released_buttons & mapping[i].sce_button) {
                NativeOnKeyUp(&jni, (void *) 0x42424242, 600, mapping[i].android_button, 1);
            }
        }
    }

    // Analog sticks

    lx = ((float)pad.lx - 128.0f) / 128.0f;
    ly = ((float)pad.ly - 128.0f) / 128.0f;
    rx = ((float)pad.rx - 128.0f) / 128.0f;
    ry = ((float)pad.ry - 128.0f) / 128.0f;

    if (fabsf(lx) < leftStickDeadZone)
        lx = 0.0f;
    if (fabsf(ly) < leftStickDeadZone)
        ly = 0.0f;
    if (fabsf(rx) < rightStickDeadZone)
        rx = 0.0f;
    if (fabsf(ry) < rightStickDeadZone)
        ry = 0.0f;

    if (fabsf(lx) > L_OUTER_DEADZONE) {
        if (lx > L_OUTER_DEADZONE)
            lx = 1.0f;
        if (lx < -L_OUTER_DEADZONE)
            lx = -1.0f;
    }
    if (fabsf(ly) > L_OUTER_DEADZONE) {
        if (ly > L_OUTER_DEADZONE)
            ly = 1.0f;
        if (ly < -L_OUTER_DEADZONE)
            ly = -1.0f;
    }

    if (fabsf(rx) > R_OUTER_DEADZONE) {
        if (rx > R_OUTER_DEADZONE)
            rx = R_OUTER_DEADZONE;
        if (rx < -R_OUTER_DEADZONE)
            rx = -R_OUTER_DEADZONE;
    }
    if (fabsf(ry) > R_OUTER_DEADZONE) {
        if (ry > R_OUTER_DEADZONE)
            ry = R_OUTER_DEADZONE;
        if (ry < -R_OUTER_DEADZONE)
            ry = -R_OUTER_DEADZONE;
    }

    if ((rx == 0.f && ry == 0.f) && (lastRx != 0.f || lastRy != 0.f)) {
        // rstick stop
        NativeDispatchGenericMotionEvent(&jni, (void *) 0x42424242, lx, ly, rx, ry, 3);
    }
    if ((lx == 0.f && ly == 0.f) && (lastLx != 0.f || lastLy != 0.f)) {
        // lstick stop
        NativeDispatchGenericMotionEvent(&jni, (void *) 0x42424242, lx, ly, rx, ry, 2);
    }
    if (lx != 0.f || ly != 0.f) {
        // lstick move
        NativeDispatchGenericMotionEvent(&jni, (void *) 0x42424242, lx, ly, rx, ry, 0);
    }
    if (rx != 0.f || ry != 0.f) {
        // rstick move
        NativeDispatchGenericMotionEvent(&jni, (void *) 0x42424242, lx, ly, rx, ry, 1);
    }

    lastLx = lx;
    lastLy = ly;
    lastRx = rx;
    lastRy = ry;
}

void pollAccel() {
    SceMotionSensorState sensor;
    sceMotionGetSensorState(&sensor, 1);

    float x = sensor.accelerometer.x*GRAVITY_CONSTANT;
    float y = sensor.accelerometer.y*GRAVITY_CONSTANT;
    float z = sensor.accelerometer.z*GRAVITY_CONSTANT;
    NativeOnAcceleration(&jni, (void*)0x42424242, x, y, z);
}
