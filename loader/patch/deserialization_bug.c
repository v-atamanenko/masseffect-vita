so_hook GetUnstructuredObjectField_hook;
so_hook AddUnstructuredObjectField_hook;
so_hook RemoveUnstructuredObjectField_hook;

int GetUnstructuredObjectField(void *this, int *param_1, int param_2, void **param_3) {
    if (!param_1) {
        deserializationBugCaught = 1;
        log_error("GetUnstructuredObjectField: NULL");
        sceKernelDelayThread(3000000);
        for (int i = 0; i < 3; ++i) {
            drawFakeLoadingScreen();
            gl_swap();
        }

        char argp[512];
        sprintf(argp, "-silent");
        sceAppMgrLoadExec("app0:/eboot.bin", (char * const*)((const char*[]){argp, 0}), NULL);
        while(1) sceKernelDelayThread(10000);
    }
    return SO_CONTINUE(int, GetUnstructuredObjectField_hook, this, param_1, param_2, param_3);
}

void* AddUnstructuredObjectField(void *this,int *param_1,int param_2,void *param_3) {
    if (!param_1) {
        deserializationBugCaught = 1;
        log_error("AddUnstructuredObjectField: NULL");
        sceKernelDelayThread(3000000);
        for (int i = 0; i < 3; ++i) {
            drawFakeLoadingScreen();
            gl_swap();
        }

        char argp[512];
        sprintf(argp, "-silent");
        sceAppMgrLoadExec("app0:/eboot.bin", (char * const*)((const char*[]){argp, 0}), NULL);
        while(1) sceKernelDelayThread(10000);
    }
    return SO_CONTINUE(void*, AddUnstructuredObjectField_hook, this, param_1, param_2, param_3);
}

void RemoveUnstructuredObjectField(void *this,int *param_1,void *param_2) {
    if (!param_1) {
        log_error("RemoveUnstructuredObjectField: NULL");
        deserializationBugCaught = 1;
        sceKernelDelayThread(3000000);
        for (int i = 0; i < 3; ++i) {
            drawFakeLoadingScreen();
            gl_swap();
        }

        char argp[512];
        sprintf(argp, "-silent");
        sceAppMgrLoadExec("app0:/eboot.bin", (char * const*)((const char*[]){argp, 0}), NULL);
        while(1) sceKernelDelayThread(10000);
    }
    SO_CONTINUE(void*, RemoveUnstructuredObjectField_hook, this, param_1, param_2);
}

void patch__deserialization_bug__init() {
    GetUnstructuredObjectField_hook = hook_addr((uintptr_t)so_mod.text_base + 0x003b7c78, (uintptr_t)&GetUnstructuredObjectField);
    AddUnstructuredObjectField_hook = hook_addr((uintptr_t)so_mod.text_base + 0x003bb158, (uintptr_t)&AddUnstructuredObjectField);
    RemoveUnstructuredObjectField_hook = hook_addr((uintptr_t)so_mod.text_base + 0x003b620c, (uintptr_t)&RemoveUnstructuredObjectField);
}
