//
// Created by asphox on 1/3/24.
//

#include "../minitel_core/Interfaces.h"
#include <SFML/Network.hpp>

#ifndef TCPCOM_H
#define TCPCOM_H

class TCPCom
{
    char buffer[1024] = {};
    sf::TcpSocket m_socket;
    bool m_connected = false;
    std::string m_address;
    int m_port;
public:
    TCPCom()
    {
    }

    bool connect(const char* address, int port)
    {
        m_address = "";
        m_port = 0;
        sf::Time timeout = sf::milliseconds(5000);
        sf::Socket::Status status;
        m_socket.setBlocking(true);
        do
        {
            status = m_socket.connect(address,port, timeout);
        }while(status == sf::Socket::Status::NotReady);
        if(status == sf::Socket::Status::Done)
        {
            m_address = address;
            m_port = port;
            m_socket.setBlocking(false);
            m_connected = true;
            return true;
        }
        m_connected = false;
        return false;
    }

    void disconnect()
    {
        m_address = "";
        m_port = 0;
        m_socket.disconnect();
        std::fill_n(buffer, sizeof(buffer), 0);
        m_connected = false;
    }

    inline bool isConnected() const { return m_connected; }
    inline std::string_view getAddress() const { return m_address; }
    inline int getPort() const { return m_port; }

    ~TCPCom()
    {
        disconnect();
    }

    static uint16_t s_mtlc_din_control(MTLC_OP_DIN op, intptr_t param, void* ctx)
    {
        if(!ctx)
            return 0;
        auto _this = (TCPCom*)ctx;
        switch(op)
        {
            case OPD_SEND_DATA:
            {
                auto param_send_data = (mtlc_op_din_param_send_data*)param;
                _this->send_data(param_send_data->data, param_send_data->size);
                return 0;
            }
            case OPD_PULL_DATA:
            {
                auto param_pull_data = (mtlc_op_din_param_pull_data*)param;
                return _this->pull_data((const char**)param_pull_data->data, param_pull_data->nb_byte_to_pull);
            }
            default:
                return 0;
        }
    }

    std::size_t pull_data(const char** p_data, std::size_t nb_byte_to_pull)
    {
        if(!m_connected)
            return 0;

        if (nb_byte_to_pull > 1024)
            nb_byte_to_pull = 1024;

        std::size_t r = 0;
        if(m_socket.receive(buffer, nb_byte_to_pull, r) == sf::Socket::Done)
            *p_data = buffer;
        return r;
    }

    void send_data(const std::uint8_t* data, std::size_t size)
    {
        if(!m_connected)
            return;
        m_socket.send(data, size);
    }

};

#endif //TCPCOM_H
