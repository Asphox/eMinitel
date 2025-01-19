//
// Created by asphox on 4/3/24.
//

#include "eMinitel.h"
#include "Audio/Beep.h"

eMinitel::eMinitel(sf::RenderWindow &target)
:m_render_target(target)
,m_ihm_win_connection_tcp(*this)
,m_ihm_win_open_vdtfile(*this)
,m_ihm_menu_bar(*this)
,m_ihm_win_about(*this)
,m_ihm_win_debug(*this)
,m_minitel_screen(target.getSize().x*0.765)
{
    ImGui::SFML::Init(target);
    m_minitel_screen.setPosition(target.getSize().x*0.01, target.getSize().y*0.05);
    m_minitel_core.set_screen_control(Screen::s_mtlc_screen_control, &m_minitel_screen);
    m_minitel_core.set_keyboard_control(Keyboard::s_mtlc_keyboard_control, &m_minitel_keyboard);
    m_minitel_core.set_callback_on_bell([](void*)
    {
        sf::Sound& sound = get_beep_sound();
        sound.setVolume(4);
        sound.play();
    }, nullptr);

    __InitCallbacksIHM();

    m_configuration.loadFromFile("config.json");
}

eMinitel::~eMinitel()
{
    m_configuration.serializeToFile("config.json");
}

void eMinitel::openFileBrowser()
{
    m_ihm_win_open_vdtfile.open();
}

bool eMinitel::connect(const std::string& connector)
{
    std::stringstream ss(connector);

    std::string subpart;
    if (!std::getline(ss, subpart, ':'))
        return false;

    if (subpart.compare("tcp") == 0)
    {
        m_minitel_core.set_din_control(TCPCom::s_mtlc_din_control, &m_minitel_TCP);
        m_minitel_TCP.disconnect();
        // retrieves address
        if (!std::getline(ss, subpart, ':'))
            return false;
        std::string address = subpart;
        // retrieves port
        if (!std::getline(ss, subpart, ':'))
            return false;
        int port = std::stoi(subpart.c_str());
        bool res = m_minitel_TCP.connect(address.c_str(), port);
        if (res)
        {
            m_configuration.m_recent_connections.insert(m_configuration.m_recent_connections.begin(), connector);
            return true;
        }
        return false;
    }
    else if (subpart.compare("file") == 0)
    {
        m_minitel_core.set_din_control(VDTFileCom::s_mtlc_din_control, &m_minitel_vdtfile);
        m_minitel_vdtfile.close();
        if (!std::getline(ss, subpart, '\n'))
            return false;
        std::string path = subpart;
        if (m_minitel_vdtfile.open(path.c_str()))
        {
            m_configuration.m_recent_connections.insert(m_configuration.m_recent_connections.begin(), connector);
            return true;
        }
        return false;
    }
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
        m_minitel_core.set_din_control(TCPCom::s_mtlc_din_control, &m_minitel_TCP);
        if(m_minitel_TCP.connect(address, port))
        {
            std::string connection = "tcp:";
            connection += address;
            connection += ":";
            connection += std::to_string(port);
            m_configuration.m_recent_connections.insert(m_configuration.m_recent_connections.begin(), connection);
            return true;
        }
        return false;
    });

    m_ihm_win_open_vdtfile.OnConnect([this]() -> bool {
        const auto& path = m_ihm_win_open_vdtfile.getFilePath();
        m_minitel_vdtfile.close();
        m_minitel_core.set_din_control(VDTFileCom::s_mtlc_din_control, &m_minitel_vdtfile);
        if (m_minitel_vdtfile.open(path.c_str()))
        {
            std::string connection = "file:";
            connection += path;
            m_configuration.m_recent_connections.insert(m_configuration.m_recent_connections.begin(), connection);
            return true;
        }
        return false;
    });
}

void eMinitel::run()
{
    sf::Clock deltaClock;
    m_render_target.setFramerateLimit(60);
    while (m_render_target.isOpen())
    {
        std::string title = "eMinitel ";
        m_render_target.setTitle(title + getStatus());
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
        m_ihm_win_open_vdtfile.update();
        m_ihm_win_about.update();
        m_ihm_win_debug.update();

        m_render_target.clear();
        m_render_target.draw(m_minitel_screen);
        ImGui::SFML::Render(m_render_target);
        m_render_target.display();
    }

    ImGui::SFML::Shutdown();
}

std::string eMinitel::getStatus() const
{
    std::string status;
    if (m_minitel_TCP.isConnected())
    {
        status += "(TCP: ";
        status += m_minitel_TCP.getAddress();
        status += ") ";
    }
    switch (m_minitel_core.get_baudrate())
    {
    case mtlc::Minitel::BAUDRATES::B300:
        status += "300 bauds ";
        break;
    case mtlc::Minitel::BAUDRATES::B1200:
        status += "1200 bauds ";
        break;
    case mtlc::Minitel::BAUDRATES::B4800:
        status += "4800 bauds ";
        break;
    case mtlc::Minitel::BAUDRATES::B9600:
        status += "9600 bauds ";
        break;
    }

    return status;
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