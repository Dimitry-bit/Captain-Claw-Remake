#include "SFML/Graphics.hpp"

#include "utils.h"

extern sf::RenderWindow* rWindow;

void DrawCollider(const sf::Sprite& s)
{
    sf::FloatRect globalBounds = s.getGlobalBounds();
    sf::RectangleShape colliderBoundaries;
    colliderBoundaries.setFillColor(sf::Color::Transparent);
    colliderBoundaries.setOutlineColor(sf::Color::Green);
    colliderBoundaries.setOutlineThickness(1.0f);
    colliderBoundaries.setPosition(globalBounds.left, globalBounds.top);
    colliderBoundaries.setSize(sf::Vector2f(globalBounds.width, globalBounds.height));

    rWindow->draw(colliderBoundaries);
}

void DrawOrigin(const sf::Sprite& s)
{
    sf::CircleShape origin;
    origin.setRadius(3.0f);
    origin.setOrigin(origin.getRadius(),origin.getRadius());
    origin.setFillColor(sf::Color::Magenta);
    origin.setPosition(s.getGlobalBounds().left + s.getOrigin().x, s.getGlobalBounds().top + s.getOrigin().y);

    rWindow->draw(origin);
}