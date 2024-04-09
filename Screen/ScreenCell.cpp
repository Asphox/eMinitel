// ScreenCell.cpp
// Created by asphox on 26/12/2023.
// Manages the drawing of one glyph on screen

#include "ScreenCell.h"
#include "Font.h"
#include "Screen.h"
#include <utility>

ScreenCell::ScreenCell(const Screen& screen) : m_screen(screen)
{
    m_text_glyph.setFont(get_font());
    m_text_glyph.setScale(0.80, 0.80);
    set_glyph(m_glyph);
}

//ScreenCell::ScreenCell(const ScreenCell& src) : m_screen(src.m_screen)
//{
//    m_size = src.m_size;
//    m_background = src.m_background;
//    m_underline = src.m_underline;
//    m_text_glyph = src.m_text_glyph;
//    m_mosaic = src.m_mosaic;
//    m_glyph = src.m_glyph;
//}

ScreenCell& ScreenCell::operator=(const ScreenCell& src)
{
    m_size = src.m_size;
    m_background = src.m_background;
    m_underline = src.m_underline;
    m_text_glyph = src.m_text_glyph;
    m_mosaic = src.m_mosaic;
    m_glyph = src.m_glyph;
    return *this;
}

#define __CASE_GLYPH_TO_FONT_CODEPOINT(gc, cp) case gc : return cp

// converts a glyph to font's codepoint
std::uint32_t ScreenCell::__get_glyph_codepoint(GLYPH_CODE gc)
{
    GLYPH_CHARSET cs = get_glyph_charset(gc);
    if(cs == G1)
        return 0;
    GLYPH_CODE gc_no_attributes = get_glyph_charset_accent_code(gc);
    switch(gc_no_attributes)
    {
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_EXCL          ,'!');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_QUOT          ,'"');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_NUM           ,'#');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_DOLLAR        ,'$');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_PERCNT        ,'%');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_AMP           ,'&');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_APOS          ,'\'');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LPAR          ,'(');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_RPAR          ,')');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_AST           ,'*');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_PLUS          ,'+');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_COMMA         ,',');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_MINUS         ,'-');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_PERIOD        ,'.');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_SLASH         ,'/');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_0             ,'0');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_1             ,'1');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_2             ,'2');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_3             ,'3');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_4             ,'4');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_5             ,'5');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_6             ,'6');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_7             ,'7');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_8             ,'8');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_9             ,'9');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_COLON         ,':');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_SEMI          ,';');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LT            ,'<');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_EQUALS        ,'=');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_GT            ,'>');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_QUEST         ,'?');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_AT            ,'@');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_A             ,'A');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_A_CIRC        ,'A');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_B             ,'B');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_C             ,'C');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_C_CEDILLA     ,'C');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_D             ,'D');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_E             ,'E');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_E_GRAVE       ,'E');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_E_ACUTE       ,'E');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_F             ,'F');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_G             ,'G');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_H             ,'H');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_I             ,'I');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_J             ,'J');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_K             ,'K');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_L             ,'L');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_M             ,'M');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_N             ,'N');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_O             ,'O');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_P             ,'P');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_Q             ,'Q');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_R             ,'R');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_S             ,'S');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_T             ,'T');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_U             ,'U');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_V             ,'V');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_W             ,'W');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_X             ,'X');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_Y             ,'Y');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_Z             ,'Z');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LSQB          ,'[');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_BSLASH        ,'\\');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_RSQB          ,L']');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_UPARROW       ,0x5E);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_UNDRSCR       ,'_');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_MIDLSCR       ,0x85);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_a             ,'a');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_a_CIRC        ,0xE2);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_a_GRAVE       ,0xE0);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_a_DIAERESIS   ,0xE4);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_b             ,'b');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_c             ,'c');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_c_CEDILLA     ,0xE7);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_d             ,'d');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_e             ,'e');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_e_CIRC        ,0xEA);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_e_GRAVE       ,0xE8);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_e_ACUTE       ,0xE9);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_e_DIAERESIS   ,0xEB);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_f             ,'f');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_g             ,'g');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_h             ,'h');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_i             ,'i');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_i_CIRC        ,0xEE);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_i_DIAERESIS   ,0xEF);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_j             ,'j');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_k             ,'k');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_l             ,'l');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_m             ,'m');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_n             ,'n');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_o             ,'o');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_o_CIRC        ,0xF4);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_o_DIAERESIS   ,0xF6);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_p             ,'p');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_q             ,'q');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_r             ,'r');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_s             ,'s');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_t             ,'t');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_u             ,'u');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_u_CIRC        ,0xFB);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_u_GRAVE       ,0xF9);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_u_DIAERESIS   ,0xFC);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_v             ,'v');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_w             ,'w');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_x             ,'x');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_y             ,'y');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_z             ,'z');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LVERBAR       ,0x83);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_VERBAR        ,'|');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_RVERBAR       ,0x84);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_UPRSCR        ,0x86);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_POUND         ,0xA3);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_DOLLAR_G2     ,'$');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_NUM_G2        ,'#');
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_SECT          ,0xA7);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LARROW        ,0x81);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_UPARROW_G2    ,0x5E);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_RARROW        ,0x82);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_DARROW        ,0x80);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_DEG           ,0xB0);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_PLUSMN        ,0xB1);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_DIV           ,0xF7);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_FRAC14        ,0xBC);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_FRAC12        ,0xBD);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_FRAC34        ,0xBE);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LIG_OE        ,0x87);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_LIG_oe        ,0x88);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_ESZETT        ,0xDF);
        __CASE_GLYPH_TO_FONT_CODEPOINT(GC_SPACE         , ' ');
        default :
            return 0;
    }
}

void ScreenCell::set_glyph(GLYPH_CODE gc)
{
    m_glyph = gc;
    GLYPH_ATT_FCOLOR fc = get_glyph_att_fcolor(gc);
    sf::Color sf_fc;
    GLYPH_ATT_BCOLOR bc = get_glyph_att_bcolor(gc);
    sf::Color sf_bc;
    bool underline = get_glyph_att_underline(gc);
    GLYPH_CHARSET cs = get_glyph_charset(gc);
    bool color = m_screen.get_color_mode();

    constexpr float GREY_SCALE_BLUE = 0.2f;
    constexpr float GREY_SCALE_CYAN = 0.7f;
    constexpr float GREY_SCALE_GREEN = 0.6f;
    constexpr float GREY_SCALE_MAGENTA = 0.5f;
    constexpr float GREY_SCALE_RED = 0.4f;
    constexpr float GREY_SCALE_YELLOW = 0.8f;

    switch(fc)
    {
        case GFC_BLACK:      sf_fc = sf::Color::Black; break;
        case GFC_BLUE:       sf_fc = (color) ? sf::Color::Blue      : sf::Color(GREY_SCALE_BLUE*255, GREY_SCALE_BLUE*255, GREY_SCALE_BLUE*255); break;
        case GFC_CYAN:       sf_fc = (color) ? sf::Color::Cyan      : sf::Color(GREY_SCALE_CYAN*255, GREY_SCALE_CYAN*255, GREY_SCALE_CYAN*255); break;
        case GFC_GREEN:      sf_fc = (color) ? sf::Color::Green     : sf::Color(GREY_SCALE_GREEN*255, GREY_SCALE_GREEN*255, GREY_SCALE_GREEN*255); break;
        case GFC_MAGENTA:    sf_fc = (color) ? sf::Color::Magenta   : sf::Color(GREY_SCALE_MAGENTA*255, GREY_SCALE_MAGENTA*255, GREY_SCALE_MAGENTA*255); break;
        case GFC_RED:        sf_fc = (color) ? sf::Color::Red       : sf::Color(GREY_SCALE_RED*255, GREY_SCALE_RED*255, GREY_SCALE_RED*255); break;
        case GFC_YELLOW:     sf_fc = (color) ? sf::Color::Yellow    : sf::Color(GREY_SCALE_YELLOW*255, GREY_SCALE_YELLOW*255, GREY_SCALE_YELLOW*255); break;
        case GFC_WHITE:      sf_fc = sf::Color::White; break;
    }
    switch(bc)
    {
        case GBC_BLACK:      sf_bc = sf::Color::Black; break;
        case GBC_BLUE:       sf_bc = (color) ? sf::Color::Blue      : sf::Color(GREY_SCALE_BLUE*255, GREY_SCALE_BLUE*255, GREY_SCALE_BLUE*255); break;
        case GBC_CYAN:       sf_bc = (color) ? sf::Color::Cyan      : sf::Color(GREY_SCALE_CYAN*255, GREY_SCALE_CYAN*255, GREY_SCALE_CYAN*255); break;
        case GBC_GREEN:      sf_bc = (color) ? sf::Color::Green     : sf::Color(GREY_SCALE_GREEN*255, GREY_SCALE_GREEN*255, GREY_SCALE_GREEN*255); break;
        case GBC_MAGENTA:    sf_bc = (color) ? sf::Color::Magenta   : sf::Color(GREY_SCALE_MAGENTA*255, GREY_SCALE_MAGENTA*255, GREY_SCALE_MAGENTA*255); break;
        case GBC_RED:        sf_bc = (color) ? sf::Color::Red       : sf::Color(GREY_SCALE_RED*255, GREY_SCALE_RED*255, GREY_SCALE_RED*255); break;
        case GBC_YELLOW:     sf_bc = (color) ? sf::Color::Yellow    : sf::Color(GREY_SCALE_YELLOW*255, GREY_SCALE_YELLOW*255, GREY_SCALE_YELLOW*255); break;
        case GBC_WHITE:      sf_bc = sf::Color::White; break;
    }

    // switches foreground and background color if negative
    // not available for mosaic
    if(cs != G1 && get_glyph_att_negative(gc))
        std::swap(sf_bc, sf_fc);

    m_text_glyph.setFillColor(sf_fc);
    m_underline.setFillColor(sf_fc);
    m_mosaic.setFillColor(sf_fc);
    m_background.setFillColor(sf_bc);

    // if glyph's charset is not mosaic
    if(cs != G1)
    {
        std::uint32_t font_code = __get_glyph_codepoint(gc);
        m_text_glyph.setString(font_code);
        switch(get_glyph_att_size(gc))
        {
            case GS_NORMAL:         m_text_glyph.setScale({0.80, 0.80});
                                    m_text_glyph.setOrigin({0, 0});
                                    m_background.setSize(m_size);
                                    m_background.setOrigin({0, 0});
                                    m_underline.setPosition(0, m_size.y*0.9);
                                    m_underline.setScale({1, 1});
                                    break;
            case GS_DOUBLE_WIDTH:   m_text_glyph.setScale({1.6, 0.80});
                                    m_text_glyph.setOrigin({0, 0});
                                    m_background.setScale({2, 1});
                                    m_background.setOrigin({0, 0});
                                    m_underline.setPosition(0, m_size.y*0.9);
                                    m_underline.setScale({2, 1});
                                    break;
            case GS_DOUBLE_HEIGHT:  m_text_glyph.setScale({0.8, 1.6});
                                    m_text_glyph.setOrigin({0, m_size.y/2});
                                    m_background.setScale({1, 2});
                                    m_background.setOrigin({0, m_size.y/2});
                                    m_underline.setPosition(0, m_size.y*0.9);
                                    m_underline.setScale({1, 2});
                                    break;
            case GS_DOUBLE_SIZE:    m_text_glyph.setScale({1.6, 1.6});
                                    m_text_glyph.setOrigin({0, m_size.y/2});
                                    m_background.setScale({2, 2});
                                    m_background.setOrigin({0, m_size.y/2});
                                    m_underline.setPosition(0, m_size.y*0.9);
                                    m_underline.setScale({2, 2});
                                    break;
        }
    }
    else
    {
        std::uint8_t mosaic_binary = get_glyph_mosaic_binary(gc);
        m_mosaic.set_value(mosaic_binary, underline);
    }
}

void ScreenCell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);

    bool blink = get_glyph_att_blink(m_glyph);
    bool blink_state = m_screen.get_blink_state();
    bool negative    = get_glyph_att_negative(m_glyph);
    bool underline   = get_glyph_att_underline(m_glyph);
    // only draws if no blink attribute or valid blink state and not masked
    if(!m_screen.get_masked() && (!blink || blink && (blink_state == negative)))
    {
        if(get_glyph_charset(m_glyph) == G1)
            target.draw(m_mosaic, states);
        else
        {
            target.draw(m_text_glyph, states);
            if(underline && !is_glyph_zone_delimiter(m_glyph))
                target.draw(m_underline, states);
        }
    }
}

void ScreenCell::setSize(const sf::Vector2f &size)
{
    m_size = size;
    m_background.setSize(size);
    m_underline.setSize({size.x, size.y/10});
    m_mosaic.set_size(size);
    m_text_glyph.setCharacterSize(size.y);
}