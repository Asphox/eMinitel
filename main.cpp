
#include "eMinitel.h"

using namespace mtlc;

int main()
{
    sf::RenderWindow window(sf::VideoMode(900*1.2, 900), "eMinitel");
    eMinitel viewer(window);

    viewer.run();

    return 0;
}