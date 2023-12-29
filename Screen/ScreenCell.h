// ScreenCell.h
// Created by asphox on 26/12/2023.
// Manages the drawing of one glyph on screen

#ifndef SCREENCELL_H
#define SCREENCELL_H

#include <SFML/Graphics.hpp>
#include "Glyph.h"
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
    GLYPH_CODE          m_glyph = GC_SPACE;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    ScreenCell(const Screen&);
    void setSize(const sf::Vector2f& size);
    void set_glyph(GLYPH_CODE gc);
    inline GLYPH_CODE get_glypĥ() const { return m_glyph; }
};
#endif //SCREENCELL_H
