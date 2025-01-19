//
// Created by asphox on 4/3/24.
//

#ifndef MINITELVIEWER_WINCONNECTIONTCP_H
#define MINITELVIEWER_WINCONNECTIONTCP_H

#include "IHM.h"

namespace IHM
{
    class WinConnectionTCP : public CIHM
    {
    public:
        WinConnectionTCP(eMinitel& r) : CIHM(r){}
        void update() override;
        void reset();
        inline const char* getAddress() const { return m_address; }
        inline uint32_t getPort() const { return m_port; }

        inline void OnConnect(std::function<bool()>&& on){ m_on_connect = on; }
    private:
       char m_address[256] = {};
       uint32_t m_port = 0;
       bool m_last_connection_failed = false;

        std::function<bool()> m_on_connect;
    };
}


#endif //MINITELVIEWER_WINCONNECTIONTCP_H
