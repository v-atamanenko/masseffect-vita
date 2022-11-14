#include <stdio.h>
#include <string.h>
#include "settings.h"

#define CONFIG_FILE_PATH DATA_PATH"config.txt"

float aimSensitivity;
float leftStickDeadZone;
float rightStickDeadZone;
int fpsLock;

bool cheat_infiniteAmmo;
bool cheat_infiniteHealth;
bool cheat_instantKill;
bool cheat_noclip;

void resetSettings() {
    aimSensitivity = 1.66f;
    leftStickDeadZone = 0.13f;
    rightStickDeadZone = 0.12f;
    fpsLock = 0;

    cheat_infiniteAmmo = false;
    cheat_infiniteHealth = false;
    cheat_instantKill = false;
    cheat_noclip = false;
}

void loadSettings(void) {
    resetSettings();

    char buffer[30];
    int value;

    FILE *config = fopen(CONFIG_FILE_PATH, "r");

    if (config) {
        while (EOF != fscanf(config, "%[^ ] %d\n", buffer, &value)) {
            if (strcmp("aimSensitivity", buffer) == 0) aimSensitivity = ((float)value / 100.f);
            else if (strcmp("leftStickDeadZone", buffer) == 0) leftStickDeadZone = ((float)value / 100.f);
            else if (strcmp("rightStickDeadZone", buffer) == 0) rightStickDeadZone = ((float)value / 100.f);

            else if (strcmp("fpsLock", buffer) == 0) fpsLock = value;
            else if (strcmp("cheat_infiniteAmmo", buffer) == 0) cheat_infiniteAmmo = (bool)value;
            else if (strcmp("cheat_infiniteHealth", buffer) == 0) cheat_infiniteHealth = (bool)value;
            else if (strcmp("cheat_instantKill", buffer) == 0) cheat_instantKill = (bool)value;
            else if (strcmp("cheat_noclip", buffer) == 0) cheat_noclip = (bool)value;
        }
        fclose(config);
    }
}
