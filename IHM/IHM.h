//
// Created by asphox on 4/3/24.
//

#ifndef MINITELVIEWER_IHM_H
#define MINITELVIEWER_IHM_H

#include "imgui/imgui.h"
#include <functional>
class eMinitel;

namespace IHM
{
    class IHM
    {
    protected:
        eMinitel& m_minitel;
    public:
        IHM(eMinitel& minitel) : m_minitel(minitel){}
        virtual void update() = 0;
    };

    bool isModalOpened();
    void reset();
}

namespace ImGui
{
    // call to mark popup as open (don't call every frame!)
    // Unrelated to imgui's ID stack
    IMGUI_API void  custom_GlobalOpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);

    // return true if the modal is open, and you can start outputting to it.
    // Unrelated to imgui's ID stack
    IMGUI_API void  custom_GlobalBeginPopupModal(const char* name, const std::function<void()>& onBegin, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
}

#endif //MINITELVIEWER_IHM_H
