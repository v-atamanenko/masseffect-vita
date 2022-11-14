#include <stdbool.h>

void patch__cheats__init() {
    if (cheat_noclip) {
        bool s_DebugWalkThroughWalls = true; // 00a97d08
        kuKernelCpuUnrestrictedMemcpy((void *) (so_mod.text_base + 0x00a97d08), &s_DebugWalkThroughWalls,
                                      sizeof(s_DebugWalkThroughWalls));
    }

    if (cheat_infiniteHealth) {
        bool s_DebugDisableHits = true; // 00a97cff
        kuKernelCpuUnrestrictedMemcpy((void *) (so_mod.text_base + 0x00a97cff), &s_DebugDisableHits,
                                      sizeof(s_DebugDisableHits));
    }

    if (cheat_instantKill) {
        bool instantKill = true;
        kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00a988f4), &instantKill,
                                      sizeof(instantKill));
    }

    if (cheat_infiniteAmmo) {
        bool s_DebugInfiniteAmmoEnabled = true; // 00a988c4
        kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00a988c4), &s_DebugInfiniteAmmoEnabled, sizeof(s_DebugInfiniteAmmoEnabled));
    }

    if (fpsLock > 0) {
        kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x00a98d4c), &fpsLock, sizeof(fpsLock));
    }
}
