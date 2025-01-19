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
                if (m_minitel.m_configuration.m_recent_connections.size() > 0)
                {
                    if (ImGui::BeginMenu("Recent"))
                    {
                        for (const auto& connection : m_minitel.m_configuration.m_recent_connections)
                        {
                            if (ImGui::MenuItem(connection.c_str()))
                            {
                                m_minitel.connect(connection);
                            }
                        }
                        ImGui::EndMenu();
                    }
                }
                if(ImGui::MenuItem("TCP"))
                {
                    ImGui::custom_GlobalOpenPopup("TCP configuration");
                }
                if (ImGui::MenuItem("VDT file"))
                {
                    m_minitel.openFileBrowser();
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
                if(ImGui::MenuItem("Summary / Sommaire", "TAB"))
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
            if (ImGui::BeginMenu("Baudrate##OptionsBaudrate"))
            {
                auto baudrate = m_minitel.getMinitelCore().get_baudrate();
                bool bauto  =   baudrate == mtlc::Minitel::BAUDRATES::AUTO;
                bool b300   =   baudrate == mtlc::Minitel::BAUDRATES::B300;
                bool b1200  =   baudrate == mtlc::Minitel::BAUDRATES::B1200;
                bool b4800  =   baudrate == mtlc::Minitel::BAUDRATES::B4800;
                bool b9600  =   baudrate == mtlc::Minitel::BAUDRATES::B9600;
                if (ImGui::Checkbox("Auto", &bauto))
                {
                    baudrate = mtlc::Minitel::BAUDRATES::AUTO;
                }
                if (ImGui::Checkbox("300 bauds", &b300))
                {
                    baudrate = mtlc::Minitel::BAUDRATES::B300;
                }
                if (ImGui::Checkbox("1200 bauds", &b1200))
                {
                    baudrate = mtlc::Minitel::BAUDRATES::B1200;
                }
                if (ImGui::Checkbox("4800 bauds", &b4800))
                {
                    baudrate = mtlc::Minitel::BAUDRATES::B4800;
                }
                if (ImGui::Checkbox("9600 bauds", &b9600))
                {
                    baudrate = mtlc::Minitel::BAUDRATES::B9600;
                }
                m_minitel.getMinitelCore().set_baudrate(baudrate);
                ImGui::EndMenu();
            }
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