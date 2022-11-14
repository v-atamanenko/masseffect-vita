/*
 * Fix crash on INFO => ABOUT menu open
 */
so_hook im__scene2d_new__Node__SetVisible_hook;
void im__scene2d_new__Node__SetVisible(void *this,bool param_1) {
    if (!this) {
        log_error("im__scene2d_new__Node__SetVisible: null");
        return;
    }

    SO_CONTINUE(void*,im__scene2d_new__Node__SetVisible_hook, this, param_1 );
}

void patch__info_menu__init() {
    // Kill INFO=>HELP
    hook_addr((uintptr_t)so_mod.text_base + 0x00284a18, (uintptr_t)&ret0);

    im__scene2d_new__Node__SetVisible_hook  = hook_addr((uintptr_t)so_mod.text_base + 0x00488ff8, (uintptr_t)&im__scene2d_new__Node__SetVisible );
}
