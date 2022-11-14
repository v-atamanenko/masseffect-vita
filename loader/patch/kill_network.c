void * retthis(void* this){
    return this;
}

void patch__kill_network__init() {
    // 1.39
    //hook_addr((uintptr_t)so_mod.text_base + 0x0079e658, (uintptr_t)&retthis); // EA::SP::Origin::OriginMinimizedDialogState::OriginMinimizedDialogState
    // 1.58
    hook_addr((uintptr_t)so_mod.text_base + 0x007b6ee8, (uintptr_t)&retthis); // EA::SP::Origin::OriginMinimizedDialogState::OriginMinimizedDialogState

    // 1.39
    //hook_addr((uintptr_t)so_mod.text_base + 0x006b7988, (uintptr_t)&ret0); // EA::SP::Origin::OriginImpl::ShowOriginUI
    // 1.58
    hook_addr((uintptr_t)so_mod.text_base + 0x006d021c, (uintptr_t)&ret0); // EA::SP::Origin::OriginImpl::ShowOriginUI

    hook_addr((uintptr_t)so_mod.text_base + 0x00622ba8, (uintptr_t)&ret0); // ProtoSSLConnect
    hook_addr((uintptr_t)so_mod.text_base + 0x00626c34, (uintptr_t)&retminus1); // SocketCreate
    hook_addr((uintptr_t)so_mod.text_base + 0x00627c58, (uintptr_t)&retminus1); // SocketControl
    hook_addr((uintptr_t)so_mod.text_base + 0x0040e31c, (uintptr_t)&ret1); //  EA::Util::InstallWallpaper
    hook_addr((uintptr_t)so_mod.text_base + 0x0027c244, (uintptr_t)&ret0); // masseffect_widgets::GalaxyAtWarWidget::OnDraw()
    hook_addr((uintptr_t)so_mod.text_base + 0x0027e96c, (uintptr_t)&ret0); // widgets::UIWidget::CreateInstance<masseffect_widgets::GalaxyAtWarWidget>
}
