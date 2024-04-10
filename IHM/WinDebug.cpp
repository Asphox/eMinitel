//
// Created by Asphox on 10/04/2024.
//

#include "WinDebug.h"
#include "../eMinitel.h"

using namespace IHM;

void WinDebug::update()
{
    if(m_visible)
    {
        bool open = true;
        ImGui::Begin("Debug", &open);
        bool stop_exec = m_minitel.getMinitelCore().debug_is_execution_stopped();
        ImGui::Checkbox("Stop execution", &stop_exec);
        m_minitel.getMinitelCore().debug_stop_execution(stop_exec);
        ImGui::SameLine();
        if(!stop_exec)
            ImGui::BeginDisabled();
        if(ImGui::Button("Next step"))
            m_minitel.getMinitelCore().debug_next_step();
        if(!stop_exec)
            ImGui::EndDisabled();
        ImGui::End();
        if(!open)
        {
            m_visible = false;
            m_minitel.getMinitelCore().debug_stop_execution(false);
        }
    }
}