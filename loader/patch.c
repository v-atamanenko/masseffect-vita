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
#include "reimpl/mem.h"

#include <stdio.h>
#include <so_util.h>
#include <main.h>
#include <kubridge.h>
#include <psp2/kernel/threadmgr.h>
#include <string.h>
#include <stdbool.h>

int (*AppendInternalStorageList)(void*); // 0x00350850
int (*AppendPrimaryExternalStorageList)(void*); //00350ad4
int (*AppendSecondaryExternalStorageList)(void*); // 003504d4
int (*AppendSharedStorageList)(void*); // 0034f558
int (*AppendMountedStorageList)(void *param_1,void* param_2,void* param_3,void *param_4); // 0035152c
void* sDedicatedDirectoryName; // 00a88784
void * sCustomExternalStorageDirectories; // 00a88b94

int * waitfororient;

int * _ZN2EA12GameSkeleton18waitForOrientationE;
int * _ZL15creatingCounter;

int GetAllStorageList(void *this,void *param_1,uint param_2) {
    fprintf(stderr, "GRAND ABACUS!!!! waitfororient %i ///\n", *waitfororient);
    //*waitfororient = 0;
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar4 = *(int *)(this + 4);
    iVar1 = *(int *)this;
    if (*(int *)this != iVar4) {
        do {
            iVar2 = *(int *)(iVar1 + 0x90);
            if (((1 < *(int *)(iVar1 + 0x98) - iVar2) && (iVar2 != 0)) &&
                (iVar2 != *(int *)(iVar1 + 0xa4))) {
                void (*func)(int*, int);
                func = ((**(int **)(iVar1 + 0x9c) + 0x10));
                func(*(int **)(iVar1 + 0x9c),iVar2);
            }
            iVar2 = *(int *)(iVar1 + 0x18);
            iVar3 = iVar1 + 0x108;
            param_2 = *(int *)(iVar1 + 0x20) - iVar2;
            if (((1 < (int)param_2) && (iVar2 != 0)) && (iVar2 != *(int *)(iVar1 + 0x2c))) {
                void (*func)(int*, int);
                func = ((**(int **)(iVar1 + 0x24) + 0x10));
                func(*(int **)(iVar1 + 0x24),iVar2);
            }
            iVar1 = iVar3;
        } while (iVar4 != iVar3);
        iVar4 = *(int *)this;
    }
    *(int *)(this + 4) = iVar4;
    if ((((((uint)param_1 & 1) == 0) ||
          (iVar1 = AppendInternalStorageList((void *)this), iVar1 != 0)) &&
         (((((uint)param_1 & 2) == 0 ||
            (iVar1 = AppendPrimaryExternalStorageList((void *)this), iVar1 != 0)) &&
           ((((uint)param_1 & 4) == 0 ||
             (iVar1 = AppendSecondaryExternalStorageList((void *)this), iVar1 != 0)))))) &&
        ((((uint)param_1 & 0x10) == 0 || (iVar1 = AppendSharedStorageList((void *)this), iVar1 != 0))
        )) {
        if (((uint)param_1 & 8) == 0) {
            fprintf(stderr, "ret1, 1\n");
            return 1;
        }
        iVar1 = AppendMountedStorageList
                (sCustomExternalStorageDirectories,this,param_2,
                 (void *)&sDedicatedDirectoryName);
        if (iVar1 != 0) {
            iVar1 = 1;
        }
        fprintf(stderr, "ret ivar1(%i)\n", iVar1);
        return iVar1;
    }
    fprintf(stderr, "ret0, 1\n");
    return 0;
}

void sync_synchronize() {
    __sync_synchronize();
}

void * retthis(void* this){
    return this;
}

so_hook futunlock;

void EA__Thread__Futex__Unlock(void *this) {
    if (!this) {
        fprintf(stderr,"EA__Thread__Futex__UnlockX this NULL!!!\n");
        return;
    } else {
        //fprintf(stderr,"EA__Thread__Futex__UnlockX this not null but %i!!!\n", *(int *)(this + 4));
    }
    SO_CONTINUE(void*, futunlock, this);
}




void so_patch(void) {
    /*AppendInternalStorageList  = (uintptr_t)so_mod.text_base + 0x00350850;
    AppendPrimaryExternalStorageList = (uintptr_t)so_mod.text_base + 0x00350ad4;
    AppendSecondaryExternalStorageList = (uintptr_t)so_mod.text_base + 0x003504d4;
    AppendSharedStorageList = (uintptr_t)so_mod.text_base + 0x0034f558;
    AppendMountedStorageList = (uintptr_t)so_mod.text_base + 0x0035152c;
    sDedicatedDirectoryName = (uintptr_t)so_mod.text_base + 0x00a88784;
    sCustomExternalStorageDirectories = (uintptr_t)so_mod.text_base + 0x00a88b94;
    waitfororient = (uintptr_t)so_mod.text_base + 0x00a931a8;*/

    _ZN2EA12GameSkeleton18waitForOrientationE = (uintptr_t)so_mod.text_base + 0x00a931a8;
    _ZL15creatingCounter = (uintptr_t)so_mod.text_base + 0x00a80108;

    hook_addr((uintptr_t)so_mod.text_base + 0x0009e630, (uintptr_t)&sync_synchronize);
    hook_addr((uintptr_t)so_mod.text_base + 0x0079e658, (uintptr_t)&retthis);
    hook_addr((uintptr_t)so_mod.text_base + 0x006b7988, (uintptr_t)&ret0);

    //futunlock = hook_addr((uintptr_t)so_mod.text_base + 0x0034202c, (uintptr_t)&EA__Thread__Futex__Unlock);
    //hook_addr((uintptr_t)so_mod.text_base + 0x00351858, (uintptr_t)&GetAllStorageList);

   /* //hook_addr(so_symbol(&so_mod, "_ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE"), (uintptr_t)_ZN2EA4StdC10ScanfLocal12ReadFormat16EPKwPNS1_10FormatDataE);
    operator_new = (uintptr_t)so_mod.text_base + 0x00340840;
    gEmptyString = (uintptr_t)so_mod.text_base + 0x00a88c88;
    //MMapAllocInternal = (uintptr_t)so_mod.text_base + 0x0036a050;
    MMapAllocInternal = (uintptr_t)&MMapAllocInternal_fake;

    hook_addr((uintptr_t)so_mod.text_base + 0x0036a0b8, (uintptr_t)&EA__Allocator__GeneralAllocator__MMapMalloc);
    hook_addr((uintptr_t)so_mod.text_base + 0x0036a050, (uintptr_t)&MMapAllocInternal_fake);


    hook_addr((uintptr_t)so_mod.text_base + 0x000a8ca8, (uintptr_t)&_ZN5eastl12basic_stringIcNS_9allocatorEE15RangeInitializeEPKcS4_);


    AddCoreInternal = hook_addr((uintptr_t)so_mod.text_base + 0x0036a258, (uintptr_t)&EA__Allocator__GeneralAllocator__AddCoreInternal);
*/
    uint32_t nop = 0xe1a00000;
    kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0079e6c8), &nop, sizeof(nop));
    kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x0079e6d4), &nop, sizeof(nop));

    //kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00384274), &nop, sizeof(nop));
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
