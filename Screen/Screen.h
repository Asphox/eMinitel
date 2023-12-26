// Screen.h
// Created by asphox on 26/12/2023.
// Manages the drawing of the entire Minitel screen

#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include "ScreenCell.h"

class Screen : public sf::Drawable, public sf::Transformable
{
private:
    // true screen ratio for Minitel screen
    static constexpr float SCREEN_RATIO = 320.0f/250.0f;

    sf::Vector2f                        m_size;
    std::vector<ScreenCell>             m_cells;
    bool                                m_blink_state;
    sf::Clock                           m_clock_blink;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    Screen(float height);
    void update();
    // updates the blink status (need to be called for each iteration of the main loop)
    void update_blink();
    inline bool get_blink_state() const { return m_blink_state; }
};



#endif //SCREEN_H
