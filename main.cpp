#include <SFML/Graphics.hpp>
#include "MinitelScreen.h"

using namespace mvw;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "MinitelViewer");
    MinitelScreen minitel_screen({700,500});
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

        window.clear();
        window.draw(minitel_screen);
        window.display();
    }

    return 0;
}