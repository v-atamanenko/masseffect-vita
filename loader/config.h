/*
 * config.h
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_CONFIG_H
#define SOLOADER_CONFIG_H

/*
 * Following config definitions are set from CMake:
 * DEBUG, DEBUG_GL, DEBUG_SOLOADER, DATA_PATH, DATA_PATH_INT, SO_PATH
 */

#define LOAD_ADDRESS 0x99000000

#define MEMORY_NEWLIB_MB 220

#define GLSL_PATH DATA_PATH
#define GXP_PATH "app0:shaders"

#endif // SOLOADER_CONFIG_H
