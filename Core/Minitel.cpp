//
// Created by asphox on 12/26/23.
//

#include "Minitel.h"
#include "../Utils/Constants.h"

constexpr GLYPH_CODE F_CONNECT_00 = set_glyph_att_color(GC_F, GFC_BLACK, GBC_WHITE);

Minitel::Minitel(float height) : m_screen(height)
{
    reset();
}

void Minitel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_screen, states);
}

void Minitel::reset()
{
    m_context.reset();
    m_inputs_buffer.clear();
    m_screen.update();
    m_screen.clear();
    m_screen.set_cursor_visible(false);
    m_cursor_pos = CursorPos();
    m_screen.set_cursor_pos(0, 1);
    m_screen.set_glyph_at(F_CONNECT_00, 38, 0);
}

void Minitel::update()
{
    __process_inputs();
    m_screen.update_blink();
}

void Minitel::__pull_data_from_inputs()
{
    // tests data
    static std::uint8_t test = 0;
    m_inputs_buffer.push_back(VC_FF);
    m_inputs_buffer.push_back(VC_LF);
    m_inputs_buffer.push_back(VC_ESC);
    m_inputs_buffer.push_back(0x4F);
    m_inputs_buffer.push_back(VC_ESC);
    m_inputs_buffer.push_back(0x45);
    m_inputs_buffer.push_back('H');
    m_inputs_buffer.push_back('e');
    m_inputs_buffer.push_back('l');
    m_inputs_buffer.push_back('l');
    m_inputs_buffer.push_back('o');
}

void Minitel::__process_inputs()
{
    __pull_data_from_inputs();
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
    m_screen.set_glyph_at(gc, pos.col, pos.line);
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
    m_screen.set_cursor_pos(m_cursor_pos.col, m_cursor_pos.line);
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

    // we need to apply current char and zone attributes
    gc = set_glyph_att_fcolor(gc, m_context.current_char_attributes.fcolor);
    gc = set_glyph_att_blink(gc, m_context.current_char_attributes.blink);
    gc = set_glyph_att_negative(gc, m_context.current_char_attributes.negative);
    // we can't apply double height and double size on line 00
    if(pos.line != 0 || (pos.line == 0 && m_context.current_char_attributes.size != GS_DOUBLE_SIZE && m_context.current_char_attributes.size != GS_DOUBLE_HEIGHT))
        gc = set_glyph_att_size(gc, m_context.current_char_attributes.size);
    m_context.last_glyph = gc;

    m_screen.set_glyph_at(gc, pos.col, pos.line);
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
            pos.line--;
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
            pos.line = 1;
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
        if(loop)
            pos.line = 24;
    }
    else
        pos.line--;

    __set_cursor_pos(pos);
}

//==================================================================
// Functionalities
//==================================================================

Minitel::RESULT_FUNC Minitel::__func_BS(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    __push_cursor_left(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_HT(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    __push_cursor_right(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_LF(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    __push_cursor_down(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_VT(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    __push_cursor_up(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_FF(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    m_screen.clear(true);
    __set_cursor_pos(CursorPos());
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CR(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    CursorPos pos = __get_cursor_pos();
    pos.col = 0;
    __set_cursor_pos(pos);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SO(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    m_context.is_escaped_to_G1 = true;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SI(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    m_context.is_escaped_to_G1 = false;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CON(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    m_screen.set_cursor_visible(true);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_COFF(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    m_screen.set_cursor_visible(false);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_CAN(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    CursorPos old_pos = __get_cursor_pos();
    for(int col = old_pos.col; col<40; col++)
        __write_glyph(GC_SPACE);
    __set_cursor_pos(old_pos);
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_SS2(int& nb_of_byte_concerned)
{
    //todo accent
    if(m_inputs_buffer.size() < 2)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 2;
    if(m_context.is_escaped_to_G1)
        return RF_IGNORED;

    m_context.is_escaped_to_G2 = true;
    __write_char(m_inputs_buffer[1]);
    m_context.is_escaped_to_G2 = false;
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_REP(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 2)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 2;
    char code = m_inputs_buffer[1];
    if(code < 0x40)
        return RF_ERR;

    code -= 0x40;
    for(int i=0; i<code; i++)
        __write_glyph(m_context.last_glyph);

    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_RS(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    CursorPos pos = __get_cursor_pos();
    pos.line = 1;
    pos.col = 0;
    __set_cursor_pos(pos);
    m_context.reset_article();
    return RF_OK;
}
Minitel::RESULT_FUNC Minitel::__func_US(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 3;

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
Minitel::RESULT_FUNC Minitel::__func_write_char(int& nb_of_byte_concerned)
{
    nb_of_byte_concerned = 1;
    __write_char(m_inputs_buffer[0]);
    return RF_OK;
}

//==================================================================
// Escaped Functionalities
//==================================================================

Minitel::RESULT_FUNC Minitel::__func_ESC(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 2)
        return  RF_INSUFFICIENT_DATA;
    // if we have an escapement to ISO 6429
    if(m_inputs_buffer[1] == 0x5B)
        return __func_CSI(nb_of_byte_concerned);
    // attributes
    nb_of_byte_concerned = 2;
    switch(m_inputs_buffer[1])
    {
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
        case 0x50 : /*todo attribues zone*/ break;
        case 0x51 : /*todo attribues zone*/ break;
        case 0x52 : /*todo attribues zone*/ break;
        case 0x53 : /*todo attribues zone*/ break;
        case 0x54 : /*todo attribues zone*/ break;
        case 0x55 : /*todo attribues zone*/ break;
        case 0x56 : /*todo attribues zone*/ break;
        case 0x57 : /*todo attribues zone*/ break;
        case 0x58 : /*todo attribues zone*/ break;
        case 0x59 : /*todo attribues zone*/ break;
        case 0x5A : /*todo attribues zone*/ break;
        case 0x5C : m_context.current_char_attributes.negative = false; break;
        case 0x5D : m_context.current_char_attributes.negative = true; break;
        case 0x5F : /*todo attribues zone*/ break;
        default: return RF_ERR;
    }

    return RF_OK;
}
// called by __func_ESC during ISO 6429 escaping
Minitel::RESULT_FUNC Minitel::__func_CSI(int& nb_of_byte_concerned)
{
    // CSI functionalities that only use 3 bytes
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 3;
    switch(m_inputs_buffer[2])
    {
        case 0x4A : return __func_CSI_J(nb_of_byte_concerned);
        case 0x4B : return __func_CSI_K(nb_of_byte_concerned);
        default:    break;
    }

    // CSI functionalities that use 4 bytes
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    switch(m_inputs_buffer[3])
    {
        case 0x4A :
        {
            switch(m_inputs_buffer[2])
            {
                case 0x30 : return __func_CSI_0J(nb_of_byte_concerned);
                case 0x31 : return __func_CSI_1J(nb_of_byte_concerned);
                case 0x32 : return __func_CSI_2J(nb_of_byte_concerned);
                default:    return RF_ERR;
            }
        }
        case 0x4B :
        {
            switch(m_inputs_buffer[2])
            {
                case 0x30 : return __func_CSI_0K(nb_of_byte_concerned);
                case 0x31 : return __func_CSI_1K(nb_of_byte_concerned);
                case 0x32 : return __func_CSI_2K(nb_of_byte_concerned);
                default:    return RF_ERR;
            }
        }
        case 0x68 : return __func_CSI_h(nb_of_byte_concerned);
        case 0x6C : return __func_CSI_i(nb_of_byte_concerned);
        default:    break;
    }

    // CSI functionalities that use 5 bytes
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    switch(m_inputs_buffer[4])
    {
        case 0x40: return RF_IGNORED;   // only for RTIC Minitel (not implemented in this emulator)
        case 0x41: return __func_CSI_A(nb_of_byte_concerned);
        case 0x42: return __func_CSI_B(nb_of_byte_concerned);
        case 0x43: return __func_CSI_C(nb_of_byte_concerned);
        case 0x44: return __func_CSI_D(nb_of_byte_concerned);
        case 0x4C: return __func_CSI_L(nb_of_byte_concerned);
        case 0x4D: return __func_CSI_M(nb_of_byte_concerned);
        case 0x50: return __func_CSI_P(nb_of_byte_concerned);
        default:   break;
    }

    // CSI functionalities that use 8 bytes
    if(m_inputs_buffer.size() < 8)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 8;
    if(m_inputs_buffer[4] == 0x3B && m_inputs_buffer[7] == 0x48)
        return __func_CSI_H(nb_of_byte_concerned);

    // if we are here, there is a problem ...
    return RF_ERR;
}

// clears from cursor included to end of screen
Minitel::RESULT_FUNC Minitel::__func_CSI_J(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 3;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    for(int line=pos.line; line<24; line++)
    {
        for(int col=pos.col; col<39; col++)
            m_screen.set_glyph_at(GC_SPACE, col, line);
    }
    return RF_IGNORED;
}
// same as __func_CSI_J
Minitel::RESULT_FUNC Minitel::__func_CSI_0J(int& nb_of_byte_concerned)
{
    RESULT_FUNC r = __func_CSI_J(nb_of_byte_concerned);
    nb_of_byte_concerned = 4;
    return r;
}
// clears from screen's start to cursor included
Minitel::RESULT_FUNC Minitel::__func_CSI_1J(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;

    for(int line=1; line<=pos.line; line++)
    {
        for(int col=0; col<=pos.col; col++)
            m_screen.set_glyph_at(GC_SPACE, col, line);
    }
    return RF_OK;
}
// clears all screen (cursor is not moved)
Minitel::RESULT_FUNC Minitel::__func_CSI_2J(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    m_screen.clear(true);
    return RF_OK;
}
// clears from cursor included to end of line
Minitel::RESULT_FUNC Minitel::__func_CSI_K(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 3)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 3;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// same as __func_CSI_K
Minitel::RESULT_FUNC Minitel::__func_CSI_0K(int& nb_of_byte_concerned)
{
    RESULT_FUNC r = __func_CSI_K(nb_of_byte_concerned);
    nb_of_byte_concerned = 4;
    return r;
}
// clears from current line's start to cursor (included)
Minitel::RESULT_FUNC Minitel::__func_CSI_1K(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    for(int col=0; col<=pos.col; col++)
        m_screen.set_glyph_at(GC_SPACE, col, pos.line);
    return RF_OK;
}
// clears all the current line
Minitel::RESULT_FUNC Minitel::__func_CSI_2K(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    for(int col=0; col<39; col++)
        m_screen.set_glyph_at(GC_SPACE, col, pos.line);
    return RF_OK;
}
// cursor goes up n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_A(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    char dec = m_inputs_buffer[2];
    char unit = m_inputs_buffer[3];
    if(!isdigit(dec) || !isdigit(unit))
        return RF_ERR;
    char n = (dec-'0')*10 + (unit-'0');
    for(int i=0; i<n; i++)
        __push_cursor_up(false);
    return RF_OK;
}
// cursor goes down n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_B(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    char dec = m_inputs_buffer[2];
    char unit = m_inputs_buffer[3];
    if(!isdigit(dec) || !isdigit(unit))
        return RF_ERR;
    char n = (dec-'0')*10 + (unit-'0');
    for(int i=0; i<n; i++)
        __push_cursor_down(false);
    return RF_OK;
}
// cursor goes right n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_C(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    char dec = m_inputs_buffer[2];
    char unit = m_inputs_buffer[3];
    if(!isdigit(dec) || !isdigit(unit))
        return RF_ERR;
    char n = (dec-'0')*10 + (unit-'0');
    for(int i=0; i<n; i++)
        __push_cursor_right(false);
    return RF_OK;
}
// cursor goes left n-times, no looping
Minitel::RESULT_FUNC Minitel::__func_CSI_D(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    char dec = m_inputs_buffer[2];
    char unit = m_inputs_buffer[3];
    if(!isdigit(dec) || !isdigit(unit))
        return RF_ERR;
    char n = (dec-'0')*10 + (unit-'0');
    for(int i=0; i<n; i++)
        __push_cursor_left(false);
    return RF_OK;
}
// direct addressing for cursor
Minitel::RESULT_FUNC Minitel::__func_CSI_H(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 8)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 8;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    char line_digit = m_inputs_buffer[2];
    char line_unit = m_inputs_buffer[3];
    char col_digit = m_inputs_buffer[5];
    char col_unit = m_inputs_buffer[6];
    char line = (line_digit-'0')*10 + (line_unit-'0');
    char col  = (col_digit-'0')*10 + (col_unit-'0');
    if(line < 0 || line > 24 || col < 0 || col > 39)
        return RF_ERR;
    pos.line = line;
    pos.col = col;
    __set_cursor_pos(pos);
    return RF_OK;
}
// inserts n lines from current cursor line
Minitel::RESULT_FUNC Minitel::__func_CSI_L(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// deletes n lines from current cursor line
Minitel::RESULT_FUNC Minitel::__func_CSI_M(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// deletes n chars starting on cursor included
Minitel::RESULT_FUNC Minitel::__func_CSI_P(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 5)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 5;
    CursorPos pos = __get_cursor_pos();
    // no CSI on line 00
    if(pos.line == 0)
        return RF_IGNORED;
    // todo
    return RF_IGNORED;
}
// starts char-insertion mode
Minitel::RESULT_FUNC Minitel::__func_CSI_h(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    //todo
    return RF_OK;
}
// ends char-insertion mode
Minitel::RESULT_FUNC Minitel::__func_CSI_i(int& nb_of_byte_concerned)
{
    if(m_inputs_buffer.size() < 4)
        return RF_INSUFFICIENT_DATA;
    nb_of_byte_concerned = 4;
    // no CSI on line 00
    CursorPos pos = __get_cursor_pos();
    if(pos.line == 0)
        return RF_IGNORED;
    //todo
    return RF_OK;
}
