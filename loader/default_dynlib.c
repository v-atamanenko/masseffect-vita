/*
 * default_dynlib.c
 *
 * Resolving dynamic imports of the .so.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

// Disable IDE complaints about _identifiers and global interfaces
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#pragma ide diagnostic ignored "cppcoreguidelines-interfaces-global-init"

#include "default_dynlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <netdb.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <zlib.h>

#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <psp2/kernel/clib.h>
#include <dirent.h>
#include <utime.h>

#include "utils/glutil.h"
#include "libc_bridge.h"

#include "reimpl/env.h"
#include "reimpl/io.h"
#include "reimpl/log.h"
#include "reimpl/mem.h"
#include "reimpl/sys.h"
#include "reimpl/pthr.h"

#define PAGE_SIZE 4096
unsigned int __page_size = 4096;

extern void * _ZNSt9exceptionD2Ev;
extern void * _ZSt17__throw_bad_allocv;
extern void * _ZSt9terminatev;
extern void * _ZdaPv;
extern void * _ZdlPv;
extern void * _Znaj;
extern void * __cxa_allocate_exception;
extern void * __cxa_begin_catch;
extern void * __cxa_end_catch;
extern void * __cxa_free_exception;
extern void * __cxa_rethrow;
extern void * __cxa_throw;
extern void * __gxx_personality_v0;
extern void *_ZNSt8bad_castD1Ev;
extern void *_ZTISt8bad_cast;
extern void *_ZTISt9exception;
extern void *_ZTVN10__cxxabiv117__class_type_infoE;
extern void *_ZTVN10__cxxabiv120__si_class_type_infoE;
extern void *_ZTVN10__cxxabiv121__vmi_class_type_infoE;
extern void *_Znwj;
extern void *__aeabi_atexit;
extern void *__aeabi_memclr;
extern void *__aeabi_memcpy;
extern void *__aeabi_memmove;
extern void *__aeabi_memset;

extern void *__cxa_atexit;
extern void *__cxa_finalize;
extern void *__cxa_pure_virtual;
extern void *__cxa_call_unexpected;
extern void *__cxa_guard_acquire;
extern void *__cxa_guard_release;
extern void *__gnu_unwind_frame;
extern void *__stack_chk_fail;
extern void *__stack_chk_guard;
extern void *__dso_handle;

static FILE __sF_fake[0x100][3];

extern const char *BIONIC_ctype_;
extern const short *BIONIC_tolower_tab_;
extern const short *BIONIC_toupper_tab_;

int timezone = 1;
char *__tzname[2];

#define __ATOMIC_INLINE__ static __inline__ __attribute__((always_inline))

__ATOMIC_INLINE__ int __atomic_swap(int new_value, volatile int *ptr)
{
    int old_value;
    do {
        old_value = *ptr;
    } while (__sync_val_compare_and_swap(ptr, old_value, new_value) != old_value);
    return old_value;
}

__ATOMIC_INLINE__ int __atomic_inc(volatile int *ptr)
{
    return __sync_fetch_and_add (ptr, 1);
}

__ATOMIC_INLINE__ int __atomic_dec(volatile int *ptr)
{
    return __sync_fetch_and_sub (ptr, 1);
}

__ATOMIC_INLINE__ int __atomic_cmpxchg(int old_value, int new_value, volatile int* ptr)
{
    /* We must return 0 on success */
    return __sync_val_compare_and_swap(ptr, old_value, new_value) != old_value;
}

int __gnu_Unwind_Find_exidx_tempwrap() { fprintf(stderr, "ret0d call!!! __gnu_Unwind_Find_exidx_tempwrap\n"); return 0; }
int poll_tempwrap() { fprintf(stderr, "ret0d call!!! poll_tempwrap\n"); return 0; }
int __cxa_type_match_tempwrap() { fprintf(stderr, "ret0d call!!! __cxa_type_match_tempwrap\n"); return 0; }
int __cxa_begin_cleanup_tempwrap() { fprintf(stderr, "ret0d call!!! __cxa_begin_cleanup_tempwrap\n"); return 0; }
int __isinf_tempwrap() { fprintf(stderr, "ret0d call!!! __isinf_tempwrap\n"); return 0; }
int prctl_tempwrap() { fprintf(stderr, "ret0d call!!! prctl_tempwrap\n"); return 0; }
int sysconf_tempwrap() { fprintf(stderr, "ret0d call!!! sysconf_tempwrap\n"); return 0; }
int setenv_ret0() { fprintf(stderr, "ret0d call!!! setenv_ret0\n"); return 0; }
int waitpid_tempwrap() { fprintf(stderr, "ret0d call!!! waitpid_tempwrap\n"); return 0; }
int execv_tempwrap() { fprintf(stderr, "ret0d call!!! execv_tempwrap\n"); return 0; }

int glDrawTexfOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexfOES_tempwrap\n"); return 0; }
int glDrawTexsvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexsvOES_tempwrap\n"); return 0; }
int glIsRenderbufferOES_tempwrap() { fprintf(stderr, "ret0d call!!! glIsRenderbufferOES_tempwrap\n"); return 0; }
int glDrawTexxvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexxvOES_tempwrap\n"); return 0; }
int glTexGenxvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGenxvOES_tempwrap\n"); return 0; }
int glDrawTexivOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexivOES_tempwrap\n"); return 0; }
int glTexGenfOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGenfOES_tempwrap\n"); return 0; }
int glGetTexGenxvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexGenxvOES_tempwrap\n"); return 0; }
int glGetTexGenivOES_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexGenivOES_tempwrap\n"); return 0; }
int glMatrixIndexPointerOES_tempwrap() { fprintf(stderr, "ret0d call!!! glMatrixIndexPointerOES_tempwrap\n"); return 0; }
int glTexGenfvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGenfvOES_tempwrap\n"); return 0; }
int glWeightPointerOES_tempwrap() { fprintf(stderr, "ret0d call!!! glWeightPointerOES_tempwrap\n"); return 0; }
int glCurrentPaletteMatrixOES_tempwrap() { fprintf(stderr, "ret0d call!!! glCurrentPaletteMatrixOES_tempwrap\n"); return 0; }
int glGetBufferPointervOES_tempwrap() { fprintf(stderr, "ret0d call!!! glGetBufferPointervOES_tempwrap\n"); return 0; }
int glEGLImageTargetRenderbufferStorageOES_tempwrap() { fprintf(stderr, "ret0d call!!! glEGLImageTargetRenderbufferStorageOES_tempwrap\n"); return 0; }
int glLoadPaletteFromModelViewMatrixOES_tempwrap() { fprintf(stderr, "ret0d call!!! glLoadPaletteFromModelViewMatrixOES_tempwrap\n"); return 0; }
int glDrawTexsOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexsOES_tempwrap\n"); return 0; }
int glGetTexGenfvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexGenfvOES_tempwrap\n"); return 0; }
int glDrawTexiOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexiOES_tempwrap\n"); return 0; }
int glEGLImageTargetTexture2DOES_tempwrap() { fprintf(stderr, "ret0d call!!! glEGLImageTargetTexture2DOES_tempwrap\n"); return 0; }
int glQueryMatrixxOES_tempwrap() { fprintf(stderr, "ret0d call!!! glQueryMatrixxOES_tempwrap\n"); return 0; }
int glTexGenxOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGenxOES_tempwrap\n"); return 0; }
int glDrawTexxOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexxOES_tempwrap\n"); return 0; }
int glTexGenivOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGenivOES_tempwrap\n"); return 0; }
int glDrawTexfvOES_tempwrap() { fprintf(stderr, "ret0d call!!! glDrawTexfvOES_tempwrap\n"); return 0; }
int glTexGeniOES_tempwrap() { fprintf(stderr, "ret0d call!!! glTexGeniOES_tempwrap\n"); return 0; }
int glGetPointerv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetPointerv_tempwrap\n"); return 0; }
int glCompressedTexSubImage2D_tempwrap() { fprintf(stderr, "ret0d call!!! glCompressedTexSubImage2D_tempwrap\n"); return 0; }
int glSampleCoverage_tempwrap() { fprintf(stderr, "ret0d call!!! glSampleCoverage_tempwrap\n"); return 0; }
int glPointSizePointerOES_tempwrap() { fprintf(stderr, "ret0d call!!! glPointSizePointerOES_tempwrap\n"); return 0; }
int glLightf_tempwrap() { fprintf(stderr, "ret0d call!!! glLightf_tempwrap\n"); return 0; }
int glGetLightfv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetLightfv_tempwrap\n"); return 0; }
int glPointParameterf_tempwrap() { fprintf(stderr, "ret0d call!!! glPointParameterf_tempwrap\n"); return 0; }
int glPointParameterxv_tempwrap() { fprintf(stderr, "ret0d call!!! glPointParameterxv_tempwrap\n"); return 0; }
int glMultiTexCoord4x_tempwrap() { fprintf(stderr, "ret0d call!!! glMultiTexCoord4x_tempwrap\n"); return 0; }
int glTexEnviv_tempwrap() { fprintf(stderr, "ret0d call!!! glTexEnviv_tempwrap\n"); return 0; }
int glGetLightxv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetLightxv_tempwrap\n"); return 0; }
int glSampleCoveragex_tempwrap() { fprintf(stderr, "ret0d call!!! glSampleCoveragex_tempwrap\n"); return 0; }
int glMaterialx_tempwrap() { fprintf(stderr, "ret0d call!!! glMaterialx_tempwrap\n"); return 0; }
int glPointParameterfv_tempwrap() { fprintf(stderr, "ret0d call!!! glPointParameterfv_tempwrap\n"); return 0; }
int glTexParameterxv_tempwrap() { fprintf(stderr, "ret0d call!!! glTexParameterxv_tempwrap\n"); return 0; }
int glGetTexParameterfv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexParameterfv_tempwrap\n"); return 0; }
int glCopyTexImage2D_tempwrap() { fprintf(stderr, "ret0d call!!! glCopyTexImage2D_tempwrap\n"); return 0; }
int glMaterialf_tempwrap() { fprintf(stderr, "ret0d call!!! glMaterialf_tempwrap\n"); return 0; }
int glGetTexEnvxv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexEnvxv_tempwrap\n"); return 0; }
int glGetTexParameterxv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexParameterxv_tempwrap\n"); return 0; }
int glGetMaterialxv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetMaterialxv_tempwrap\n"); return 0; }
int glGetClipPlanef_tempwrap() { fprintf(stderr, "ret0d call!!! glGetClipPlanef_tempwrap\n"); return 0; }
int glGetFixedv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetFixedv_tempwrap\n"); return 0; }
int glGetMaterialfv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetMaterialfv_tempwrap\n"); return 0; }
int glNormal3x_tempwrap() { fprintf(stderr, "ret0d call!!! glNormal3x_tempwrap\n"); return 0; }
int glGetTexParameteriv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexParameteriv_tempwrap\n"); return 0; }
int glLightModelf_tempwrap() { fprintf(stderr, "ret0d call!!! glLightModelf_tempwrap\n"); return 0; }
int glTexParameterfv_tempwrap() { fprintf(stderr, "ret0d call!!! glTexParameterfv_tempwrap\n"); return 0; }
int glIsBuffer_tempwrap() { fprintf(stderr, "ret0d call!!! glIsBuffer_tempwrap\n"); return 0; }
int glGetTexEnviv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexEnviv_tempwrap\n"); return 0; }
int glGetClipPlanex_tempwrap() { fprintf(stderr, "ret0d call!!! glGetClipPlanex_tempwrap\n"); return 0; }
int glLightx_tempwrap() { fprintf(stderr, "ret0d call!!! glLightx_tempwrap\n"); return 0; }
int glMultiTexCoord4f_tempwrap() { fprintf(stderr, "ret0d call!!! glMultiTexCoord4f_tempwrap\n"); return 0; }
int glPointParameterx_tempwrap() { fprintf(stderr, "ret0d call!!! glPointParameterx_tempwrap\n"); return 0; }
int glGetTexEnvfv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetTexEnvfv_tempwrap\n"); return 0; }
int glLightModelx_tempwrap() { fprintf(stderr, "ret0d call!!! glLightModelx_tempwrap\n"); return 0; }
int glLogicOp_tempwrap() { fprintf(stderr, "ret0d call!!! glLogicOp_tempwrap\n"); return 0; }
int glPixelStorei_tempwrap() { /*fprintf(stderr, "ret0d call!!! glPixelStorei_tempwrap\n");*/ return 0; }
int glCopyTexSubImage2D_tempwrap() { fprintf(stderr, "ret0d call!!! glCopyTexSubImage2D_tempwrap\n"); return 0; }
int glGetRenderbufferParameterivOES_tempwrap() { fprintf(stderr, "ret0d call!!! glGetRenderbufferParameterivOES_tempwrap\n"); return 0; }

int ioctl_tempwrap() { fprintf(stderr, "ret0d call!!! ioctl_tempwrap\n"); return 0; }
int setlocale_tempwrap() { fprintf(stderr, "ret0d call!!! setlocale_tempwrap\n"); return 0; }
int statfs_tempwrap() { fprintf(stderr, "ret0d call!!! statfs_tempwrap\n"); return 0; }
int sigaction_tempwrap() { fprintf(stderr, "ret0d call!!! sigaction_tempwrap\n"); return 0; }

int glBlendColor_tempwrap() { fprintf(stderr, "ret0d call!!! glBlendColor_tempwrap\n"); return 0; }
int glGetUniformfv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetUniformfv_tempwrap\n"); return 0; }
int glDetachShader_tempwrap() { fprintf(stderr, "ret0d call!!! glDetachShader_tempwrap\n"); return 0; }
int glGetUniformiv_tempwrap() { fprintf(stderr, "ret0d call!!! glGetUniformiv_tempwrap\n"); return 0; }
int glValidateProgram_tempwrap() { fprintf(stderr, "ret0d call!!! glValidateProgram_tempwrap\n"); return 0; }
int glGetRenderbufferParameteriv_tempwrap(GLenum target,
                                          GLenum pname,
                                          GLint *params) { fprintf(stderr, "ret0d call!!! glGetRenderbufferParameteriv(0x%x, 0x%x, 0x%x)\n", target, pname, params); return 0; }
int glIsShader_tempwrap() { fprintf(stderr, "ret0d call!!! glIsShader_tempwrap\n"); return 0; }
int glGetShaderPrecisionFormat_tempwrap() { fprintf(stderr, "ret0d call!!! glGetShaderPrecisionFormat_tempwrap\n"); return 0; }

static FILE __sF_fake[0x100][3];

// NOLINT(cppcoreguidelines-interfaces-global-init)
so_default_dynlib default_dynlib[] = {
        { "__aeabi_atexit", (uintptr_t)&__aeabi_atexit },
        { "__android_log_print", (uintptr_t)&android_log_print },
        { "__android_log_vprint", (uintptr_t)&android_log_vprint },
        { "__android_log_write", (uintptr_t)&android_log_write },
        { "__atomic_cmpxchg", (uintptr_t)&__atomic_cmpxchg },
        { "__atomic_dec", (uintptr_t)&__atomic_dec },
        { "__atomic_inc", (uintptr_t)&__atomic_inc },
        { "__atomic_swap", (uintptr_t)&__atomic_swap },
        { "__cxa_finalize", (uintptr_t)&__cxa_finalize },
        { "__cxa_guard_acquire", (uintptr_t)&__cxa_guard_acquire },
        { "__cxa_guard_release", (uintptr_t)&__cxa_guard_release },
        { "__cxa_pure_virtual", (uintptr_t)&__cxa_pure_virtual },
        { "__cxa_type_match", (uintptr_t)&__cxa_type_match_tempwrap },
        { "__errno", (uintptr_t)&__errno },
        { "__gnu_Unwind_Find_exidx", (uintptr_t)&__gnu_Unwind_Find_exidx_tempwrap },
        { "__isinf", (uintptr_t)&__isinf_tempwrap },
        { "__page_size", (uintptr_t)&__page_size },
        { "__sF", (uintptr_t)&__sF_fake },
        { "__stack_chk_fail", (uintptr_t)&__stack_chk_fail },
        { "__stack_chk_guard", (uintptr_t)&__stack_chk_guard },
        { "_ctype_", (uintptr_t)&BIONIC_ctype_ },
        { "_tolower_tab_", (uintptr_t)&BIONIC_tolower_tab_ },
        { "_toupper_tab_", (uintptr_t)&BIONIC_toupper_tab_ },
        { "abort", (uintptr_t)&abort },
        { "acos", (uintptr_t)&acos },
        { "acosf", (uintptr_t)&acosf },
        { "atan2", (uintptr_t)&atan2 },
        { "atan2f", (uintptr_t)&atan2f },
        { "atoi", (uintptr_t)&atoi },
        { "bind", (uintptr_t)&bind },
        { "calloc", (uintptr_t)&calloc },
        { "ceil", (uintptr_t)&ceil },
        { "ceilf", (uintptr_t)&ceilf },
        { "clock", (uintptr_t)&clock },
        { "clock_gettime", (uintptr_t)&clock_gettime_soloader },
        { "close", (uintptr_t)&close_soloader },
        { "closedir", (uintptr_t)&closedir_soloader },
        { "connect", (uintptr_t)&connect },
        { "cos", (uintptr_t)&cos },
        { "cosf", (uintptr_t)&cosf },
        { "exit", (uintptr_t)&exit },
        { "exp", (uintptr_t)&exp },
        { "expf", (uintptr_t)&expf },
        { "fclose", (uintptr_t)&fclose },
        { "fcntl", (uintptr_t)&fcntl_soloader },
        { "fflush", (uintptr_t)&fflush },
        { "floor", (uintptr_t)&floor },
        { "floorf", (uintptr_t)&floorf },
        { "fmod", (uintptr_t)&fmod },
        { "fmodf", (uintptr_t)&fmodf },
        { "fopen", (uintptr_t)&fopen_soloader },
        { "fprintf", (uintptr_t)&fprintf },
        { "fputc", (uintptr_t)&fputc },
        { "fputs", (uintptr_t)&fputs },
        { "fread", (uintptr_t)&fread },
        { "free", (uintptr_t)&free },
        { "freeaddrinfo", (uintptr_t)&freeaddrinfo },
        { "fseek", (uintptr_t)&fseek },
        { "fstat", (uintptr_t)&fstat_soloader },
        { "fsync", (uintptr_t)&fsync_soloader },
        { "ftell", (uintptr_t)&ftell },
        { "ftruncate", (uintptr_t)&ftruncate },
        { "fwrite", (uintptr_t)&fwrite },
        { "getaddrinfo", (uintptr_t)&getaddrinfo },
        { "getcwd", (uintptr_t)&getcwd },
        { "getpeername", (uintptr_t)&getpeername },
        { "getsockname", (uintptr_t)&getsockname },
        { "getsockopt", (uintptr_t)&getsockopt },
        { "gettimeofday", (uintptr_t)&gettimeofday },
        { "glActiveTexture", (uintptr_t)&glActiveTexture },
        { "glAttachShader", (uintptr_t)&glAttachShader },
        { "glBindAttribLocation", (uintptr_t)&glBindAttribLocation },
        { "glBindBuffer", (uintptr_t)&glBindBuffer },
        { "glBindFramebuffer", (uintptr_t)&glBindFramebuffer },
        { "glBindRenderbuffer", (uintptr_t)&glBindRenderbuffer },
        { "glBindTexture", (uintptr_t)&glBindTexture },
        { "glBlendColor", (uintptr_t)&glBlendColor_tempwrap },
        { "glBlendEquation", (uintptr_t)&glBlendEquation },
        { "glBlendEquationSeparate", (uintptr_t)&glBlendEquationSeparate },
        { "glBlendFunc", (uintptr_t)&glBlendFunc },
        { "glBlendFuncSeparate", (uintptr_t)&glBlendFuncSeparate },
        { "glBufferData", (uintptr_t)&glBufferData },
        { "glBufferSubData", (uintptr_t)&glBufferSubData },
        { "glCheckFramebufferStatus", (uintptr_t)&glCheckFramebufferStatus },
        { "glClear", (uintptr_t)&glClear },
        { "glClearColor", (uintptr_t)&glClearColor },
        { "glClearDepthf", (uintptr_t)&glClearDepthf },
        { "glClearStencil", (uintptr_t)&glClearStencil },
        { "glColorMask", (uintptr_t)&glColorMask },
        { "glCompileShader", (uintptr_t)&ret0 },
        { "glCompressedTexImage2D", (uintptr_t)&glCompressedTexImage2D },
        { "glCompressedTexSubImage2D", (uintptr_t)&glCompressedTexSubImage2D_tempwrap },
        { "glCopyTexImage2D", (uintptr_t)&glCopyTexImage2D_tempwrap },
        { "glCopyTexSubImage2D", (uintptr_t)&glCopyTexSubImage2D_tempwrap },
        { "glCreateProgram", (uintptr_t)&glCreateProgram },
        { "glCreateShader", (uintptr_t)&glCreateShader },
        { "glCullFace", (uintptr_t)&glCullFace },
        { "glDeleteBuffers", (uintptr_t)&glDeleteBuffers },
        { "glDeleteFramebuffers", (uintptr_t)&glDeleteFramebuffers },
        { "glDeleteProgram", (uintptr_t)&glDeleteProgram },
        { "glDeleteRenderbuffers", (uintptr_t)&glDeleteRenderbuffers },
        { "glDeleteShader", (uintptr_t)&glDeleteShader },
        { "glDeleteTextures", (uintptr_t)&glDeleteTextures },
        { "glDepthFunc", (uintptr_t)&glDepthFunc },
        { "glDepthMask", (uintptr_t)&glDepthMask },
        { "glDepthRangef", (uintptr_t)&glDepthRangef },
        { "glDetachShader", (uintptr_t)&glDetachShader_tempwrap },
        { "glDisable", (uintptr_t)&glDisable },
        { "glDisableVertexAttribArray", (uintptr_t)&glDisableVertexAttribArray },
        { "glDrawArrays", (uintptr_t)&glDrawArrays },
        { "glDrawElements", (uintptr_t)&glDrawElements },
        { "glEnable", (uintptr_t)&glEnable },
        { "glEnableVertexAttribArray", (uintptr_t)&glEnableVertexAttribArray },
        { "glFinish", (uintptr_t)&glFinish },
        { "glFlush", (uintptr_t)&glFlush },
        { "glFramebufferRenderbuffer", (uintptr_t)&glFramebufferRenderbuffer },
        { "glFramebufferTexture2D", (uintptr_t)&glFramebufferTexture2D },
        { "glFrontFace", (uintptr_t)&glFrontFace },
        { "glGenBuffers", (uintptr_t)&glGenBuffers },
        { "glGenFramebuffers", (uintptr_t)&glGenFramebuffers },
        { "glGenRenderbuffers", (uintptr_t)&glGenRenderbuffers },
        { "glGenTextures", (uintptr_t)&glGenTextures },
        { "glGenerateMipmap", (uintptr_t)&glGenerateMipmap },
        { "glGetActiveAttrib", (uintptr_t)&glGetActiveAttrib },
        { "glGetActiveUniform", (uintptr_t)&glGetActiveUniform },
        { "glGetAttachedShaders", (uintptr_t)&glGetAttachedShaders },
        { "glGetAttribLocation", (uintptr_t)&glGetAttribLocation },
        { "glGetBooleanv", (uintptr_t)&glGetBooleanv },
        { "glGetBufferParameteriv", (uintptr_t)&glGetBufferParameteriv },
        { "glGetError", (uintptr_t)&glGetError },
        { "glGetFloatv", (uintptr_t)&glGetFloatv },
        { "glGetFramebufferAttachmentParameteriv", (uintptr_t)&glGetFramebufferAttachmentParameteriv },
        { "glGetIntegerv", (uintptr_t)&glGetIntegerv },
        { "glGetProgramInfoLog", (uintptr_t)&glGetProgramInfoLog },
        { "glGetProgramiv", (uintptr_t)&glGetProgramiv },
        { "glGetRenderbufferParameteriv", (uintptr_t)&glGetRenderbufferParameteriv_tempwrap },
        { "glGetShaderInfoLog", (uintptr_t)&glGetShaderInfoLog },
        { "glGetShaderPrecisionFormat", (uintptr_t)&glGetShaderPrecisionFormat_tempwrap },
        { "glGetShaderSource", (uintptr_t)&glGetShaderSource },
        { "glGetShaderiv", (uintptr_t)&glGetShaderiv },
        { "glGetString", (uintptr_t)&glGetString },
        { "glGetTexParameterfv", (uintptr_t)&glGetTexParameterfv_tempwrap },
        { "glGetTexParameteriv", (uintptr_t)&glGetTexParameteriv_tempwrap },
        { "glGetUniformLocation", (uintptr_t)&glGetUniformLocation },
        { "glGetUniformfv", (uintptr_t)&glGetUniformfv_tempwrap },
        { "glGetUniformiv", (uintptr_t)&glGetUniformiv_tempwrap },
        { "glGetVertexAttribPointerv", (uintptr_t)&glGetVertexAttribPointerv },
        { "glGetVertexAttribfv", (uintptr_t)&glGetVertexAttribfv },
        { "glGetVertexAttribiv", (uintptr_t)&glGetVertexAttribiv },
        { "glHint", (uintptr_t)&glHint },
        { "glIsBuffer", (uintptr_t)&glIsBuffer_tempwrap },
        { "glIsEnabled", (uintptr_t)&glIsEnabled },
        { "glIsFramebuffer", (uintptr_t)&glIsFramebuffer },
        { "glIsProgram", (uintptr_t)&glIsProgram },
        { "glIsRenderbuffer", (uintptr_t)&glIsRenderbuffer },
        { "glIsShader", (uintptr_t)&glIsShader_tempwrap },
        { "glIsTexture", (uintptr_t)&glIsTexture },
        { "glLineWidth", (uintptr_t)&glLineWidth },
        { "glLinkProgram", (uintptr_t)&glLinkProgram },
        { "glPixelStorei", (uintptr_t)&glPixelStorei },
        { "glPolygonOffset", (uintptr_t)&glPolygonOffset },
        { "glReadPixels", (uintptr_t)&glReadPixels },
        { "glReleaseShaderCompiler", (uintptr_t)&glReleaseShaderCompiler },
        { "glRenderbufferStorage", (uintptr_t)&glRenderbufferStorage },
        { "glSampleCoverage", (uintptr_t)&glSampleCoverage_tempwrap },
        { "glScissor", (uintptr_t)&glScissor },
        { "glShaderBinary", (uintptr_t)&glShaderBinary },
        { "glShaderSource", (uintptr_t)&glShaderSourceHook },
        { "glStencilFunc", (uintptr_t)&glStencilFunc },
        { "glStencilFuncSeparate", (uintptr_t)&glStencilFuncSeparate },
        { "glStencilMask", (uintptr_t)&glStencilMask },
        { "glStencilMaskSeparate", (uintptr_t)&glStencilMaskSeparate },
        { "glStencilOp", (uintptr_t)&glStencilOp },
        { "glStencilOpSeparate", (uintptr_t)&glStencilOpSeparate },
        { "glTexImage2D", (uintptr_t)&glTexImage2D },
        { "glTexParameterf", (uintptr_t)&glTexParameterf },
        { "glTexParameterfv", (uintptr_t)&glTexParameterfv_tempwrap },
        { "glTexParameteri", (uintptr_t)&glTexParameteri },
        { "glTexParameteriv", (uintptr_t)&glTexParameteriv },
        { "glTexSubImage2D", (uintptr_t)&glTexSubImage2D },
        { "glUniform1f", (uintptr_t)&glUniform1f },
        { "glUniform1fv", (uintptr_t)&glUniform1fv },
        { "glUniform1i", (uintptr_t)&glUniform1i },
        { "glUniform1iv", (uintptr_t)&glUniform1iv },
        { "glUniform2f", (uintptr_t)&glUniform2f },
        { "glUniform2fv", (uintptr_t)&glUniform2fv },
        { "glUniform2i", (uintptr_t)&glUniform2i },
        { "glUniform2iv", (uintptr_t)&glUniform2iv },
        { "glUniform3f", (uintptr_t)&glUniform3f },
        { "glUniform3fv", (uintptr_t)&glUniform3fv },
        { "glUniform3i", (uintptr_t)&glUniform3i },
        { "glUniform3iv", (uintptr_t)&glUniform3iv },
        { "glUniform4f", (uintptr_t)&glUniform4f },
        { "glUniform4fv", (uintptr_t)&glUniform4fv },
        { "glUniform4i", (uintptr_t)&glUniform4i },
        { "glUniform4iv", (uintptr_t)&glUniform4iv },
        { "glUniformMatrix2fv", (uintptr_t)&glUniformMatrix2fv },
        { "glUniformMatrix3fv", (uintptr_t)&glUniformMatrix3fv },
        { "glUniformMatrix4fv", (uintptr_t)&glUniformMatrix4fv },
        { "glUseProgram", (uintptr_t)&glUseProgram },
        { "glValidateProgram", (uintptr_t)&glValidateProgram_tempwrap },
        { "glVertexAttrib1f", (uintptr_t)&glVertexAttrib1f },
        { "glVertexAttrib1fv", (uintptr_t)&glVertexAttrib1fv },
        { "glVertexAttrib2f", (uintptr_t)&glVertexAttrib2f },
        { "glVertexAttrib2fv", (uintptr_t)&glVertexAttrib2fv },
        { "glVertexAttrib3f", (uintptr_t)&glVertexAttrib3f },
        { "glVertexAttrib3fv", (uintptr_t)&glVertexAttrib3fv },
        { "glVertexAttrib4f", (uintptr_t)&glVertexAttrib4f },
        { "glVertexAttrib4fv", (uintptr_t)&glVertexAttrib4fv },
        { "glVertexAttribPointer", (uintptr_t)&glVertexAttribPointer },
        { "glViewport", (uintptr_t)&glViewport },
        { "gmtime", (uintptr_t)&gmtime },
        { "ioctl", (uintptr_t)&ioctl_tempwrap },
        { "ldexp", (uintptr_t)&ldexp },
        { "localtime", (uintptr_t)&localtime },
        { "localtime_r", (uintptr_t)&localtime_r },
        { "log", (uintptr_t)&log },
        { "log10", (uintptr_t)&log10 },
        { "log10f", (uintptr_t)&log10f },
        { "longjmp", (uintptr_t)&longjmp },
        { "lrand48", (uintptr_t)&lrand48 },
        { "lseek", (uintptr_t)&lseek_soloader },
        { "malloc", (uintptr_t)&malloc },
        { "memcmp", (uintptr_t)&memcmp },
        { "memcpy", (uintptr_t)&memcpy },
        { "memmove", (uintptr_t)&memmove },
        { "memset", (uintptr_t)&memset },
        { "mkdir", (uintptr_t)&mkdir },
        { "mmap", (uintptr_t)&mmap },
        { "modf", (uintptr_t)&modf },
        { "munmap", (uintptr_t)&munmap },
        { "nanosleep", (uintptr_t)&nanosleep_soloader },
        { "open", (uintptr_t)&open_soloader },
        { "opendir", (uintptr_t)&opendir_soloader },
        { "poll", (uintptr_t)&ret0 },
        { "pow", (uintptr_t)&pow },
        { "powf", (uintptr_t)&powf },
        { "prctl", (uintptr_t)&prctl_tempwrap },
        { "pthread_attr_destroy", (uintptr_t)&pthread_attr_destroy_soloader },
        { "pthread_attr_init", (uintptr_t)&pthread_attr_init_soloader },
        { "pthread_attr_setdetachstate", (uintptr_t)&pthread_attr_setdetachstate_soloader },
        { "pthread_attr_setschedparam", (uintptr_t)&pthread_attr_setschedparam_soloader },
        { "pthread_attr_setstack", (uintptr_t)&pthread_attr_setstack_soloader },
        { "pthread_attr_setstacksize", (uintptr_t)&pthread_attr_setstacksize_soloader },
        { "pthread_cond_broadcast", (uintptr_t)&pthread_cond_broadcast_soloader },
        { "pthread_cond_destroy", (uintptr_t)&pthread_cond_destroy_soloader },
        { "pthread_cond_init", (uintptr_t)&pthread_cond_init_soloader },
        { "pthread_cond_signal", (uintptr_t)&pthread_cond_signal_soloader },
        { "pthread_cond_timedwait", (uintptr_t)&pthread_cond_timedwait_soloader },
        { "pthread_cond_wait", (uintptr_t)&pthread_cond_wait_soloader },
        { "pthread_create", (uintptr_t)&pthread_create_soloader },
        { "pthread_getspecific", (uintptr_t)&pthread_getspecific },
        { "pthread_key_create", (uintptr_t)&pthread_key_create },
        { "pthread_key_delete", (uintptr_t)&pthread_key_delete },
        { "pthread_mutex_destroy", (uintptr_t)&pthread_mutex_destroy_soloader },
        { "pthread_mutex_init", (uintptr_t)&pthread_mutex_init_soloader },
        { "pthread_mutex_lock", (uintptr_t)&pthread_mutex_lock_soloader },
        { "pthread_mutex_trylock", (uintptr_t)&pthread_mutex_trylock_soloader },
        { "pthread_mutex_unlock", (uintptr_t)&pthread_mutex_unlock_soloader },
        { "pthread_mutexattr_destroy", (uintptr_t)&pthread_mutexattr_destroy_soloader },
        { "pthread_mutexattr_init", (uintptr_t)&pthread_mutexattr_init_soloader },
        { "pthread_mutexattr_setpshared", (uintptr_t)&pthread_mutexattr_setpshared_soloader },
        { "pthread_mutexattr_settype", (uintptr_t)&pthread_mutexattr_settype_soloader },
        { "pthread_once", (uintptr_t)&pthread_once },
        { "pthread_self", (uintptr_t)&pthread_self_soloader },
        { "pthread_setspecific", (uintptr_t)&pthread_setspecific },
        { "qsort", (uintptr_t)&qsort },
        { "raise", (uintptr_t)&raise },
        { "read", (uintptr_t)&read_soloader },
        { "readdir", (uintptr_t)&readdir_soloader },
        { "readdir_r", (uintptr_t)&readdir_r_soloader },
        { "realloc", (uintptr_t)&realloc },
        { "recv", (uintptr_t)&recv },
        { "recvfrom", (uintptr_t)&recvfrom },
        { "remove", (uintptr_t)&remove },
        { "rename", (uintptr_t)&rename },
        { "rmdir", (uintptr_t)&rmdir },
        { "roundf", (uintptr_t)&roundf },
        { "sbrk", (uintptr_t)&sbrk },
        { "sched_yield", (uintptr_t)&sched_yield },
        { "sem_destroy", (uintptr_t)&sem_destroy_soloader },
        { "sem_getvalue", (uintptr_t)&sem_getvalue_soloader },
        { "sem_init", (uintptr_t)&sem_init_soloader },
        { "sem_post", (uintptr_t)&sem_post_soloader },
        { "sem_timedwait", (uintptr_t)&sem_timedwait_soloader },
        { "sem_trywait", (uintptr_t)&sem_trywait_soloader },
        { "sem_wait", (uintptr_t)&sem_wait_soloader },
        { "send", (uintptr_t)&send },
        { "sendto", (uintptr_t)&sendto },
        { "setjmp", (uintptr_t)&setjmp },
        { "setlocale", (uintptr_t)&setlocale_tempwrap },
        { "setsockopt", (uintptr_t)&setsockopt },
        { "shutdown", (uintptr_t)&shutdown },
        { "sigaction", (uintptr_t)&sigaction_tempwrap },
        { "sin", (uintptr_t)&sin },
        { "sinf", (uintptr_t)&sinf },
        { "snprintf", (uintptr_t)&snprintf },
        { "socket", (uintptr_t)&socket },
        { "sprintf", (uintptr_t)&sprintf },
        { "sqrt", (uintptr_t)&sqrt },
        { "sqrtf", (uintptr_t)&sqrtf },
        { "srand48", (uintptr_t)&srand48 },
        { "stat", (uintptr_t)&stat_soloader },
        { "statfs", (uintptr_t)&statfs_tempwrap },
        { "strcat", (uintptr_t)&strcat },
        { "strchr", (uintptr_t)&strchr },
        { "strcmp", (uintptr_t)&strcmp },
        { "strcpy", (uintptr_t)&strcpy },
        { "strdup", (uintptr_t)&strdup },
        { "strlen", (uintptr_t)&strlen },
        { "strncmp", (uintptr_t)&strncmp },
        { "strncpy", (uintptr_t)&strncpy },
        { "strstr", (uintptr_t)&strstr },
        { "strtod", (uintptr_t)&strtod },
        { "strtok", (uintptr_t)&strtok },
        { "strtol", (uintptr_t)&strtol },
        { "strtoul", (uintptr_t)&strtoul },
        { "tan", (uintptr_t)&tan },
        { "tanf", (uintptr_t)&tanf },
        { "time", (uintptr_t)&time },
        { "timezone", (uintptr_t)&timezone },
        { "tzname", (uintptr_t)&__tzname },
        { "unlink", (uintptr_t)&unlink },
        { "usleep", (uintptr_t)&usleep },
        { "wcscpy", (uintptr_t)&wcscpy },
        { "wcslen", (uintptr_t)&wcslen },
        { "wcstol", (uintptr_t)&wcstol },
        { "wcstoul", (uintptr_t)&wcstoul },
        { "write", (uintptr_t)&write_soloader },
};

void resolve_imports(so_module* mod) {
    printf("sz: %i\n", sizeof(default_dynlib) / sizeof(default_dynlib[0]));
    so_resolve(mod, default_dynlib, sizeof(default_dynlib), 0);
}
