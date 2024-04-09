//
// Created by Asphox on 04/04/2024.
//

#ifndef MINITELVIEWER_WINABOUT_H
#define MINITELVIEWER_WINABOUT_H


#include "IHM.h"

namespace IHM
{
    class WinAbout : public IHM
    {
    public:
        WinAbout(eMinitel& r) : IHM(r){}
        void update() override;
    };
}


#endif //MINITELVIEWER_WINABOUT_H
