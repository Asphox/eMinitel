// ScreenInterface.h
// Created by asphox on 1/3/24.
// Interface between Minitel minitel_core and a screen implementation (SFML, ncurses, ...)

#ifndef SCREENINTERFACE_H
#define SCREENINTERFACE_H

#include "../minitel_core/Glyph.h"

namespace mtlc  //MiniTeL minitel_core
{
    class IScreen
    {
    public:
        // note : width must be determined with Minitel's screen ratio (320/250)
        virtual void set_height(float height) = 0;
        virtual void update() = 0;
        virtual void clear(bool keep_line_0 = false) = 0;
        virtual void set_glyph_at(GLYPH_CODE gc, std::uint8_t col, std::uint8_t line) = 0;
        virtual void set_cursor_pos(std::uint8_t col, std::uint8_t line) = 0;
        virtual void set_cursor_visible(bool visible) = 0;
        virtual void set_mask(bool mask) = 0;
        // scrolls screen up or down (only lines 01 to 24)
        // nb_of_line > 0 : scroll-down
        // otherwise : scroll-up
        virtual void scroll(std::int8_t nb_of_line) = 0;
    };
}

#endif //SCREENINTERFACE_H
