#include <iostream>
#include "SFML/Graphics.hpp"
#include "entity.h"
#include "platforms.h"

void PlatformMoving(c_platform_t *platform,float deltaTime,sf::Transformable *transform){
    float leftBound=platform->starting.x - platform->a.x;
    float rightBound=platform->starting.x  + platform->b.x;
    if(transform->getPosition().x >= rightBound)
        transform->move(-5*deltaTime,0);
    else if(transform->getPosition().x <= leftBound)
        transform->move(5*deltaTime,0);
}