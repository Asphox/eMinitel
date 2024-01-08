//
// Created by asphox on 1/3/24.
//

#ifndef MINITELVIEWER_TESTCOM_H
#define MINITELVIEWER_TESTCOM_H

#include "../Core/ComInterface.h"
#include "../Core/Glyph.h"
#include "../minitel_core_interfaces/Constants.h"
#include <iostream>
#include <string_view>

class Com : public mtlc::ICom
{
    char data[12] = {};
public:
    std::size_t pull_data(const char** p_data) override
    {

        data[0] = VC_CON;
        data[1] = VC_FF;
        data[2] = 'H';
        data[3] = 'e';
        data[4] = 'l';
        data[5] = 'l';
        data[6] = VC_ESC;
        data[7] = 0x61;
        *p_data = data;
        return 8;
    }
    void        send_data(const char* data, std::size_t size) override
    {
        for(int i=0; i<size; i++)
        {
            std::cout << (int)data[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif //MINITELVIEWER_TESTCOM_H
