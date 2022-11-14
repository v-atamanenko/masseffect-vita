/*
 * settings.h
 *
 * Loader settings that can be set via the companion app.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_SETTINGS_H
#define SOLOADER_SETTINGS_H
#include "stdbool.h"

extern float aimSensitivity;
extern float leftStickDeadZone;
extern float rightStickDeadZone;
extern int fpsLock;

extern bool cheat_infiniteAmmo;
extern bool cheat_infiniteHealth;
extern bool cheat_instantKill;
extern bool cheat_noclip;

void loadSettings(void);

#endif // SOLOADER_SETTINGS_H
