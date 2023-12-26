#include <SFML/Graphics.hpp>
#include "Screen/Screen.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1300), "MinitelViewer");
    window.setFramerateLimit(30);
    Screen minitel_screen(1100);
    minitel_screen.update();
    minitel_screen.setPosition(50,50);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        minitel_screen.update_blink();
        window.clear();
        window.draw(minitel_screen);
        window.display();
    }

    return 0;
}