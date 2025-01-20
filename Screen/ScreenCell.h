// ScreenCell.h
// Created by asphox on 26/12/2023.
// Manages the drawing of one glyph on screen

#ifndef SCREENCELL_H
#define SCREENCELL_H

#include <SFML/Graphics.hpp>
#include "../minitel_core/Glyph.h"
#include "MosaicGlyph.h"

class Screen;

// =========================================================================================
//      ScreenCell class
//==========================================================================================

class ScreenCell : public sf::Drawable, public sf::Transformable
{
private:
// parent screen of the cell
    const Screen&       m_screen;
    sf::Vector2f        m_size;
    sf::RectangleShape  m_background;
    // eMinitel font doesn't have underlined glyphs, so we need to make it ourselves
    sf::RectangleShape  m_underline;
    // drawable item if glyph is a text
    sf::Text            m_text_glyph;
    // drawable item if glyph is a mosaic
    MosaicGlyph         m_mosaic;
    // current glyph in the cell
    GLYPH_CODE          m_glyph = GC_DEFAULT;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // converts a glyph to font's codepoint
    static std::uint32_t __get_glyph_codepoint(GLYPH_CODE gc);

public:
    ScreenCell(const Screen&);
    ScreenCell(const ScreenCell&) = default;
    ScreenCell& operator=(const ScreenCell& src);
    void setSize(const sf::Vector2f& size);
    void set_glyph(GLYPH_CODE gc);
    inline GLYPH_CODE get_glyph() const { return m_glyph; }
};
#endif //SCREENCELL_H
