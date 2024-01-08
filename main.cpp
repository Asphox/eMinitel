#include <SFML/Graphics.hpp>
#include "minitel_core/Minitel.h"
#include "Screen/Screen.h"
#include "Com/TCPCom.h"
#include <iostream>

using namespace mtlc;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1300), "MinitelViewer");
    window.setFramerateLimit(30);
    Screen screen(1100);
    screen.setPosition(50,50);
    TCPCom com;
    Minitel minitel(&screen, &com);
    std::cout << sizeof(Minitel) << std::endl;
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
                    screen.set_color_mode(false);
            }
            else
                screen.set_color_mode(true);
        }

        minitel.update();

        window.clear();
        window.draw(screen);
        window.display();
    }

    return 0;
}