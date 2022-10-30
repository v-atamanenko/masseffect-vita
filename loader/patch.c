/*
 * patch_game.c
 *
 * Patching some of the .so internal functions or bridging them to native for
 * better compatibility.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "patch.h"

#include <stdio.h>
#include <so_util.h>
#include <main.h>
#include <kubridge.h>
#include <psp2/kernel/threadmgr.h>

volatile int sus_thread_count = 0;
so_hook susthread_hook;

int SusThread(void* arg) {

    sus_thread_count++;
    sceKernelDelayThread(sus_thread_count * 4 * 1000 * 1000); // delay 4xThreadNum seconds
    SO_CONTINUE(int, susthread_hook, arg);
}

ushort * _ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE(wchar_t *param_1,void *param_2) {
    printf("_ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE\n");
    return 0;
}

void so_patch(void) {
    //hook_addr(so_symbol(&so_mod, "_ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE"), (uintptr_t)_ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE);

    uint32_t nop = 0xe1a00000;
    //kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00384274), &nop, sizeof(nop));
    //kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00382934), &nop, sizeof(nop));
    //kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00382938), &nop, sizeof(nop));
    //kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0038293c), &nop, sizeof(nop));

    /*
    // Always fail check for "appbundle:" in filename ==> don't use JNI IO funcs.
    uint32_t fix = 0xea000007;
    kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0022bf6c), &fix, sizeof(fix));
    kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0022bbe8), &fix, sizeof(fix));
    uint32_t nop = 0xe1a00000;
    kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0022b214), &nop, sizeof(nop));

    // Sus thread
    // A thread that causes some kind of undefined behaviour and crashes in different places if not delayed
    susthread_hook = hook_addr((uintptr_t)so_mod.text_base + 0x00320624, (uintptr_t)&SusThread);*/
}
