//
// Created by asphox on 11/30/23.
//

#ifndef MINITELVIEWER_MINITELSCREEN_H
#define MINITELVIEWER_MINITELSCREEN_H

#include <SFML/Graphics.hpp>
#include "MinitelScreenCell.h"

namespace mvw
{
    class MinitelScreen : public sf::Drawable, public sf::Transformable
    {
    private:
        sf::Vector2f                        m_size;
        std::vector<sf::RectangleShape>     m_rects;
        std::vector<MinitelScreenCell>      m_cells;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        MinitelScreen(sf::Vector2f size);
        void update();
    };
}


#endif //MINITELVIEWER_MINITELSCREEN_H
