// Functionalities.cpp
// Created by asphox on 1/3/24.
// Implements all functionalities for the Minitel protocol

#include "Minitel.h"
#include "VideotexCodes.h"
#include <ctype.h>

using namespace mtlc;

Minitel::RESULT_FUNC Minitel::__func_BS(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    //resets current zone attributes
    m_context.waiting_zone_attributes.reset();
    __push_cursor_left(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_HT(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    //resets current zone attributes
    m_context.waiting_zone_attributes.reset();
    __push_cursor_right(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_LF(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    //resets current zone attributes
    m_context.waiting_zone_attributes.reset();
    __push_cursor_down(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_VT(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    //resets current zone attributes
    m_context.waiting_zone_attributes.reset();
    __push_cursor_up(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_FF(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    m_context.reset_article();
    if(!m_screen_control)
        return RF_IGNORED;
    m_screen_control(OPS_CLEAR, true, m_screen_ctx);
    __set_cursor_pos(CursorPos());
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CR(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    CursorPos pos = __get_cursor_pos();
    pos.col = 0;
    __set_cursor_pos(pos);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SO(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    m_context.is_escaped_to_G1 = true;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SI(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    m_context.is_escaped_to_G1 = false;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CON(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    if(!m_screen_control)
        return RF_IGNORED;
    m_screen_control(OPS_SET_CURSOR_VISIBLE, true, m_screen_ctx);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_COFF(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    if(!m_screen_control)
        return RF_IGNORED;
    m_screen_control(OPS_SET_CURSOR_VISIBLE, false, m_screen_ctx);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CAN(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    CursorPos old_pos = __get_cursor_pos();
    for(int col = old_pos.col; col<40; col++)
        __write_glyph(GC_SPACE);
    __set_cursor_pos(old_pos);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SS2(int& nb_of_byte_processed)
{
    //todo accent
    if(m_inputs_buffer.size() < 2)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 2;
    if(m_context.is_escaped_to_G1)
        return RF_IGNORED;

    m_context.is_escaped_to_G2 = true;
    __write_char(m_inputs_buffer[1]);
    m_context.is_escaped_to_G2 = false;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_REP(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 2)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 2;
    char code = m_inputs_buffer[1];
    if(code < 0x40)
    {
        return RF_ERR;
    }

    code -= 0x40;
    for(int i=0; i<code; i++)
        __write_glyph(m_context.last_glyph);

    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_RS(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    m_context.reset_article();
    CursorPos pos = __get_cursor_pos();
    pos.line = 1;
    pos.col = 0;
    __set_cursor_pos(pos);
    m_context.reset_article();
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_US(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 3;
    m_context.reset_article();

    char col = m_inputs_buffer[1] - 0x40;
    char line = m_inputs_buffer[2] - 0x40;
    if(col < 0 || col > 39 || line < 0 || line > 24)
        return RF_IGNORED;

    m_context.reset_article();
    CursorPos pos;
    pos.col = col;
    pos.line = line;
    __set_cursor_pos(pos);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_write_char(int& nb_of_byte_processed)
{
    nb_of_byte_processed = 1;
    __write_char(m_inputs_buffer[0]);
    return RF_OK;
}

//==================================================================
// Escaped Functionalities
//==================================================================

Minitel::RESULT_FUNC Minitel::__func_ESC(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 2)
        return  RF_INSUFFICIENT_DATA;
    // if we have an escapement to ISO 6429
    if(m_inputs_buffer[1] == 0x5B)
        return __func_CSI(nb_of_byte_processed);
    nb_of_byte_processed = 2;
    switch(m_inputs_buffer[1])
    {
        // mask/unmask screen
        case 0x23 : return __func_set_mask_screen(nb_of_byte_processed);
        // protocol
        case 0x39 : return __func_PRO1(nb_of_byte_processed);  // PRO1
        case 0x3A : return __func_PRO2(nb_of_byte_processed);  // PRO2
        case 0x3B : return __func_PRO3(nb_of_byte_processed);  // PRO3
        case 0x61 : return __func_PRO_send_cursor_pos(nb_of_byte_processed);
        // char attributes
        case 0x40 : m_context.current_char_attributes.fcolor = GFC_BLACK; break;
        case 0x41 : m_context.current_char_attributes.fcolor = GFC_RED; break;
        case 0x42 : m_context.current_char_attributes.fcolor = GFC_GREEN; break;
        case 0x43 : m_context.current_char_attributes.fcolor = GFC_YELLOW; break;
        case 0x44 : m_context.current_char_attributes.fcolor = GFC_BLUE; break;
        case 0x45 : m_context.current_char_attributes.fcolor = GFC_MAGENTA; break;
        case 0x46 : m_context.current_char_attributes.fcolor = GFC_CYAN; break;
        case 0x47 : m_context.current_char_attributes.fcolor = GFC_WHITE; break;
        case 0x48 : m_context.current_char_attributes.blink = true; break;
        case 0x49 : m_context.current_char_attributes.blink = false; break;
        case 0x4C : m_context.current_char_attributes.size = GS_NORMAL; break;
        case 0x4D : m_context.current_char_attributes.size = GS_DOUBLE_HEIGHT; break;
        case 0x4E : m_context.current_char_attributes.size = GS_DOUBLE_WIDTH; break;
        case 0x4F : m_context.current_char_attributes.size = GS_DOUBLE_SIZE; break;
        // zone attributes
        case 0x50 :
            m_context.waiting_zone_attributes.bcolor    = GBC_BLACK;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x51 :
            m_context.waiting_zone_attributes.bcolor   = GBC_RED;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x52 :
            m_context.waiting_zone_attributes.bcolor   = GBC_GREEN;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x53 :
            m_context.waiting_zone_attributes.bcolor   = GBC_YELLOW;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x54 :
            m_context.waiting_zone_attributes.bcolor   = GBC_BLUE;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x55 :
            m_context.waiting_zone_attributes.bcolor   = GBC_MAGENTA;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x56 :
            m_context.waiting_zone_attributes.bcolor   = GBC_CYAN;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x57 :
            m_context.waiting_zone_attributes.bcolor   = GBC_WHITE;
            m_context.waiting_zone_attributes.applied   = false;
            break;
        case 0x58 :
            m_context.waiting_zone_attributes.masked = true;
            m_context.waiting_zone_attributes.applied  = false;
            break;
        case 0x59 :
            m_context.waiting_zone_attributes.underline = false;
            m_context.waiting_zone_attributes.applied  = false;
            break;
        case 0x5A :
            m_context.waiting_zone_attributes.underline = true;
            m_context.waiting_zone_attributes.applied  = false;
            break;
        case 0x5C : m_context.current_char_attributes.negative = false; break;
        case 0x5D : m_context.current_char_attributes.negative = true; break;
        case 0x5F :
            m_context.waiting_zone_attributes.masked = false;
            m_context.waiting_zone_attributes.applied  = false;
            break;
        default: return RF_IGNORED;
    }

    return RF_OK;
}
// called by __func_ESC during ISO 6429 escaping
Minitel::RESULT_FUNC Minitel::__func_CSI(int& nb_of_byte_processed)
{
    // CSI functionalities that only use 3 bytes
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 3;
    switch(m_inputs_buffer[2])
    {
        case 0x4A : return __func_CSI_J(nb_of_byte_processed);
        case 0x4B : return __func_CSI_K(nb_of_byte_processed);
        default:    break;
    }

    // CSI functionalities that use 4 bytes
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    switch(m_inputs_buffer[3])
    {
        case 0x41: return __func_CSI_A(nb_of_byte_processed);
        case 0x42: return __func_CSI_B(nb_of_byte_processed);
        case 0x43: return __func_CSI_C(nb_of_byte_processed);
        case 0x44: return __func_CSI_D(nb_of_byte_processed);
        case 0x4A :
        {
            switch(m_inputs_buffer[2])
            {
                case 0x30 : return __func_CSI_0J(nb_of_byte_processed);
                case 0x31 : return __func_CSI_1J(nb_of_byte_processed);
                case 0x32 : return __func_CSI_2J(nb_of_byte_processed);
                default:    return RF_ERR;
            }
        }
        case 0x4B :
        {
            switch(m_inputs_buffer[2])
            {
                case 0x30 : return __func_CSI_0K(nb_of_byte_processed);
                case 0x31 : return __func_CSI_1K(nb_of_byte_processed);
                case 0x32 : return __func_CSI_2K(nb_of_byte_processed);
                default:    return RF_ERR;
            }
        }
        case 0x68 : return __func_CSI_h(nb_of_byte_processed);
        case 0x6C : return __func_CSI_i(nb_of_byte_processed);
        default:    break;
    }

    // CSI functionalities that use 5 bytes
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 5;
    switch(m_inputs_buffer[4])
    {
        case 0x40: return RF_IGNORED;   // only for RTIC Minitel (not implemented in this emulator)
        case 0x41: return __func_CSI_A(nb_of_byte_processed);
        case 0x42: return __func_CSI_B(nb_of_byte_processed);
        case 0x43: return __func_CSI_C(nb_of_byte_processed);
        case 0x44: return __func_CSI_D(nb_of_byte_processed);
        case 0x4C: return __func_CSI_L(nb_of_byte_processed);
        case 0x4D: return __func_CSI_M(nb_of_byte_processed);
        case 0x50: return __func_CSI_P(nb_of_byte_processed);
        default:   break;
    }

    // CSI functionalities that use 6 bytes or more ...
    if(m_inputs_buffer.size() < 6)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 6;
    if(m_inputs_buffer[3] == 0x3B || m_inputs_buffer[4] == 0x3B)
        return __func_CSI_H(nb_of_byte_processed);

    // if we are here, there is a problem ...
    return RF_ERR;
}

// clears from cursor included to end of screen
Minitel::RESULT_FUNC Minitel::__func_CSI_J(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 3;
    if(!m_screen_control)
        return RF_IGNORED;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
    param_set_glyph_at.gc = GC_SPACE;
    for(int line=pos.line; line<24; line++)
    {
        for(int col=pos.col; col<39; col++)
        {
            param_set_glyph_at.pos.col = col;
            param_set_glyph_at.pos.line = line;
            m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
        }
    }

    return RF_IGNORED;
}
// same as __func_CSI_J
Minitel::RESULT_FUNC Minitel::__func_CSI_0J(int& nb_of_byte_processed)
{
    RESULT_FUNC r = __func_CSI_J(nb_of_byte_processed);
    nb_of_byte_processed = 4;
    return r;
}
// clears from screen's start to cursor included
Minitel::RESULT_FUNC Minitel::__func_CSI_1J(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    if(!m_screen_control)
        return RF_IGNORED;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
    param_set_glyph_at.gc = GC_SPACE;
    for(int line=1; line<=pos.line; line++)
    {
        for(int col=0; col<=pos.col; col++)
        {
            param_set_glyph_at.pos.col = col;
            param_set_glyph_at.pos.line = line;
            m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
        }
    }

    return RF_OK;
}
// clears all screen (cursor is not moved)
Minitel::RESULT_FUNC Minitel::__func_CSI_2J(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    if(!m_screen_control)
        return RF_IGNORED;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    m_screen_control(OPS_CLEAR, true, m_screen_ctx);

    return RF_OK;
}
// clears from cursor included to end of line
Minitel::RESULT_FUNC Minitel::__func_CSI_K(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 3;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// same as __func_CSI_K
Minitel::RESULT_FUNC Minitel::__func_CSI_0K(int& nb_of_byte_processed)
{
    RESULT_FUNC r = __func_CSI_K(nb_of_byte_processed);
    nb_of_byte_processed = 4;
    return r;
}
// clears from current line's start to cursor (included)
Minitel::RESULT_FUNC Minitel::__func_CSI_1K(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    if(!m_screen_control)
        return RF_IGNORED;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;

    mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
    param_set_glyph_at.gc = GC_SPACE;
    param_set_glyph_at.pos.line = pos.line;
    for(int col=0; col<=pos.col; col++)
    {
        param_set_glyph_at.pos.col = col;
        m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
    }
    return RF_OK;
}
// clears all the current line
Minitel::RESULT_FUNC Minitel::__func_CSI_2K(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    if(!m_screen_control)
        return RF_IGNORED;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
    param_set_glyph_at.gc = GC_SPACE;
    param_set_glyph_at.pos.line = pos.line;
    for(int col=0; col<39; col++)
    {
        param_set_glyph_at.pos.col = col;
        m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
    }
    return RF_OK;
}
// cursor goes up n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_A(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    //we must check if the sequence is 4 or 5 bytes
    if(m_inputs_buffer[3] != 0x41)
    {
        if(m_inputs_buffer.size() < 5)
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed = 5;
    }

    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // units digit
    std::uint8_t n = (m_inputs_buffer[nb_of_byte_processed-2]-'0');
    // tens digit
    if(nb_of_byte_processed == 5)
        n += 10*(m_inputs_buffer[nb_of_byte_processed-3]-'0');

    for(int i=0; i<n; i++)
        __push_cursor_up(false);
    return RF_OK;
}
// cursor goes down n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_B(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    //we must check if the sequence is 4 or 5 bytes
    if(m_inputs_buffer[3] != 0x42)
    {
        if(m_inputs_buffer.size() < 5)
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed = 5;
    }

    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // units digit
    std::uint8_t n = (m_inputs_buffer[nb_of_byte_processed-2]-'0');
    // tens digit
    if(nb_of_byte_processed == 5)
        n += 10*(m_inputs_buffer[nb_of_byte_processed-3]-'0');

    for(int i=0; i<n; i++)
        __push_cursor_down(false);
    return RF_OK;
}
// cursor goes right n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_C(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    //we must check if the sequence is 4 or 5 bytes
    if(m_inputs_buffer[3] != 0x43)
    {
        if(m_inputs_buffer.size() < 5)
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed = 5;
    }

    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // units digit
    std::uint8_t n = (m_inputs_buffer[nb_of_byte_processed-2]-'0');
    // tens digit
    if(nb_of_byte_processed == 5)
        n += 10*(m_inputs_buffer[nb_of_byte_processed-3]-'0');

    for(int i=0; i<n; i++)
        __push_cursor_right(false);
    return RF_OK;
}
// cursor goes left n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_D(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    //we must check if the sequence is 4 or 5 bytes
    if(m_inputs_buffer[3] != 0x44)
    {
        if(m_inputs_buffer.size() < 5)
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed = 5;
    }

    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // units digit
    std::uint8_t n = (m_inputs_buffer[nb_of_byte_processed-2]-'0');
    // tens digit
    if(nb_of_byte_processed == 5)
        n += 10*(m_inputs_buffer[nb_of_byte_processed-3]-'0');

    for(int i=0; i<n; i++)
        __push_cursor_left(false);
    return RF_OK;
}
// direct addressing for cursor
Minitel::RESULT_FUNC Minitel::__func_CSI_H(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 6)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 6;

    std::uint8_t index_separator = 3;
    if(m_inputs_buffer[index_separator] != 0x3B)
    {
        index_separator++;
        if(m_inputs_buffer[index_separator] != 0x3B)
            return RF_ERR;
        if(m_inputs_buffer.size() < (nb_of_byte_processed+1))
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed++;
    }
    std::uint8_t index_end_seq = 2+index_separator;
    if(m_inputs_buffer.size() <= index_end_seq)
        return RF_INSUFFICIENT_DATA;
    if(m_inputs_buffer[index_end_seq] != 0x48)
    {
        index_end_seq++;
        if (m_inputs_buffer.size() <= index_end_seq)
            return RF_INSUFFICIENT_DATA;
        if(m_inputs_buffer[index_end_seq] != 0x48)
            return RF_ERR;
        if(m_inputs_buffer.size() < (nb_of_byte_processed+1))
            return RF_INSUFFICIENT_DATA;
        nb_of_byte_processed++;
    }
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;

    std::uint8_t index = index_end_seq-1;
    pos.col = m_inputs_buffer[index] - '0';
    index--;
    if(m_inputs_buffer[index] != 0x3B)
    {
        pos.col += 10*(m_inputs_buffer[index] - '0');
        index--;
    }
    pos.col--;
    index--;
    pos.line = (m_inputs_buffer[index] - '0');
    index--;
    if(m_inputs_buffer[index] != 0x5B)
        pos.line += 10*(m_inputs_buffer[index] - '0');

    if(pos.line < 0 || pos.line > 24 || pos.col < 0 || pos.col > 39)
        return RF_ERR;
    __set_cursor_pos(pos);
    return RF_OK;
}
// inserts n lines from current cursor line
Minitel::RESULT_FUNC Minitel::__func_CSI_L(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// deletes n lines from current cursor line
Minitel::RESULT_FUNC Minitel::__func_CSI_M(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// deletes n chars starting on cursor included
Minitel::RESULT_FUNC Minitel::__func_CSI_P(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// starts char-insertion mode
Minitel::RESULT_FUNC Minitel::__func_CSI_h(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    //todo
    return RF_OK;
}
// ends char-insertion mode
Minitel::RESULT_FUNC Minitel::__func_CSI_i(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    //todo
    return RF_OK;
}
// masks/unmasks screen
Minitel::RESULT_FUNC Minitel::__func_set_mask_screen(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    if(m_inputs_buffer[2] != 0x20)
        return RF_ERR;
    bool mask = false;
    if(m_inputs_buffer[3] == 0x58)
        mask = true;
    else if(m_inputs_buffer[3] == 0x5F)
        mask = false;
    else
        return RF_ERR;
    if(!m_screen_control)
        return RF_IGNORED;
    m_screen_control(OPS_SET_MASK, mask, m_screen_ctx);
    return RF_OK;
}

//==================================================================
// Protocol
//==================================================================
// Sends current cursor pos
Minitel::RESULT_FUNC Minitel::__func_PRO_send_cursor_pos(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 2)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 2;
    CursorPos pos = __get_cursor_pos();
    const std::uint8_t answer[3] = {VC_US, static_cast<std::uint8_t>(pos.line), static_cast<std::uint8_t>(pos.col)};
    __send_data(answer, 3);
    return RF_OK;
}

Minitel::RESULT_FUNC Minitel::__func_PRO1(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 3;
    switch(m_inputs_buffer[2])
    {
        case 0x72 : // Global modules status
        {
            // PRO2 (VC_ESC + 0x3A) + GLBOAL STATUS (0x73) + STATUS BYTE
            //#TODO global status byte
            std::uint8_t status = 0b0100000;
            status |= m_context.is_rolling_mode ? 2 : 0;
            const std::uint8_t answer[] = {VC_ESC, 0x3A, 0x73, status};
            __send_data(answer, sizeof(answer));
            return RF_OK;
        }
        case 0x7B : // ENQROM
        {
            const std::uint8_t answer[] = {VC_SOH, 'C', 'u', '<', VC_EOT};
            __send_data(answer, sizeof(answer));
            break;
        }
        default:
            return RF_IGNORED;
    }
    return RF_OK;
}

constexpr std::uint8_t PRO2_MODE_ROLLING = 0x43;
constexpr std::uint8_t PRO2_MODE_PCE = 0x44;
constexpr std::uint8_t PRO2_MODE_LOWERCASE = 0x45;
constexpr std::uint8_t PRO2_STATUS_KEYBOARD_OUT = 0x51;
constexpr std::uint8_t PRO2_STATUS_KEYBOARD_IN = 0x59;
constexpr std::uint8_t PRO2_MODE_MIXED = 0x7D;
constexpr std::uint8_t PRO2_MODE_VIDEOTEX = 0x7E;

Minitel::RESULT_FUNC Minitel::__func_PRO2(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 4;
    bool start = false;
    switch(m_inputs_buffer[2])
    {
        case 0x32 : // Protocol mode VIDEOTEX <=> MIXED
            if(m_inputs_buffer[3] == PRO2_MODE_MIXED)
            {
                // NOT IMPLEMENTED
                return RF_ERR;
            }
            else if(m_inputs_buffer[3] == PRO2_MODE_VIDEOTEX)
            {
                // We stay in VIDEOTEX => ACK
                const std::uint8_t answer [] = {VC_SEP, 0x71};
                __send_data(answer, sizeof(answer));
                return RF_OK;
            }
            return RF_ERR;
        case 0x69 : start = true; break;
        case 0x6A : start = false; break;
        case 0x72 :
        {
            // get keyboard status
            if(m_inputs_buffer[3] == PRO2_STATUS_KEYBOARD_IN)
            {
                // PRO3 (VC_ESC + 0x3B) + KEYBOARD STATUS ANSWER (0x73) + KEYBOARD_IN + KEYBOARD STATUS BYTE
                //#TODO keyboard status byte
                const std::uint8_t status = 0b11000000;
                const std::uint8_t answer [] = {VC_ESC, 0x3B, 0x73, PRO2_STATUS_KEYBOARD_IN, status};
                __send_data(answer, sizeof(answer));
                return RF_OK;
            }
        }
        default:
            return RF_ERR;
    }

    switch(m_inputs_buffer[3])
    {
        // Rolling mode
        case PRO2_MODE_ROLLING:
            m_context.is_rolling_mode = start;
            break;
        // Error Correction Procedure (PCE in datasheet)
        case PRO2_MODE_PCE:
            // TODO
            break;
        // Lowercase mode
        case PRO2_MODE_LOWERCASE:
            // TODO
            break;
        default:
            return RF_ERR;
    }

    std::uint8_t status = __get_PRO2_status_byte();
    const std::uint8_t answer[] = {VC_ESC, 0x3A, 0x73, status};
    __send_data(answer, sizeof(answer));
    return RF_OK;
}

Minitel::RESULT_FUNC Minitel::__func_PRO3(int& nb_of_byte_processed)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_processed = 5;
    // "switch/aiguillage" protocol
    const MODULE_CODE code_module_in = static_cast<MODULE_CODE>(m_inputs_buffer[3]);
    const MODULE_CODE code_module_out = static_cast<MODULE_CODE>(m_inputs_buffer[4]);
    bool activate = false;
    switch(m_inputs_buffer[2])
    {
        case 0x60: // OFF
            activate = false;
            break;
        case 0x61: // ON
            activate = true;
            break;
        default:
            return RF_ERR;
    }
    if(!__activate_module_switch(code_module_out, code_module_in, activate))
        return RF_IGNORED;

    // PRO3 (VC_ESC + 0x3B) + FROM (0x63) + MODULE_IN + SWITCH_STATUS of MODULE
    std::uint8_t module_switch_status = 0b01000000;
    module_switch_status |= __get_module_switch_state(MODULE_CODE::SCREEN_IN,code_module_in)<<0;
    module_switch_status |= __get_module_switch_state(MODULE_CODE::KEYBOARD_IN,code_module_in)<<1;
    module_switch_status |= __get_module_switch_state(MODULE_CODE::MODEM_IN,code_module_in)<<2;
    module_switch_status |= __get_module_switch_state(MODULE_CODE::DIN_IN,code_module_in)<<3;

    const std::uint8_t answer[] = {VC_ESC, 0x3B, 0x63, static_cast<std::uint8_t>(code_module_in), module_switch_status};
    __send_data(answer, sizeof(answer));
    return RF_OK;
}
