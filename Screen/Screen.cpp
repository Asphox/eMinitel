// Screen.cpp
// Created by asphox on 26/12/2023.
// Manages the drawing of the entire Minitel screen

#include "Screen.h"

Screen::Screen(float height) : m_size({height*SCREEN_RATIO, height})
{
    for(int i=0; i<40*25; i++)
    {
        m_cells.emplace_back(*this);
    }
    clear();
}

void Screen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // we draw non-normal-sized glyph after all the normal ones
    // to avoid background overlap
    std::vector<const ScreenCell*> non_normal_sized_cells;

    states.transform *= getTransform();
    for(const ScreenCell& r_cell : m_cells)
    {
        if(get_glyph_att_size(r_cell.get_glypĥ()) == GS_NORMAL)
            target.draw(r_cell, states);
        else
            non_normal_sized_cells.push_back(&r_cell);
    }

    for(const ScreenCell* p_non_normal_sized_cell : non_normal_sized_cells)
        target.draw(*p_non_normal_sized_cell, states);

    if(m_cursor_visible && m_blink_state)
    {
        target.draw(m_cursor, states);
    }
}

void Screen::update_blink()
{
    if(m_clock_blink.getElapsedTime().asSeconds() > 1)
    {
        m_blink_state = !m_blink_state;
        m_clock_blink.restart();
    }
}


void Screen::update()
{
    for(std::size_t line=0; line<25; line++)
    {
        for(std::size_t col=0; col<40; col++)
        {
            ScreenCell&  cell = m_cells[col + line * 40];
            cell.setSize({m_size.x/40, m_size.y/25});
            cell.setPosition(col*m_size.x/40, line*m_size.y/25);
        }
    }
    m_cursor.setSize({m_size.x/40, m_size.y/25});
    m_cursor.setFillColor(sf::Color::White);
}

void Screen::clear(bool keep_line_0)
{
    for(int line = keep_line_0 ? 1 : 0; line<25; line++)
    {
        for(int col = 0; col < 40; col++)
        {
            m_cells[col+line*40].set_glyph(GC_SPACE);
        }
    }
}

void Screen::set_glyph_at(GLYPH_CODE gc, int col, int line)
{
    m_cells[col+line*40].set_glyph(gc);
}

bool Screen::set_cursor_pos(unsigned int col, unsigned int line)
{
    if(col > 39 || line > 24)
        return false;

    m_cursor_pos.x = col;
    m_cursor_pos.y = line;
    m_cursor.setPosition(col*m_cursor.getSize().x, line*m_cursor.getSize().y);
    return true;
}

void Screen::set_cursor_visible(bool visible)
{
    m_cursor_visible = visible;
}