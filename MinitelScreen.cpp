//
// Created by asphox on 11/30/23.
//

#include "MinitelScreen.h"
#include "MinitelFont.h"

using namespace mvw;

MinitelScreen::MinitelScreen(sf::Vector2f size) : m_size(size)
{
    m_cells = std::vector<MinitelScreenCell>(40*25);
    m_rects = std::vector<sf::RectangleShape>(40*25);
}

void MinitelScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for(std::size_t line=0; line<25; line++)
    {
        for(std::size_t col=0; col<40; col++)
        {
            const sf::RectangleShape& rect = m_rects[col+line*40];
            const MinitelScreenCell& cell = m_cells[col+line*40];
            //target.draw(rect, states);
            target.draw(cell, states);
        }
    }
}

void MinitelScreen::update()
{
    for(std::size_t col=0; col<40; col++)
    {
        for(std::size_t line=0; line<25; line++)
        {
            sf::RectangleShape& rect = m_rects[col+line*40];
            rect.setSize({m_size.x/40, m_size.y/25});
            rect.setPosition(col*m_size.x/40, line*m_size.y/25);
            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color::Red);
            rect.setFillColor(sf::Color::Transparent);

            MinitelScreenCell&  cell = m_cells[col+line*40];
            //cell.setSize({m_size.x/40, m_size.y/25});
            cell.setSize({m_size.x/40, m_size.y/25});
            cell.setPosition(col*m_size.x/40, line*m_size.y/25);
        }
    }
}