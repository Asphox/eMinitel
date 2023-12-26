//
// Created by asphox on 12/1/23.
//

#include <cstdint>
#include <memory>
#include "Font.h"



const sf::Font& get_font()
{
    static std::unique_ptr<sf::Font> font;
    if(!font)
    {
        font = std::make_unique<sf::Font>();
        font->loadFromFile("eMinitel.ttf");
    }
    return *font;
}