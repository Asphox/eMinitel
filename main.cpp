#include <SFML/Graphics.hpp>
#include "Core/Minitel.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1300), "MinitelViewer");
    window.setFramerateLimit(30);
    Minitel minitel(1100);
    minitel.setPosition(50,50);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::Space)
                    minitel.reset();
            }
        }

        minitel.update();

        window.clear();
        window.draw(minitel);
        window.display();
    }

    return 0;
}