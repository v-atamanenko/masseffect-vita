/*
 * android/EAAudioCore.h
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_EAAUDIOCORE_H
#define SOLOADER_EAAUDIOCORE_H

#include <FalsoJNI/FalsoJNI.h>

void EAAudioCore__Startup();
void EAAudioCore__Shutdown();

int EAAudioCore_AudioTrack_write(int id, va_list args);
void EAAudioCore_AudioTrack_play(int id, va_list args);
void EAAudioCore_AudioTrack_stop(int id, va_list args);

#endif // SOLOADER_EAAUDIOCORE_H
