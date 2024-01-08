//
// Created by asphox on 12/24/23.
//

#ifndef MINITELVIEWER_MOSAICGLYPH_H
#define MINITELVIEWER_MOSAICGLYPH_H

#include <SFML/Graphics.hpp>
#include <array>

class MosaicGlyph : public sf::Drawable, public sf::Transformable
{
private:
    std::array<sf::RectangleShape, 6>   m_rectangles;
    sf::Vector2f                        m_size;
    sf::Color                           m_fcolor = sf::Color::White;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    MosaicGlyph();
    void set_size(const sf::Vector2f& size);
    void set_value(std::uint8_t v, bool disjointed);
    void setFillColor(sf::Color color);
};


#endif //MINITELVIEWER_MOSAICGLYPH_H
