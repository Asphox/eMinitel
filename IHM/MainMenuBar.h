//
// Created by asphox on 4/3/24.
//

#ifndef MINITELVIEWER_MAINMENUBAR_H
#define MINITELVIEWER_MAINMENUBAR_H

#include "IHM.h"
#include <functional>

namespace IHM
{
    class MainMenuBar : public CIHM
    {
    private:
        std::function<void()> m_on_quit;
        std::function<void()> m_on_connection_tcp;
        std::function<void()> m_on_disconnect;
        std::function<void()> m_on_reset;
        std::function<void()> m_on_log;
    public:
        MainMenuBar(eMinitel& r) : CIHM(r){}
        virtual ~MainMenuBar()  {}
        void update() override;
        inline void OnQuit(std::function<void()>&& on){ m_on_quit = on; }
        inline void OnConnectionTCP(std::function<void()>&& on){ m_on_connection_tcp = on; }
        inline void OnDisconnect(std::function<void()>&& on){ m_on_disconnect = on; }
        inline void OnReset(std::function<void()>&& on){ m_on_reset = on; }
        inline void OnLog(std::function<void()>&& on){ m_on_log = on; }
    };
}


#endif //MINITELVIEWER_MAINMENUBAR_H
