// Screen.h
// Created by asphox on 26/12/2023.
// Manages the drawing of the entire Minitel screen

#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include "../minitel_core/Interfaces.h"
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
    bool                                m_color_mode = true;
    bool                                m_masked = false;
    sf::Clock                           m_clock_blink;
    sf::Vector2u                        m_cursor_pos;
    sf::RectangleShape                  m_cursor;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    // applies zones attributes
    void __apply_zones_attributes();
public:
    Screen(float height);
    inline bool get_blink_state() const { return m_blink_state; }
    inline void set_color_mode(bool c){ m_color_mode = c; }
    inline bool get_color_mode() const { return m_color_mode; }
    inline bool get_masked() const {return m_masked; }

    static void s_mtlc_screen_control(MTLC_OP_SCREEN op, intptr_t param, void* ctx);


    void set_height(float height);
    void update();
    void clear(bool keep_line_0 = false);
    void set_glyph_at(GLYPH_CODE gc, std::uint8_t col, std::uint8_t line);
    void set_cursor_pos(std::uint8_t col, std::uint8_t line);
    void set_cursor_visible(bool visible);
    void set_mask(bool mask);
    void scroll(std::int8_t nb_of_line);
};



#endif //SCREEN_H
