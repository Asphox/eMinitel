//
// Created by Asphox on 10/04/2024.
//

#ifndef EMINITEL_WINDEBUG_H
#define EMINITEL_WINDEBUG_H

#include "IHM.h"

namespace IHM
{
    class WinDebug : public CIHM
    {
    private:
        bool m_visible = false;
    public:
        WinDebug(eMinitel& r) : CIHM(r){}
        void update() override;
        inline void show(bool b){ m_visible = b;}
        inline bool visible() const { return m_visible; }
    };
}

#endif //EMINITEL_WINDEBUG_H
