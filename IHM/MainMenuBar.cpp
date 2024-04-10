//
// Created by asphox on 4/3/24.
//
#include "MainMenuBar.h"
using namespace IHM;

#include "../eMinitel.h"

void MainMenuBar::update()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Connection##AboutMenuBar"))
        {
            if(!m_minitel.isConnected())
            {
                if(ImGui::MenuItem("TCP"))
                {
                    ImGui::custom_GlobalOpenPopup("TCP configuration");
                }
            }
            if(ImGui::MenuItem("Disconnect",nullptr, false, m_minitel.isConnected()))
            {
                if(m_on_disconnect)
                    m_on_disconnect();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Controls##AboutMenuBar"))
        {
            if(ImGui::BeginMenu("Functions keys"))
            {
                if(ImGui::MenuItem("Send / Envoie", "ENTER"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_SEND;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                if(ImGui::MenuItem("Previous / Retour", "F2"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_RETURN;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                if(ImGui::MenuItem("Next / Suite", "F3"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_NEXT;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                if(ImGui::MenuItem("Guide", "F1"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_GUIDE;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                if(ImGui::MenuItem("Summary / Sommaire", "TBD"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_SUMMARY;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                if(ImGui::MenuItem("Repeat / Répétition","F5"))
                {
                    mtlc_KeyEvent ke;
                    ke.kc = KC_REPETITION;
                    m_minitel.getMinitelCore().key_event(ke);
                }
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Reset"))
            {
                if(m_on_reset)
                    m_on_reset();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Options##AboutMenuBar"))
        {
            if(ImGui::MenuItem("Debug##DebugMenuBar"))
            {
                m_minitel.m_ihm_win_debug.show(true);
            }
            bool bCheck = m_minitel.colorMode();
            ImGui::Checkbox("Color mode",&bCheck);
            m_minitel.setColorMode(bCheck);
            ImGui::EndMenu();
        }
//        if(ImGui::BeginMenu("Debug"))
//        {
//            if(ImGui::MenuItem("Log##MenuLog"))
//            {
//                if(m_on_log)
//                    m_on_log();
//            }
//            ImGui::EndMenu();
//        }
        if(ImGui::Button("About"))
        {
            ImGui::custom_GlobalOpenPopup("About eMinitel");
        }
        ImGui::EndMainMenuBar();
    }
}