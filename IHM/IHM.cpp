//
// Created by asphox on 4/3/24.
//

#include "IHM.h"
#include "imgui/imgui_internal.h"
#include <string>

static bool s_modal_opened = false;

bool IHM::isModalOpened()
{
    return s_modal_opened;
}

void IHM::reset()
{
    s_modal_opened = false;
}

// call to mark popup as open (don't call every frame!)
// Unrelated to imgui's ID stack
void ImGui::custom_GlobalOpenPopup(const char *str_id, ImGuiPopupFlags popup_flags)
{
    ImGuiID popup_id = ImHashStr( str_id);
    ImGui::PushOverrideID( popup_id );
    ImGui::OpenPopup(str_id, popup_flags);
    ImGui::PopID();
}

// return true if the modal is open, and you can start outputting to it.
// Unrelated to imgui's ID stack
void  ImGui::custom_GlobalBeginPopupModal(const char* name, const std::function<void()>& onBegin, bool* p_open, ImGuiWindowFlags flags)
{
    ImGuiID popup_id = ImHashStr(name);
    ImGui::PushOverrideID( popup_id );
    bool is_open = false;
    if( ImGui::BeginPopupModal( name, p_open, flags) ) {
        is_open = true;
        s_modal_opened = true;
        onBegin();
        if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            ImGui::CloseCurrentPopup();
            s_modal_opened = false;
        }
        ImGui::EndPopup();
    }
    if(is_open && s_modal_opened && p_open && !*p_open)
        s_modal_opened = false;
    ImGui::PopID();
}