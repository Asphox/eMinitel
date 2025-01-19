// Uses a VDT file as a DIN input

#include "minitel_core/Interfaces.h"
#include <fstream>

#ifndef VDTFILECOM_H
#define VDTFILECOM_H

class VDTFileCom
{
private:
    char buffer[1024] = {};
    bool m_open = false;
    std::ifstream m_file;
public:

	~VDTFileCom()
	{
		close();
	}

	bool open(const char* path)
	{
        m_file.open(path, std::ifstream::in | std::ifstream::binary);
        m_open = m_file.is_open();
        return m_open;
	}

	void close()
	{
        m_file.close();
	}

    static uint16_t s_mtlc_din_control(MTLC_OP_DIN op, intptr_t param, void* ctx)
    {
        if (!ctx)
            return 0;
        auto _this = (VDTFileCom*)ctx;
        switch (op)
        {
        case OPD_SEND_DATA:
        {
            return 0;   // not supported (VDT files are read-only)
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
        if (!m_file.is_open())
            return 0;

        if (nb_byte_to_pull > 1024)
            nb_byte_to_pull = 1024;

        m_file.read(buffer, nb_byte_to_pull);
        *p_data = buffer;
        return m_file.gcount();
    }

};

#endif //VDTFILECOM_H


