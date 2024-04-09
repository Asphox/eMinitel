//
// Created by asphox on 4/3/24.
//

#include "WinConnectionTCP.h"

using namespace IHM;

void WinConnectionTCP::reset()
{
    m_last_connection_failed = false;
}

void WinConnectionTCP::update()
{
    bool opened = true;
    ImGui::custom_GlobalBeginPopupModal("TCP configuration", [this,&opened]()
    {
        ImGui::InputTextWithHint(": Address","Address",m_address, sizeof(m_address)-1);
        int tmp = m_port;
        ImGui::InputInt(": Port", &tmp);
        m_port = tmp;
        if(m_last_connection_failed)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
            ImGui::Text("Error: cannot connect !");
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::LabelText("","");
        }
        ImGui::Separator();
        if(ImGui::Button("Connect"))
        {
            if(m_on_connect)
            {
                // if connection failed => display error message
                if(!m_on_connect())
                    m_last_connection_failed = true;
                else
                    ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
    },&opened, ImGuiWindowFlags_AlwaysAutoResize);
    if(!opened)
    {
        reset();
    }
}