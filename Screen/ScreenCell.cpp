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

void ScreenCell::set_glyph(GLYPH_CODE gc)
{
    m_glyph = gc;
    GLYPH_ATT_FCOLOR fc = get_glyph_att_fcolor(gc);
    sf::Color sf_fc;
    GLYPH_ATT_BCOLOR bc = get_glyph_att_bcolor(gc);
    sf::Color sf_bc;
    bool underline = get_glyph_att_underline(gc);
    GLYPH_CHARSET cs = get_glyph_charset(gc);
    bool color = true; // TODO color vs grey
    switch(fc)
    {
        case GFC_BLACK:      sf_fc = sf::Color::Black; break;
        case GFC_BLUE:       sf_fc = (color) ? sf::Color::Blue      : sf::Color(0.4*255, 0.4*255, 0.4*255); break;
        case GFC_CYAN:       sf_fc = (color) ? sf::Color::Cyan      : sf::Color(0.8*255, 0.8*255, 0.8*255); break;
        case GFC_GREEN:      sf_fc = (color) ? sf::Color::Green     : sf::Color(0.7*255, 0.7*255, 0.7*255); break;
        case GFC_MAGENTA:    sf_fc = (color) ? sf::Color::Magenta   : sf::Color(0.6*255, 0.6*255, 0.6*255); break;
        case GFC_RED:        sf_fc = (color) ? sf::Color::Red       : sf::Color(0.5*255, 0.5*255, 0.5*255); break;
        case GFC_YELLOW:     sf_fc = (color) ? sf::Color::Yellow    : sf::Color(0.9*255, 0.9*255, 0.9*255); break;
        case GFC_WHITE:      sf_fc = sf::Color::White; break;
    }
    switch(bc)
    {
        case GBC_BLACK:      sf_bc = sf::Color::Black; break;
        case GBC_BLUE:       sf_bc = (color) ? sf::Color::Blue      : sf::Color(0.4*255, 0.4*255, 0.4*255); break;
        case GBC_CYAN:       sf_bc = (color) ? sf::Color::Cyan      : sf::Color(0.8*255, 0.8*255, 0.8*255); break;
        case GBC_GREEN:      sf_bc = (color) ? sf::Color::Green     : sf::Color(0.7*255, 0.7*255, 0.7*255); break;
        case GBC_MAGENTA:    sf_bc = (color) ? sf::Color::Magenta   : sf::Color(0.6*255, 0.6*255, 0.6*255); break;
        case GBC_RED:        sf_bc = (color) ? sf::Color::Red       : sf::Color(0.5*255, 0.5*255, 0.5*255); break;
        case GBC_YELLOW:     sf_bc = (color) ? sf::Color::Yellow    : sf::Color(0.9*255, 0.9*255, 0.9*255); break;
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
        std::uint32_t font_code = get_glyph_font_code(gc);
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
    // only draws if no blink attribute or valid blink state
    if(!blink || blink && (blink_state == negative))
    {
        if(get_glyph_charset(m_glyph) == G1)
            target.draw(m_mosaic, states);
        else
        {
            target.draw(m_text_glyph, states);
            if(underline)
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