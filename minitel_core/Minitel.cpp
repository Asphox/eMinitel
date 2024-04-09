//
// Created by asphox on 12/26/23.
//

#include "Minitel.h"
#include "VideotexCodes.h"

using namespace mtlc;

constexpr GLYPH_CODE F_CONNECT_00 = set_glyph_att_color(GC_F, GFC_BLACK, GBC_WHITE);

Minitel::Minitel()
{
    reset();
}

void Minitel::set_screen_control(mtlc_module_control_screen screen, void* ctx)
{
    m_screen_control = screen;
    m_screen_ctx = ctx;
    reset();
}
void Minitel::set_din_control(mtlc_module_control_DIN din, void* ctx)
{
    m_DIN_control = din;
    m_DIN_ctx = ctx;
}
void Minitel::set_keyboard_control(mtlc_module_control_keyboard keyboard, void* ctx)
{
    m_keyboard_control = keyboard;
    m_keyboard_ctx = ctx;
}

// press a key (from __pull_keyboard or external call)
void Minitel::key_event(mtlc_KeyEvent ke)
{
    // if keyboard is off => does nothing
    if(!__get_module_switch_state(MODULE_CODE::KEYBOARD_OUT, MODULE_CODE::KEYBOARD_IN))
        return;
    if(ke.kc == KC_NONE)
        return;
    bool shift = ke.rshft || ke.lshft;

    VDTX_CODE vdtx_code = make_vdtx_code(0);

    // letters
    if(ke.kc >= KC_A && ke.kc <= KC_Z)
        vdtx_code = static_cast<VDTX_CODE>(VC_A + (ke.kc - KC_A) + (shift ? 32 : 0));
        // numeric pad
    else if(ke.kc >= KC_0 && ke.kc <= KC_9)
    {
        if(shift)
        {
            GLYPH_CODE gc;
            switch(ke.kc)
            {
                case KC_0 : vdtx_code = VC_UPARROW; break;
                case KC_1 : vdtx_code = VC_EXCL; break;
                case KC_2 : vdtx_code = VC_QUOT; break;
                case KC_3 : vdtx_code = VC_NUM; break;
                case KC_4 : vdtx_code = VC_DOLLAR; break;
                case KC_5 : vdtx_code = VC_PERCNT; break;
                case KC_6 : vdtx_code = VC_AMP; break;
                case KC_7 : vdtx_code = VC_APOS; break;
                case KC_8 : vdtx_code = VC_LPAR; break;
                case KC_9 : vdtx_code = VC_RPAR; break;
            }
        }
        else
            vdtx_code = static_cast<VDTX_CODE>(VC_0 + (ke.kc - KC_0));
    }
    else
    {
        GLYPH_CODE gc;
        switch(ke.kc)
        {
            case KC_AST     : vdtx_code = (shift) ? VC_LSQB : VC_AST;   break;
            case KC_NUM     : vdtx_code = (shift) ? VC_RSQB    : VC_NUM;       break;
            case KC_COMMA   : vdtx_code = (shift) ? VC_LT      : VC_COMMA;     break;
            case KC_POINT   : vdtx_code = (shift) ? VC_GT      : VC_PERIOD;    break;
            case KC_APOS    : vdtx_code = (shift) ? VC_AT      : VC_APOS;      break;
            case KC_SEMI    : vdtx_code = (shift) ? VC_PLUS    : VC_SEMI;      break;
            case KC_MINUS   : vdtx_code = (shift) ? VC_EQUALS  : VC_MINUS;     break;
            case KC_COLON   : vdtx_code = (shift) ? VC_AST     : VC_COLON;     break;
            case KC_QUEST   : vdtx_code = (shift) ? VC_SLASH   : VC_QUEST;     break;
            case KC_SPACE   : vdtx_code = VC_SPACE; break;
            case KC_GUIDE   : vdtx_code = VC_GUIDE; break;
            case KC_RETURN  : vdtx_code = VC_PREVIOUS; break;
            case KC_NEXT    : vdtx_code = VC_NEXT; break;
            case KC_SEND    : vdtx_code = VC_SEND; break;
        }
    }

    const std::uint8_t* data = reinterpret_cast<std::uint8_t*>(&vdtx_code);
    std::size_t size = get_vdtx_code_size(vdtx_code);
    __process_switch(MODULE_CODE::KEYBOARD_OUT, data, size);
}

void Minitel::reset()
{
    m_context.reset();
    m_inputs_buffer.clear();
    if(m_screen_control)
    {
        m_screen_control(OPS_UPDATE, 0, m_screen_ctx);
        m_screen_control(OPS_CLEAR, 0, m_screen_ctx);
        m_screen_control(OPS_SET_CURSOR_VISIBLE, false, m_screen_ctx);
        m_cursor_pos = CursorPos();
        __set_cursor_pos(m_cursor_pos);
        mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
        param_set_glyph_at.pos.col = 38;
        param_set_glyph_at.pos.line = 0;
        param_set_glyph_at.gc = F_CONNECT_00;
        m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
    }
}

void Minitel::update()
{
    __process_inputs();
    if(m_screen_control)
        m_screen_control(OPS_UPDATE, 0, m_screen_ctx);
}

void Minitel::__pull_data()
{
    const char* data = nullptr;
    if(m_DIN_control)
    {
        std::uint16_t size = m_DIN_control(OPD_PULL_DATA, (intptr_t)&data, m_DIN_ctx);
        if(size == 0 || data == nullptr)
            return;

        // if DIN is OFF => we don't push data
        if(!__get_module_switch_state(MODULE_CODE::DIN_OUT, MODULE_CODE::DIN_IN))
            return;

        for(std::size_t i=0; i<size; i++)
            m_inputs_buffer.push_back(data[i]);
    }
}

void Minitel::__send_data(const std::uint8_t* data, std::size_t size)
{
    if(m_DIN_control)
    {
        mtlc_op_din_param_send_data param_send_data;
        param_send_data.size = size;
        param_send_data.data = data;
        m_DIN_control(OPD_SEND_DATA, (intptr_t)&param_send_data, m_DIN_ctx);
    }
}

void Minitel::__pull_keyboard()
{
    if(!m_keyboard_control)
        return;
    mtlc_KeyEvent ke = m_keyboard_control(OPK_PULL_KEY, 0, m_keyboard_ctx);
    key_event(ke);
}

void Minitel::__process_switch(MODULE_CODE out, const std::uint8_t* data, std::size_t size)
{
    switch(out)
    {
        case MODULE_CODE::KEYBOARD_OUT:
        {
            if(__get_module_switch_state(out, MODULE_CODE::DIN_IN))
                __process_switch_din(data, size);
            if(__get_module_switch_state(out, MODULE_CODE::MODEM_IN))
                __process_switch_modem(data, size);
        }break;
        case MODULE_CODE::DIN_OUT:
        {
            if(__get_module_switch_state(out, MODULE_CODE::SCREEN_IN))
                __process_switch_screen(data, size);
        }break;
        case MODULE_CODE::MODEM_OUT:
        {
            if(!__get_module_switch_state(out, MODULE_CODE::MODEM_IN))
                return;
            if(__get_module_switch_state(out, MODULE_CODE::SCREEN_IN))
                __process_switch_screen(data, size);
        }break;
    }
}

void Minitel::__process_switch_modem(const std::uint8_t* data, std::size_t size)
{
    // MODEM IS NOT IMPLEMENTED
    // ONLY REDIRECT TO SCREEN
    __process_switch(MODULE_CODE::MODEM_OUT, data, size);
}

void Minitel::__process_switch_screen(const std::uint8_t* data, std::size_t size)
{
    __write_char(data[0]);
}

void Minitel::__process_switch_din(const std::uint8_t* data, std::size_t size)
{
    __send_data(data, size);
}

void Minitel::__process_inputs()
{
    __pull_keyboard();
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
    if(c >= __VC_CONTROL_MIN && c <= __VC_CONTROL_MAX)
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
    // if screen is OFF => does nothing
    if(!__get_module_switch_state(MODULE_CODE::SCREEN_OUT, MODULE_CODE::SCREEN_IN))
        return;

    CursorPos pos = __get_cursor_pos();
    if(m_screen_control)
    {
        mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
        param_set_glyph_at.gc = gc;
        param_set_glyph_at.pos.col = pos.col;
        param_set_glyph_at.pos.line = pos.line;
        m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
    }
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
    if(m_screen_control)
    {
        mtlc_op_screen_param_set_cursor_pos param_set_cursor_pos;
        param_set_cursor_pos.col = m_cursor_pos.col;
        param_set_cursor_pos.line = m_cursor_pos.line;
        m_screen_control(OPS_SET_CURSOR_POS, (intptr_t)&param_set_cursor_pos, m_screen_ctx);
    }
}

void Minitel::__write_char(char code)
{
    // if screen is OFF => does nothing
    if(!__get_module_switch_state(MODULE_CODE::SCREEN_OUT, MODULE_CODE::SCREEN_IN))
        return;

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

    if(m_screen_control)
    {
        mtlc_op_screen_param_set_glyph_at param_set_glyph_at;
        param_set_glyph_at.gc = gc;
        param_set_glyph_at.pos.col = pos.col;
        param_set_glyph_at.pos.line = pos.line;
        m_screen_control(OPS_SET_GLYPH_AT, (intptr_t)&param_set_glyph_at, m_screen_ctx);
    }
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
            if(m_context.is_rolling_mode)
            {
                if(m_screen_control)
                {
                    std::int8_t nb_line = 1;
                    if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                        nb_line = 2;
                    m_screen_control(OPS_SCROLL, nb_line, m_screen_ctx);
                }
                pos.col = 39;
            }
            else
            {
                pos.line = 24;
                pos.col = 39;
            }
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
            if(m_context.is_rolling_mode)
            {
                if(m_screen_control)
                {
                    std::int8_t nb_line = -1;
                    if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                        nb_line = -2;
                    m_screen_control(OPS_SCROLL, nb_line, m_screen_ctx);
                }
                pos.col = 0;
            }
            else
            {
                pos.line = 1;
                pos.col = 0;
            }
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
            if(m_context.is_rolling_mode && m_screen_control)
            {
                std::int8_t nb_line = -1;
                if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                    nb_line = -2;
                m_screen_control(OPS_SCROLL, nb_line, m_screen_ctx);
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
        if(m_context.is_rolling_mode && m_screen_control)
        {
            std::int8_t nb_line = 1;
            if(m_context.current_char_attributes.size == GS_DOUBLE_SIZE || m_context.current_char_attributes.size == GS_DOUBLE_HEIGHT)
                nb_line = 2;
            m_screen_control(OPS_SCROLL, nb_line, m_screen_ctx);
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

