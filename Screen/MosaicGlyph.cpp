//
// Created by asphox on 12/24/23.
//

#include "MosaicGlyph.h"

MosaicGlyph::MosaicGlyph()
{
}

void MosaicGlyph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for(const sf::RectangleShape& rec : m_rectangles)
    {
        target.draw(rec, states);
    }
}

void MosaicGlyph::setFillColor(sf::Color color)
{
    m_fcolor = color;
}

void MosaicGlyph::set_size(const sf::Vector2f& size)
{
    m_size = size;
    sf::Vector2f rec_size = {size.x/2, size.y/3};
    for(sf::RectangleShape& rec : m_rectangles)
    {
        rec.setSize(rec_size);
        rec.setOrigin({rec_size.x, 0});
    }
    m_rectangles[0].setPosition(m_size.x/2, 0);
    m_rectangles[1].setPosition(m_size.x, 0);
    m_rectangles[2].setPosition(m_size.x/2, m_size.y*(1.0/3.0));
    m_rectangles[3].setPosition(m_size.x, m_size.y*(1.0/3.0));
    m_rectangles[4].setPosition(m_size.x/2, m_size.y*(2.0/3.0));
    m_rectangles[5].setPosition(m_size.x, m_size.y*(2.0/3.0));
}

void MosaicGlyph::set_value(std::uint8_t v, bool disjointed)
{
    for(int i=0; i<6; i++)
    {
        if(disjointed)
            m_rectangles[i].setScale({0.75, 0.75});
        else
            m_rectangles[i].setScale({1.0, 1.0});

        if((v>>i) & 1)
            m_rectangles[i].setFillColor(m_fcolor);
        else
            m_rectangles[i].setFillColor(sf::Color::Transparent);
    }
}