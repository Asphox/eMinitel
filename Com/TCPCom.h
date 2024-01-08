//
// Created by asphox on 1/3/24.
//

#include "../minitel_core_interfaces/ComInterface.h"
#include <SFML/Network.hpp>

#ifndef TCPCOM_H
#define TCPCOM_H

class TCPCom : public mtlc::ICom
{
    char buffer[1024] = {};
    sf::TcpSocket m_socket;
public:
    TCPCom()
    {
        m_socket.connect("127.0.0.1",26400);
        m_socket.setBlocking(false);
    }

    ~TCPCom()
    {
        m_socket.disconnect();
    }

    std::size_t pull_data(const char** p_data) override
    {
        std::size_t r = 0;
        if(m_socket.receive(buffer, 1024, r) == sf::Socket::Done)
            *p_data = buffer;
        return r;
    }

    void send_data(const char* data, std::size_t size) override
    {
        m_socket.send(data, size);
    }

};

#endif //TCPCOM_H
