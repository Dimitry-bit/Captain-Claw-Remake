#include <iostream>
#include "SFML/Graphics.hpp"


void PlatformsMovement(sf::Sprite platform,float deltaTime,float start,float end){
    if(platform.getPosition().x == start)
    {
        platform.move(5*deltaTime, 0);
    }
    if(platform.getPosition().x + platform.getLocalBounds().width== end)
    {
        platform.move(-5*deltaTime, 0);
    }
}