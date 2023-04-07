#include <SFML/Graphics.hpp>

#include "resource_manager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Captain Claw");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    ResUnloadAll();
    return 0;
}