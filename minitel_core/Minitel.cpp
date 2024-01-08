//
// Created by asphox on 12/26/23.
//

#include "Minitel.h"
#include "../minitel_core_interfaces/Constants.h"

using namespace mtlc;

constexpr GLYPH_CODE F_CONNECT_00 = set_glyph_att_color(GC_F, GFC_BLACK, GBC_WHITE);

Minitel::Minitel(IScreen* screen, ICom* com) : m_screen(screen), m_com(com)
{
    reset();
}

void Minitel::reset()
{
    m_context.reset();
    m_inputs_buffer.clear();
    if(m_screen)
    {
        m_screen->update();
        m_screen->clear();
        m_screen->set_cursor_visible(false);
        m_cursor_pos = CursorPos();
        m_screen->set_cursor_pos(0, 1);
        m_screen->set_glyph_at(F_CONNECT_00, 38, 0);
    }
}

void Minitel::update()
{
    __process_inputs();
    if(m_screen)
        m_screen->update();
}

void Minitel::__pull_data()
{
    const char* data = nullptr;
    if(m_com)
    {
        std::size_t size = m_com->pull_data(&data);
        if(size == 0 || data == nullptr)
            return;

        for(std::size_t i=0; i<size; i++)
            m_inputs_buffer.push_back(data[i]);
    }
}

void Minitel::__send_data(const char* data, std::size_t size)
{
    if(m_com)
        m_com->send_data(data, size);
}

void Minitel::__process_inputs()
{
    __pull_data();
    // deleting all VC_NUL
    std::erase_if(m_inputs_buffer, [](char c){ return c == VC_NUL; });

    lbl_continue:
    if(m_inputs_buffer.empty())
        return;

    int nb_byte_processed = 0;
    RESULT_FUNC res_func = RF_ERR;
    char c = m_inputs_buffer[0];
    // control char
    if(c >= __VC_MIN && c <= __VC_MAX)
    {
        switch(c)
        {
            case VC_BEL: break;//todo BELL
            case VC_BS: res_func = __func_BS(nb_byte_processed); break;
            case VC_HT: res_func = __func_HT(nb_byte_processed);break;
            case VC_LF: res_func = __func_LF(nb_byte_processed);break;
            case VC_VT: res_func = __func_VT(nb_byte_processed);break;
            case VC_FF: res_func = __func_FF(nb_byte_processed); break;
            case VC_CR: res_func = __func_CR(nb_byte_processed); break;
            case VC_SO: res_func = __func_SO(nb_byte_processed); break;
            case VC_SI: res_func = __func_SI(nb_byte_processed); break;
            case VC_CON: res_func = __func_CON(nb_byte_processed); break;
            case VC_REP: res_func = __func_REP(nb_byte_processed); break;
            case VC_SEP: break; // commande/ack
            case VC_COFF: res_func = __func_COFF(nb_byte_processed); break;
            case VC_CAN: res_func = __func_CAN(nb_byte_processed); break;
            case VC_SS2: res_func = __func_SS2(nb_byte_processed); break;
            case VC_SUB: break; // print error
            case VC_ESC: res_func = __func_ESC(nb_byte_processed); break;
            case VC_RS: res_func = __func_RS(nb_byte_processed); break;
            case VC_US: res_func = __func_US(nb_byte_processed); break;

            // do-nothing commands
            case VC_NUL:
            case VC_SOH:
            case VC_EOT:
            case VC_ENQ:
            case VC_DLE:    // modem only
            case VC_NACK:
            case VC_SYN:    // modem only
            case VC_SS3:
                break;

            // invalid
            default:
                break;

        }
    }
    else
    {
        res_func = __func_write_char(nb_byte_processed);
    }

    if(res_func == RF_OK || res_func == RF_IGNORED)
    {
        for(int i=0; i<nb_byte_processed; i++)
            m_inputs_buffer.pop_front();
        goto lbl_continue;
    }
    else if(res_func == RF_INSUFFICIENT_DATA)
    {
        return;
    }
    else
    {
        m_inputs_buffer.pop_front();
    }

}

void Minitel::__write_glyph(GLYPH_CODE gc)
{
    CursorPos pos = __get_cursor_pos();
    if(m_screen)
        m_screen->set_glyph_at(gc, pos.col, pos.line);
    __push_cursor_right(true);
}

Minitel::CursorPos Minitel::__get_cursor_pos()
{
    return m_cursor_pos;
}
void Minitel::__set_cursor_pos(Minitel::CursorPos pos)
{
    m_cursor_pos = pos;
    // We need to save the last position between 01-24 lines because an LF in 00 must move to the last position in 01-24
    if(m_cursor_pos.line != 0)
        m_context.last_main_screen_cursor_pos = m_cursor_pos;
    if(m_screen)
        m_screen->set_cursor_pos(m_cursor_pos.col, m_cursor_pos.line);
}

void Minitel::__write_char(char code)
{
    GLYPH_CHARSET cs = G0;

    if(code == ' ')
        cs = SPEC;
    else if(m_context.is_escaped_to_G1)
        cs = G1;
    else if(m_context.is_escaped_to_G2)
        cs = G2;
    CursorPos pos = __get_cursor_pos();

    GLYPH_CODE gc = get_glyph_from_charset_code_accent(cs, code, GCA_NONE);

    // we need to apply current char attributes
    gc = set_glyph_att_fcolor(gc, m_context.current_char_attributes.fcolor);
    gc = set_glyph_att_blink(gc, m_context.current_char_attributes.blink);
    gc = set_glyph_att_negative(gc, m_context.current_char_attributes.negative);
    // we can't apply double height and double size on line 00
    if(pos.line != 0 || (pos.line == 0 && m_context.current_char_attributes.size != GS_DOUBLE_SIZE && m_context.current_char_attributes.size != GS_DOUBLE_HEIGHT))
        gc = set_glyph_att_size(gc, m_context.current_char_attributes.size);
    m_context.last_glyph = gc;
    // if zone attributes are waiting and charset is SPEC (space) or G1 => we must start a new zone by creating a delimiter
    if((code == ' ' || cs == G1) && !m_context.waiting_zone_attributes.applied)
    {
        gc = set_glyph_zone_delimiter(gc, true);
        gc = set_glyph_att_bcolor(gc, m_context.waiting_zone_attributes.bcolor);
        gc = set_glyph_att_underline(gc, m_context.waiting_zone_attributes.underline);
        gc = set_glyph_att_mask(gc, m_context.waiting_zone_attributes.masked);
        m_context.waiting_zone_attributes.applied = true;
    }

    if(m_screen)
        m_screen->set_glyph_at(gc, pos.col, pos.line);
    __push_cursor_right(true);
    if(get_glyph_att_size(gc) == GS_DOUBLE_WIDTH || get_glyph_att_size(gc) == GS_DOUBLE_SIZE)
        __push_cursor_right(true);
}

void Minitel::__push_cursor_left(bool loop)
{
    CursorPos pos = __get_cursor_pos();
    if(pos.col == 0)
    {
        if(pos.line == 0 || !loop)
            return;
        else if(pos.line == 1)
        {
            pos.line = 24;
            pos.col = 39;
        }
        else
        {
            pos.line--;
            pos.col = 39;
        }
    }
    else
    {
        pos.col--;
    }
    __set_cursor_pos(pos);
}

void Minitel::__push_cursor_right(bool loop)
{
    CursorPos pos = __get_cursor_pos();
    if(pos.col == 39)
    {
        if(pos.line == 0 || !loop)
            return;
        else if(pos.line == 24)
        {
            pos.line = 1;
            pos.col = 0;
        }
        else
        {
            pos.line++;
            pos.col = 0;
        }
    }
    else
    {
        pos.col++;
    }
    __set_cursor_pos(pos);
}

void Minitel::__push_cursor_down(bool loop)
{
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
    {
        __set_cursor_pos(m_context.last_main_screen_cursor_pos);
        return;
    }
    else if(pos.line == 24)
    {
        if(loop)
        {
            // if rolling => we must roll-up the screen
            if(m_context.is_rolling_mode && m_screen)
            {
                std::int8_t nb_line = -1;
                if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                    nb_line = -2;
                m_screen->scroll(nb_line);
            }
            else
                pos.line = 1;
        }

    }
    else
        pos.line++;

    __set_cursor_pos(pos);
}

void Minitel::__push_cursor_up(bool loop)
{
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return;

    if(pos.line == 1)
    {
        // if rolling => we must roll-down the screen
        if(m_context.is_rolling_mode && m_screen)
        {
            std::int8_t nb_line = 1;
            if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                nb_line = 2;
            m_screen->scroll(nb_line);
        }
        else
            pos.line = 24;
    }
    else
        pos.line--;

    __set_cursor_pos(pos);
}

std::uint8_t Minitel::__get_PRO2_status_byte()
{
    std::uint8_t status = 0b0100000;
    if(m_context.is_rolling_mode)
        status |= 1<<1;
    // TODO reste
    return status;
}

