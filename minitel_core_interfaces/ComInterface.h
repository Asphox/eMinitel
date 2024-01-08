// ComInterface.h
// Created by asphox on 1/3/24.
// Interface between Minitel minitel_core and a communication module (TCP, serial, file, modem ...)

#ifndef COMINTERFACE_H
#define COMINTERFACE_H

#include <cstdint>

namespace mtlc
{
    class ICom
    {
    public:
        virtual std::size_t pull_data(const char** p_data) = 0;
        virtual void send_data(const char* data, std::size_t size) = 0;
    };
}

#endif //COMINTERFACE_H
