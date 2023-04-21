#include <iostream>
#include "SFML/Graphics.hpp"
#include "Player Movement.h"
#include <math.h>

bool IsValidMove(const sf::Vector2f& pos, float groundH)
{
    return (pos.y < groundH);
}

void MoveUnchecked(sf::Sprite& x, sf::Vector2f movement, float& deltaTime,sf::Sprite& ladder, float jumpHeight)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movement.x += 1.0f * deltaTime;
        x.setScale(1,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movement.x -= 1.0f * deltaTime;
        x.setScale(-1,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && x.getGlobalBounds().intersects(ladder.getGlobalBounds())){
        movement.y -= 1.0f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && x.getGlobalBounds().intersects(ladder.getGlobalBounds())){
        movement.y += 1.0f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        movement.y= -sqrtf(2.0f * 981.0f * jumpHeight);
        movement.y += 981.0f * deltaTime;
    }

    x.move(movement);
}