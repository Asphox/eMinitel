//
// Created by asphox on 4/3/24.
//

#ifndef MINITELVIEWER_MINITELVIEWER_H
#define MINITELVIEWER_MINITELVIEWER_H

#include <SFML/Graphics.hpp>
#include "minitel_core/Minitel.h"
#include "Com/TCPCom.h"
#include "Keyboard/Keyboard.h"
#include "Screen/Screen.h"
#include "IHM/imgui/imgui.h"
#include "IHM/imgui/imgui-SFML.h"

#include "IHM/MainMenuBar.h"
#include "IHM/WinConnectionTCP.h"
#include "IHM/WinAbout.h"
#include "IHM/WinDebug.h"

class eMinitel
{
friend class IHM::MainMenuBar;
private:
    sf::RenderWindow& m_render_target;
    mtlc::Minitel     m_minitel_core;
    Screen            m_minitel_screen;
    Keyboard          m_minitel_keyboard;
    TCPCom            m_minitel_TCP;

    // IHM
    IHM::MainMenuBar        m_ihm_menu_bar;
    IHM::WinConnectionTCP   m_ihm_win_connection_tcp;
    IHM::WinAbout           m_ihm_win_about;
    IHM::WinDebug           m_ihm_win_debug;

    bool m_block_event_for_minitel = false;

    void __InitCallbacksIHM();

public:
    eMinitel(sf::RenderWindow& target);
    bool isConnected() const;
    bool colorMode() const ;
    void setColorMode(bool b);
    void run();

    inline mtlc::Minitel& getMinitelCore(){ return m_minitel_core; }

    static constexpr char* s_title = "eMinitel";

};


#endif //MINITELVIEWER_MINITELVIEWER_H
