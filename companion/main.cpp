#include <vitasdk.h>
#include <vitaGL.h>
#include <imgui_vita.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include <string>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <dirent.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

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

inline int8_t is_dir(char* p) {
    DIR* filetest = opendir(p);
    if (filetest != NULL) {
        closedir(filetest);
        return 1;
    }
    return 0;
}

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate ms
    return milliseconds;
}

int cp(const char *to, const char *from)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

    out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

void new_game_with_all_unlocked() {
    if (is_dir(DATA_PATH"assets/var")) {
        char savesbpfolder[1024];
        snprintf(savesbpfolder, 1024, DATA_PATH"assets/saves-backup-%lli/", current_timestamp());
        mkdir(savesbpfolder, 0777);

        struct dirent *dir;
        DIR *d = opendir(DATA_PATH"assets/var");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                char realpath[1024];
                snprintf(realpath, 1024, DATA_PATH"assets/var/%s", dir->d_name);
                char targetpath[1024];
                snprintf(targetpath, 1024, "%s/%s", savesbpfolder, dir->d_name);

                if (!is_dir(realpath)) {
                    rename(realpath, targetpath);
                    printf("Moved %s\n", dir->d_name);
                }
            }
            closedir(d);
        } else {
            mkdir(DATA_PATH"assets/var", 0777);
        }

        cp(DATA_PATH"assets/var/0_tutorials.sb", "app0:/data/everything_unlocked_start/var/0_tutorials.sb");
        cp(DATA_PATH"assets/var/career_0.sb", "app0:/data/everything_unlocked_start/var/career_0.sb");
        cp(DATA_PATH"assets/var/cp_0_01_snow_planet_checkpoint_01.sb", "app0:/data/everything_unlocked_start/var/cp_0_01_snow_planet_checkpoint_01.sb");
        cp(DATA_PATH"assets/var/cp_0_01_snow_planet_checkpoint_02.sb", "app0:/data/everything_unlocked_start/var/cp_0_01_snow_planet_checkpoint_02.sb");
        cp(DATA_PATH"assets/var/cp_0_01_snow_planet_checkpoint_02_end.sb", "app0:/data/everything_unlocked_start/var/cp_0_01_snow_planet_checkpoint_02_end.sb");
        cp(DATA_PATH"assets/var/filesystem.sb", "app0:/data/everything_unlocked_start/var/filesystem.sb");
        cp(DATA_PATH"assets/var/settings.sb", "app0:/data/everything_unlocked_start/var/settings.sb");

        char readmepath[1024];
        snprintf(readmepath, 1024, "%s/readme.txt", savesbpfolder);

        cp(readmepath, "app0:/data/readme-saves.txt");
    }
}

void loadConfig(void) {
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

void saveConfig(void) {
    FILE *config = fopen(CONFIG_FILE_PATH, "w+");

    if (config) {
        fprintf(config, "%s %d\n", "aimSensitivity", (int)(aimSensitivity * 100.f));
        fprintf(config, "%s %d\n", "leftStickDeadZone", (int)(leftStickDeadZone * 100.f));
        fprintf(config, "%s %d\n", "rightStickDeadZone", (int)(rightStickDeadZone * 100.f));
        fprintf(config, "%s %d\n", "fpsLock", (int)fpsLock);
        fprintf(config, "%s %d\n", "cheat_infiniteAmmo", (int)cheat_infiniteAmmo);
        fprintf(config, "%s %d\n", "cheat_infiniteHealth", (int)cheat_infiniteHealth);
        fprintf(config, "%s %d\n", "cheat_instantKill", (int)cheat_instantKill);
        fprintf(config, "%s %d\n", "cheat_noclip", (int)cheat_noclip);
        fclose(config);
    }
}

char *options_descs[] = {
        "Increasing this value will allow to move camera faster while aiming.\nThe default value is: 1.66.", // aimSensitivity
        "Deadzone for the left analog stick. Increase if you have stick drift issues.\nThe default value is: 0.13.", // leftStickDeadZone
        "Deadzone for the right analog stick. Increase if you have stick drift issues.\nThe default value is: 0.12.", // rightStickDeadZone
        "If you want to reduce FPS fluctuations and instead stay on a lower but constant level, you may try this.\nThe default value is: None.", // fpsLock
        "Allows you to fire indefinitely without reloads or cooldowns.\nThe default value is: Disabled.", // cheat_infiniteAmmo
        "If enabled, no matter how hard enemies hit you, you won't notice.\nThe default value is: Disabled.", // cheat_infiniteHealth
        "Kill (almost) any enemy in one shot. In a few shots for scripted bosses.\nThe default value is: Disabled.", // cheat_instantKill
        "Gain the power to walk through walls. May cause bugs and glitches!\nThe default value is: Disabled.", // cheat_noclip
};

enum {
    OPT_AIMSENS,
    OPT_DEADZONE_L,
    OPT_DEADZONE_R,
    OPT_FPSLOCK,
    OPT_INFINITE_AMMO,
    OPT_INFINITE_HEALTH,
    OPT_INSTANT_KILL,
    OPT_NOCLIP
};

char *desc = nullptr;

void SetDescription(int i) {
    if (ImGui::IsItemHovered())
        desc = options_descs[i];
}

ImTextureID loadTex(const char* fname) {
    FILE* f = fopen(fname, "r");
    if (!f) return NULL;
    int image_width, image_height, depth;
    unsigned char* data = stbi_load_from_file(f, &image_width, &image_height, &depth, 0);
    fclose(f);

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return reinterpret_cast<ImTextureID>(image_texture);
}

bool FancyButton(const char* label, const ImVec2& pos, const ImVec2& size, ImU32 color_1, ImU32 color_2) {
    bool ret = false;
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_2);
    ImGui::PushStyleColor(ImGuiCol_Border, color_1);
    ImGui::PushStyleColor(ImGuiCol_Text, color_1);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::SetCursorPos(pos);
    if (ImGui::Button(label, size))
        ret = true;
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
    return ret;
}

#include "imgui_internal.h"

namespace ImGui {
    bool SelectableCentered(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg);
}

bool ImGui::SelectableCentered(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet) // FIXME-OPT: Avoid if vertically clipped.
        PopClipRect();

    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrentLineTextBaseOffset;
    ImRect bb(pos, {pos.x + size.x, pos.y + size.y});
    ItemSize(bb);

    // Fill horizontal space.
    ImVec2 window_padding = window->WindowPadding;
    float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
    float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
    ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
    ImRect bb_with_spacing(pos, {pos.x + size_draw.x, pos.y + size_draw.y});
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
        bb_with_spacing.Max.x += window_padding.x;

    // Selectables are tightly packed together, we extend the box to cover spacing between selectable.
    float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
    float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
    float spacing_R = style.ItemSpacing.x - spacing_L;
    float spacing_D = style.ItemSpacing.y - spacing_U;
    bb_with_spacing.Min.x -= spacing_L;
    bb_with_spacing.Min.y -= spacing_U;
    bb_with_spacing.Max.x += spacing_R;
    bb_with_spacing.Max.y += spacing_D;
    if (!ItemAdd(bb_with_spacing, (flags & ImGuiSelectableFlags_Disabled) ? 0 : id))
    {
        if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
            PushColumnClipRect();
        return false;
    }

    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoldingActiveID;
    if (flags & ImGuiSelectableFlags_MenuItem) button_flags |= ImGuiButtonFlags_PressedOnRelease;
    if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
    if (flags & ImGuiSelectableFlags_Disabled)
        selected = false;

    // Hovering selectable with mouse updates NavId accordingly so navigation can be resumed with gamepad/keyboard (this doesn't happen on most widgets)
    if (pressed || hovered)// && (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f))
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerActiveMask)
        {
            g.NavDisableHighlight = true;
            //SetNavID(id, window->DC.NavLayerCurrent);
        }

    // Render
    if (hovered || selected)
    {
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
        RenderNavHighlight(bb_with_spacing, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
    } else {
        const ImU32 col = GetColorU32(ImGuiCol_TitleBgCollapsed);
        RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
    }

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
    {
        PushColumnClipRect();
        bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
    }

    ImVec2 text_pos;
    text_pos.x = bb.Min.x + (((bb.Max.x - bb.Min.x) - (label_size.x)) / 2.f);
    text_pos.y = bb.Min.y + (((bb.Max.y - bb.Min.y) - (label_size.y)) / 2.f);

    if (flags & ImGuiSelectableFlags_Disabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    RenderText(text_pos, label);
    if (flags & ImGuiSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();
    return pressed;
}

int main(int argc, char *argv[]) {
    resetSettings();
    loadConfig();
    int exit_code = 0xDEAD;

    vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_4X);

    ImTextureID bg = loadTex("app0:/data/companion-bg.png");

    ImGui::CreateContext();
    ImGui_ImplVitaGL_Init();
    ImGui_ImplVitaGL_TouchUsage(false);
    ImGui_ImplVitaGL_GamepadUsage(true);
    ImGui::StyleColorsDark();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    ImGui::GetIO().MouseDrawCursor = false;

    while (exit_code == 0xDEAD) {
        desc = nullptr;
        ImGui_ImplVitaGL_NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(960, 544), ImGuiSetCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});

        ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImDrawList* idl = ImGui::GetWindowDrawList();
        idl->AddImage(bg, {0,0}, {960,544});


        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

        ImGui::BeginGroup();

        ImGui::PushItemWidth(385);

        ImGui::SetCursorPos({73, 63});
        ImGui::SliderFloat("##aimSensitivity", &aimSensitivity, 0.75f, 3.5f, "%.2f");
        SetDescription(OPT_AIMSENS);

        ImGui::SetCursorPos({73, 121});
        ImGui::SliderFloat("##leftStickDeadZone", &leftStickDeadZone, 0.01f, 0.5f, "%.2f");
        SetDescription(OPT_DEADZONE_L);

        ImGui::SetCursorPos({73, 181});
        ImGui::SliderFloat("##rightStickDeadZone", &rightStickDeadZone, 0.01f, 0.5f, "%.2f");
        SetDescription(OPT_DEADZONE_R);

        ImGui::PopItemWidth();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, IM_COL32(24, 193, 203, 66));
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(13, 193, 204, 166));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(13, 193, 204, 166));

        {
            ImGui::SetCursorPos({73, 241});
            if (ImGui::SelectableCentered("No Lock", fpsLock == 0, 0, {120,20}))
                fpsLock = 0;
            SetDescription(OPT_FPSLOCK);

            ImGui::SetCursorPos({210, 241});
            if (ImGui::SelectableCentered("45 FPS", fpsLock == 45, 0, {72,20}))
                fpsLock = 45;
            SetDescription(OPT_FPSLOCK);

            ImGui::SetCursorPos({298, 241});
            if (ImGui::SelectableCentered("30 FPS", fpsLock == 30, 0, {72,20}))
                fpsLock = 30;
            SetDescription(OPT_FPSLOCK);

            ImGui::SetCursorPos({386, 241});
            if (ImGui::SelectableCentered("25 FPS", fpsLock == 25, 0, {72,20}))
                fpsLock = 25;
            SetDescription(OPT_FPSLOCK);
        }

        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, IM_COL32(154, 13, 204, 40));
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(166, 13, 204, 166));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(166, 13, 204, 166));

        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {3,10});
            ImGui::SetCursorPos({73, 338});
            if (ImGui::SelectableCentered("Enable##cheat_infiniteAmmo", cheat_infiniteAmmo, 0, {80,20}))
                cheat_infiniteAmmo = true;
            SetDescription(OPT_INFINITE_AMMO);
            ImGui::PopStyleVar(1);

            ImGui::SetCursorPos({157, 338});
            if (ImGui::SelectableCentered("Disable##cheat_infiniteAmmo", !cheat_infiniteAmmo, 0, {80,20}))
                cheat_infiniteAmmo = false;
            SetDescription(OPT_INFINITE_AMMO);
        }

        {
            ImGui::SetCursorPos({278, 338});
            if (ImGui::SelectableCentered("Enable##cheat_infiniteHealth", cheat_infiniteHealth, 0, {80,20}))
                cheat_infiniteHealth = true;
            SetDescription(OPT_INFINITE_HEALTH);

            ImGui::SetCursorPos({362, 338});
            if (ImGui::SelectableCentered("Disable##cheat_infiniteHealth", !cheat_infiniteHealth, 0, {80,20}))
                cheat_infiniteHealth = false;
            SetDescription(OPT_INFINITE_HEALTH);
        }

        {
            ImGui::SetCursorPos({73, 402});
            if (ImGui::SelectableCentered("Enable##cheat_instantKill", cheat_instantKill, 0, {80,20}))
                cheat_instantKill = true;
            SetDescription(OPT_INSTANT_KILL);

            ImGui::SetCursorPos({157, 402});
            if (ImGui::SelectableCentered("Disable##cheat_instantKill", !cheat_instantKill, 0, {80,20}))
                cheat_instantKill = false;
            SetDescription(OPT_INSTANT_KILL);
        }

        {
            ImGui::SetCursorPos({278, 402});
            if (ImGui::SelectableCentered("Enable##cheat_noclip", cheat_noclip, 0, {80,20}))
                cheat_noclip = true;
            SetDescription(OPT_NOCLIP);

            ImGui::SetCursorPos({362, 402});
            if (ImGui::SelectableCentered("Disable##cheat_noclip", !cheat_noclip, 0, {80,20}))
                cheat_noclip = false;
            SetDescription(OPT_NOCLIP);
        }

        ImGui::PopStyleColor(3);

        ImGui::PopStyleVar();

        if (desc) {
            ImGui::SetCursorPos({72, 468});
            ImGui::PushTextWrapPos(464);
            ImGui::Text(desc);
            ImGui::PopTextWrapPos();
        }

        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();

        if (FancyButton("Save and Launch", {523, 251}, {187, 40}, IM_COL32(190,58,237,255), IM_COL32(190,58,237,40))) {
            saveConfig();
            exit_code = 1;
        }

        if (FancyButton("Reset Settings", {724, 251}, {187, 40}, IM_COL32(237,165,58,255), IM_COL32(237,165,58,40))) {
            resetSettings();
        }

        if (FancyButton("New Game With All Unlocked", {525, 345}, {386, 40}, IM_COL32(237,58,101,255), IM_COL32(237,58,101,40))) {
            saveConfig();
            new_game_with_all_unlocked();
            exit_code = 1;
        }

        ImGui::EndGroup();

        ImGui::PopStyleVar();
        ImGui::End();
        ImGui::PopStyleVar(2);

        glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
        ImGui::Render();
        ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());
        vglSwapBuffers(GL_FALSE);
    }

    if (exit_code < 2) // Save
        saveConfig();

    if (exit_code % 2 == 1) // Launch
        sceAppMgrLoadExec("app0:/eboot.bin", NULL, NULL);

    return 0;
}
