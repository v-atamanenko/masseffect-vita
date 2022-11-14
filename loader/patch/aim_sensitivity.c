typedef struct __attribute__((__packed__)) Hud {
    void* super_IEventListener;
    void * game_world;
    void * field2_0x8;
    void * field3_0xc;
    void * inputForwarderTaps;
    void * fielddummy_1;
    void * field9_0x18;
    char fielddummy_2[352];
    void * inputSchemeBasic;
    char fielddummy_3[184];
    void * field547_0x238;
    void * field548_0x23c;
    void * field549_0x240;
    void * field553_0x244;
    void * field554_0x248;
    void * field555_0x24c;
    void * field556_0x250;
    void * field557_0x254;
    void * field558_0x258;
    void * field559_0x25c;
    void * field560_0x260;
    void * field561_0x264;
    void * field562_0x268;
    void * hudFadeTImeInverse;
    char fieldDUmmy3[248];
    void * hermiteFilter;
    char fieldDummy4[92];
    int state_mb;
    char fieldDummy5[220];
    char cameraXisMoving; /* Created by retype action */
    char cameraYisMoving; /* Created by retype action */
    char field765_0x4aa;
    char field766_0x4ab;
    float cameraXaccel;
    float cameraYaccel;
    float lastRx;
    float lastRy;
    char fieldDummy6[824];
} Hud;

int (*Playable__GetBiotic)(void*,int);
void* (*__dynamic_cast)(void *,void *,void *,int);
int (*BioticLeash__OnSwipe)(void*,float);
int (*Settings__GetInstance)();
int (*Playable__CanChangeFacing)(void*);
int (*Playable__IsInCoverProne)(void*);
void * Biotic__typeinfo;
void * BioticLeash__typeinfo;

void Hud__moveCamera(Hud *this, float rx, float ry) {
    void *pPVar2;
    int iVar3;
    void *this_00;
    void *pPVar1;
    float invertYaxisMultiplier;

    if ((((*(char *)((int)this->game_world + 0x57c) == '\0') &&
          (pPVar2 = *(void **)((int)this->game_world + 0x1c4), pPVar2 != (void *)0x0)) &&
         (iVar3 = Playable__GetBiotic(pPVar2,0x52), iVar3 != 0)) &&
        (this_00 = (void *)__dynamic_cast((void*)iVar3,&Biotic__typeinfo,&BioticLeash__typeinfo,0),
                this_00 != (void *)0x0)) {
        if (rx <= -1.0) {
            iVar3 = BioticLeash__OnSwipe(this_00,-1.5f);
        }
        else {
            if (rx < 1.0) goto moveCamera_mainBody;
            iVar3 = BioticLeash__OnSwipe(this_00,1.5f);
        }
        if (iVar3 != 0) {
            return;
        }
    }
    moveCamera_mainBody:
    iVar3 = Settings__GetInstance();
    invertYaxisMultiplier = 1.0f;
    if (*(char *)(iVar3 + 0x60) != '\0') {
        invertYaxisMultiplier = -1.0f;
    }
    if (this->game_world == (void *)0x0) {
        return;
    }
    pPVar2 = *(void **)((int)this->game_world + 0x1c4);
    if (pPVar2 == (void *)0x0) {
        return;
    }
    iVar3 = Playable__CanChangeFacing(pPVar2);
    if (iVar3 == 0) {
        return;
    }
    if (*(char *)((int)this->game_world + 0x57c) != '\0') {
        return;
    }
    pPVar1 = *(void **)((int)this->game_world + 0x1c4);
    if ((*(int *)((int)pPVar1 + 0x5b8) < 1) &&
        (iVar3 = Playable__IsInCoverProne((void *)pPVar1), iVar3 == 0)) {
        if (this->state_mb == 1) {
            if (-1.0 < ry) {
                if ((ry < -0.1) && (ry - this->lastRy < 0.0)) {
                    this->cameraYisMoving = 1;
                    this->cameraYaccel = -0.4f;
                }
            }
            else if (ry - this->lastRy < 0.0) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = -3.2f;
            }
            if (ry < 1.0) {
                if ((0.1 <= ry) && (0.0 <= ry - this->lastRy)) {
                    this->cameraYisMoving = 1;
                    this->cameraYaccel = 0.4f;
                }
            }
            else if (0.0 <= ry - this->lastRy) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = 3.2f;
            }
            if (rx < 1.0) {
                if ((0.1 <= rx) && (0.0 <= rx - this->lastRx)) {
                    this->cameraXisMoving = 1;
                    this->cameraXaccel = -0.008f;
                }
            }
            else if (0.0 <= rx - this->lastRx) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = -0.064f;
            }
            if (-1.0 < rx) {
                if (-0.1 <= rx) goto moveCamera_done;
                goto moveCamera_Rx0008;
            }
        }
        else {
            if (((ry < -0.1) && (-0.4 <= ry)) && (ry - this->lastRy < 0.0)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = -0.4f;
            }
            if (((ry < -0.4) && (-0.9 <= ry)) && (ry - this->lastRy < 0.0)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = -0.4f;
            }
            if ((ry < -0.9) && (ry - this->lastRy < 0.0)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = -3.2f;
            }
            if (((0.1 <= ry) && (ry < 0.4)) && (0.0 <= ry - this->lastRy)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = 0.4f;
            }
            if (((0.4 <= ry) && (ry < 0.9)) && (0.0 <= ry - this->lastRy)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = 0.4f;
            }
            if ((0.9 <= ry) && (0.0 <= ry - this->lastRy)) {
                this->cameraYisMoving = 1;
                this->cameraYaccel = 3.2f;
            }
            if (((0.1 <= rx) && (rx < 0.4)) && (0.0 <= rx - this->lastRx)) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = -0.008f;
            }
            if (((0.4 <= rx) && (rx < 0.9)) && (0.0 <= rx - this->lastRx)) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = -0.008f;
            }
            if ((0.9 <= rx) && (0.0 <= rx - this->lastRx)) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = -0.064f;
            }
            if (((rx < -0.1) && (-0.4 <= rx)) && (rx - this->lastRx < 0.0)) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = 0.008f;
            }
            if (((rx < -0.4) && (-0.9 <= rx)) && (rx - this->lastRx < 0.0)) {
                this->cameraXisMoving = 1;
                this->cameraXaccel = 0.008f;
            }
            if (-0.9 <= rx) goto moveCamera_done;
        }
        if (rx - this->lastRx < 0.0) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = 0.064f;
        }
    }
    else {
        if ((ry < -0.1) && ((-0.4 <= ry && (ry - this->lastRy < 0.0)))) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = -0.05f * aimSensitivity;
        }
        if (((ry < -0.4) && (-0.9 <= ry)) && (ry - this->lastRy < 0.0)) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = -0.05f * aimSensitivity;
        }
        if ((ry < -0.9) && (ry - this->lastRy < 0.0)) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = -0.4f * aimSensitivity;
        }
        if (((0.1 <= ry) && (ry < 0.4)) && (0.0 <= ry - this->lastRy)) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = 0.05f * aimSensitivity;
        }
        if (((0.4 <= ry) && (ry < 0.9)) && (0.0 <= ry - this->lastRy)) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = 0.05f * aimSensitivity;
        }
        if ((0.9 <= ry) && (0.0 <= ry - this->lastRy)) {
            this->cameraYisMoving = 1;
            this->cameraYaccel = 0.4f * aimSensitivity;
        }
        if (((0.1 <= rx) && (rx < 0.4)) && (0.0 <= rx - this->lastRx)) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = -0.001f * aimSensitivity;
        }
        if (((0.4 <= rx) && (rx < 0.9)) && (0.0 <= rx - this->lastRx)) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = -0.001f * aimSensitivity;
        }
        if ((0.9 <= rx) && (0.0 <= rx - this->lastRx)) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = -0.008f * aimSensitivity;
        }
        if (((rx < -0.1) && (-0.4 <= rx)) && (rx - this->lastRx < 0.0)) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = 0.001f * aimSensitivity;
        }
        if (((rx < -0.4) && (-0.9 <= rx)) && (rx - this->lastRx < 0.0)) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = 0.001f * aimSensitivity;
        }
        if (-0.9 <= rx) goto moveCamera_done;
        moveCamera_Rx0008:
        if (rx - this->lastRx < 0.0) {
            this->cameraXisMoving = 1;
            this->cameraXaccel = 0.008f * aimSensitivity;
        }
    }
    moveCamera_done:
    this->lastRy = ry;
    this->lastRx = rx;
    this->cameraYaccel = this->cameraYaccel * invertYaxisMultiplier;
}

void patch__aim_sensitivity__init() {
    Playable__GetBiotic = (uintptr_t)so_mod.text_base + 0x001f5b50;
    __dynamic_cast = (uintptr_t)so_mod.text_base + 0x0090741c;
    BioticLeash__OnSwipe = (uintptr_t)so_mod.text_base + 0x000a7694;
    Settings__GetInstance = (uintptr_t)so_mod.text_base + 0x0023fffc;
    Playable__CanChangeFacing = (uintptr_t)so_mod.text_base + 0x001f9724;
    Playable__IsInCoverProne = (uintptr_t)so_mod.text_base + 0x001f6818;
    Biotic__typeinfo = (uintptr_t)so_mod.text_base + 0x00a3b3d;
    BioticLeash__typeinfo = (uintptr_t)so_mod.text_base + 0x00a3c754;
    hook_addr((uintptr_t)so_mod.text_base + 0x0010e620, (uintptr_t)&Hud__moveCamera );
}
