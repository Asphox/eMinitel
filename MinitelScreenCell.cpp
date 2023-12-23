//
// Created by asphox on 11/30/23.
//

#include "MinitelScreenCell.h"
#include "MinitelFont.h"

mvw::MinitelScreenCell::MinitelScreenCell()
{
    m_char.setFont(priv::get_font());
    m_char.setString("<");
}

void mvw::MinitelScreenCell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_char, states);
}

void mvw::MinitelScreenCell::setSize(const sf::Vector2f &size)
{
    m_size = size;
    m_char.setCharacterSize(size.x);
}