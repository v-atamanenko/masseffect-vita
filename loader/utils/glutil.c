/*
 * glutil.c
 *
 * OpenGL API initializer, related functions.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils/glutil.h"
#include "utils/dialog.h"
#include "sha1.h"

#include <stdio.h>
#include <malloc.h>
#include <psp2/kernel/sysmem.h>
#include <string.h>

void gl_preload() {
    if (!file_exists("ur0:/data/libshacccg.suprx")
        && !file_exists("ur0:/data/external/libshacccg.suprx")) {
        fatal_error("Error: libshacccg.suprx is not installed. Google \"ShaRKBR33D\" for quick installation.");
    }
}

void gl_init() {
    vglInitExtended(0, 960, 544, 8 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
}

void gl_swap() {
    vglSwapBuffers(GL_FALSE);
}


void glShaderSourceHook(GLuint shader, GLsizei count, const GLchar **string,
                        const GLint *_length) {
    uint32_t sha1[5];
    SHA1_CTX ctx;
    int length;

    if (!string) {
        printf("string == null\n");
        return;
    } else if (!*string) {
        printf("*string == null\n");
        return;
    }

    // From OGL specs: If length is NULL, each string is assumed to be null terminated.
    if (!_length) {
        length = (int)strlen(*string);
    } else {
        length = *_length;
    }

    sha1_init(&ctx);
    sha1_update(&ctx, (uint8_t *)*string, length);
    sha1_final(&ctx, (uint8_t *)sha1);

    char sha_name[64];
    snprintf(sha_name, sizeof(sha_name), "%08x%08x%08x%08x%08x",
             sha1[0], sha1[1], sha1[2], sha1[3], sha1[4]);

    char gxp_path[128];
    snprintf(gxp_path, sizeof(gxp_path), "%s/%s.gxp", GXP_PATH, sha_name);

    FILE *file = fopen(gxp_path, "rb");
    if (!file) {
        debugPrintf("[%i] Could not find %s\n", count, gxp_path);

        char glsl_path[128];
        snprintf(glsl_path, sizeof(glsl_path), "%s/%s.glsl",
                 GLSL_PATH, sha_name);

        file = fopen(glsl_path, "w");
        if (file) {
            fwrite(*string, 1, length, file);
            fclose(file);
        }

        snprintf(gxp_path, sizeof(gxp_path), "%s/%s.gxp",
                 GXP_PATH, "5e7eb5913ac3778652028876934235ebfb96beae");
        file = fopen(gxp_path, "rb");
    }

    if (file) {
        size_t shaderSize;
        void *shaderBuf;

        fseek(file, 0, SEEK_END);
        shaderSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        shaderBuf = malloc(shaderSize);
        fread(shaderBuf, 1, shaderSize, file);
        fclose(file);

        glShaderBinary(1, &shader, 0, shaderBuf, (int32_t)shaderSize);

        free(shaderBuf);
    }
}