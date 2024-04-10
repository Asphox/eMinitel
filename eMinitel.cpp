//
// Created by asphox on 4/3/24.
//

#include "eMinitel.h"

eMinitel::eMinitel(sf::RenderWindow &target)
:m_render_target(target)
,m_ihm_win_connection_tcp(*this)
,m_ihm_menu_bar(*this)
,m_ihm_win_about(*this)
,m_ihm_win_debug(*this)
,m_minitel_screen(target.getSize().x*0.765)
{
    ImGui::SFML::Init(target);
    m_minitel_screen.setPosition(target.getSize().x*0.01, target.getSize().y*0.05);
    m_minitel_core.set_screen_control(Screen::s_mtlc_screen_control, &m_minitel_screen);
    m_minitel_core.set_keyboard_control(Keyboard::s_mtlc_keyboard_control, &m_minitel_keyboard);
    m_minitel_core.set_din_control(TCPCom::s_mtlc_din_control, &m_minitel_TCP);

    __InitCallbacksIHM();
}

void eMinitel::__InitCallbacksIHM()
{
    m_ihm_menu_bar.OnQuit( [this](){m_render_target.close();} );
    m_ihm_menu_bar.OnConnectionTCP( [](){ImGui::custom_GlobalOpenPopup("TCP configuration");} );
    m_ihm_menu_bar.OnDisconnect([this](){
        m_minitel_TCP.disconnect();
        m_render_target.setTitle(s_title);
    });
    m_ihm_menu_bar.OnReset([this](){
        m_minitel_core.reset();
    });

    m_ihm_win_connection_tcp.OnConnect([this]() -> bool {
        auto address = m_ihm_win_connection_tcp.getAddress();
        auto port = m_ihm_win_connection_tcp.getPort();
        m_minitel_TCP.disconnect();
        if(m_minitel_TCP.connect(address, port))
        {
            std::string s = s_title;
            s += " (tcp: ";
            s += address;
            s += ")";
            m_render_target.setTitle(s);
            return true;
        }
        m_render_target.setTitle(s_title);
        return false;
    });
}

void eMinitel::run()
{
    sf::Clock deltaClock;

    while (m_render_target.isOpen())
    {
        sf::Event event;
        while (m_render_target.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(m_render_target, event);
            if(!IHM::isModalOpened())
                m_minitel_keyboard.process_event(event);

            if (event.type == sf::Event::Closed)
                m_render_target.close();
        }

        m_minitel_core.update();

        ImGui::SFML::Update(m_render_target, deltaClock.restart());

        IHM::reset();
        m_ihm_menu_bar.update();
        m_ihm_win_connection_tcp.update();
        m_ihm_win_about.update();
        m_ihm_win_debug.update();

        m_render_target.clear();
        m_render_target.draw(m_minitel_screen);
        ImGui::SFML::Render(m_render_target);
        m_render_target.display();
    }

    ImGui::SFML::Shutdown();
}

bool eMinitel::isConnected() const
{
    return m_minitel_TCP.isConnected();
}

bool eMinitel::colorMode() const
{
    return m_minitel_screen.get_color_mode();
}

void eMinitel::setColorMode(bool b)
{
    m_minitel_screen.set_color_mode(b);
}