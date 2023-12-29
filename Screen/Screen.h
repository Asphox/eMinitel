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
    bool                                m_blink_state = false;
    bool                                m_cursor_visible = false;
    sf::Clock                           m_clock_blink;
    sf::Vector2u                        m_cursor_pos;
    sf::RectangleShape                  m_cursor;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    Screen(float height);
    void update();
    // updates the blink status (need to be called for each iteration of the main loop)
    void update_blink();
    inline bool get_blink_state() const { return m_blink_state; }

    void clear(bool keep_line_0 = false);
    void set_glyph_at(GLYPH_CODE gc, int col, int line);
    bool set_cursor_pos(unsigned int col, unsigned int line);
    void set_cursor_visible(bool visible);
};



#endif //SCREEN_H
