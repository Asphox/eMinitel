
#include "eMinitel.h"

using namespace mtlc;

int main()
{
    sf::RenderWindow window(sf::VideoMode(900*1.2, 900), "eMinitel");
    window.setFramerateLimit(30);
    eMinitel viewer(window);

    viewer.run();

    return 0;
}