//
// Created by asphox on 4/3/24.
//

#ifndef MINITELVIEWER_MINITELVIEWER_H
#define MINITELVIEWER_MINITELVIEWER_H

#include <SFML/Graphics.hpp>
#include "minitel_core/Minitel.h"
#include "Com/TCPCom.h"
#include "Com/VDTFileCom.h"
#include "Keyboard/Keyboard.h"
#include "Screen/Screen.h"
#include "IHM/imgui/imgui.h"
#include "IHM/imgui/imgui-SFML.h"

#include "IHM/MainMenuBar.h"
#include "IHM/WinConnectionTCP.h"
#include "IHM/WinOpenVDTFile.h"
#include "IHM/WinAbout.h"
#include "IHM/WinDebug.h"

#include "Configuration.h"

class eMinitel
{
friend class IHM::MainMenuBar;
private:
    sf::RenderWindow& m_render_target;
    mtlc::Minitel     m_minitel_core;
    Screen            m_minitel_screen;
    Keyboard          m_minitel_keyboard;
    TCPCom            m_minitel_TCP;
    VDTFileCom        m_minitel_vdtfile;

    // IHM
    IHM::MainMenuBar        m_ihm_menu_bar;
    IHM::WinConnectionTCP   m_ihm_win_connection_tcp;
    IHM::WinOpenVDTFile     m_ihm_win_open_vdtfile;
    IHM::WinAbout           m_ihm_win_about;
    IHM::WinDebug           m_ihm_win_debug;
    bool m_block_event_for_minitel = false;
    void __InitCallbacksIHM();

    // Config
    Configuration m_configuration;

public:
    eMinitel(sf::RenderWindow& target);
    ~eMinitel();
    bool connect(const std::string& connector);
    bool isConnected() const;
    bool colorMode() const ;
    void setColorMode(bool b);
    void run();
    std::string getStatus() const;
    void openFileBrowser();

    inline mtlc::Minitel& getMinitelCore(){ return m_minitel_core; }

    static constexpr char s_title[] = "eMinitel";

};


#endif //MINITELVIEWER_MINITELVIEWER_H
