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
}

void Screen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for(std::size_t line=0; line<25; line++)
    {
        for(std::size_t col=0; col<40; col++)
        {
            const ScreenCell& cell = m_cells[col + line * 40];
            target.draw(cell, states);
        }
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
}