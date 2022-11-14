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

#include <kubridge.h>
#include <main.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <so_util.h>
#include <stdio.h>
#include <string.h>

#include "utils/glutil.h"
#include "utils/logger.h"
#include "utils/settings.h"

int * waitForOrientation; // Flag that prevents application from booting, handled in main.c.
extern volatile int deserializationBugCaught; // Flag that shows that the known deserialization bug happened.
extern volatile int wantQuit; // Flag to tell the main loop to quit.

void sync_synchronize() {
    __sync_synchronize();
}

void onExit(void* x ) {
    wantQuit = 1;
}

#include "patch/output_streams.c"
#include "patch/aim_sensitivity.c"
#include "patch/deserialization_bug.c"
#include "patch/info_menu.c"
#include "patch/kill_network.c"
#include "patch/cheats.c"

void so_patch(void) {
    // Really quit the app when onExit is called:
    hook_addr((uintptr_t)so_mod.text_base + 0x00287fc4, (uintptr_t)&onExit);

    // Flag that prevents application from booting, handled in main.c.
    // 1.39 waitForOrientation = (uintptr_t)so_mod.text_base + 0x00a931a8;
    waitForOrientation = (uintptr_t)so_mod.text_base + 0x00aabd90; // 1.58

    // Unresolvable symbol __sync_synchronize
    // 1.39 hook_addr((uintptr_t)so_mod.text_base + 0x0009e630, (uintptr_t)&sync_synchronize);
    hook_addr((uintptr_t)so_mod.text_base + 0x0009ef58, (uintptr_t)&sync_synchronize); // 1.58

    // Other patches
    patch__info_menu__init();
    patch__output_streams__init();
    patch__aim_sensitivity__init();
    patch__deserialization_bug__init();
    patch__kill_network__init();
    patch__cheats__init();
}
