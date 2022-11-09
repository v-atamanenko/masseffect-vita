#ifndef SOLOADER_CONTROLS_H
#define SOLOADER_CONTROLS_H

#include <psp2/touch.h>
#include <psp2/ctrl.h>
#include <psp2/motion.h>

#include <math.h>

#define GRAVITY_CONSTANT 9.807f

#define kIdRawPointerCancel 0xe
#define kIdRawPointerDown 0x6000e
#define kIdRawPointerMove 0x4000e
#define kIdRawPointerUp 0x8000e
#define kIdUndefined 0

#define kModuleTypeIdTouchScreen 1000
#define kModuleTypeIdTouchPad 1100

enum {
    ACTION_DOWN = 1,
    ACTION_UP   = 2,
    ACTION_MOVE = 3,
};

enum {
    AKEYCODE_DPAD_UP = 19,
    AKEYCODE_DPAD_DOWN = 20,
    AKEYCODE_DPAD_LEFT = 21,
    AKEYCODE_DPAD_RIGHT = 22,
    AKEYCODE_X = 53,
    AKEYCODE_B = 30,
    AKEYCODE_BUTTON_A = 96,
    AKEYCODE_BUTTON_B = 97,
    AKEYCODE_BUTTON_L1 = 102,
    AKEYCODE_BUTTON_R1 = 103,
    AKEYCODE_BUTTON_START = 108,
    AKEYCODE_BUTTON_SELECT = 109,
};

typedef struct {
    uint32_t sce_button;
    uint32_t android_button;
} ButtonMapping;

void controls_init();
void controls_poll();
void pollTouch();
void pollPad();
void pollAccel();

#endif // SOLOADER_CONTROLS_H
