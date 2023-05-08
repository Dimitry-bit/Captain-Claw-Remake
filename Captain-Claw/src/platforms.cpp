#include <iostream>
#include "SFML/Graphics.hpp"
#include "entity.h"
#include "platforms.h"




void PlatformMovingVertical(c_platform_t *platform,float deltaTime,sf::Transformable *transform){
    float leftBound=platform->starting.x - platform->a.x;
    float rightBound=platform->starting.x  + platform->b.x;
    if(transform->getPosition().x >= rightBound)
        transform->move(-5*deltaTime,0);
    else if(transform->getPosition().x <= leftBound)
        transform->move(5*deltaTime,0);
}

void PlatformMovingHorizontal(c_platform_t *platform,float deltaTime,sf::Transformable *transform){
    float UpBound=platform->starting.y - platform->a.y;
    float DownBound=platform->starting.y  + platform->b.y;
    if(transform->getPosition().y <= UpBound)
        transform->move(0,5*deltaTime);
    else if(transform->getPosition().y >= DownBound)
        transform->move(0,-5*deltaTime);
}

void PlatformHiding(c_platform_t *platform,c_tile_t *tile ,c_render_t *plat,c_render_t *claw,float deltaTime){
    if(claw->sprite.getGlobalBounds().intersects(plat->sprite.getGlobalBounds())){
        sf:: Clock timer;
        int hide =(int)timer.getElapsedTime().asSeconds();
        if (time >= 2) {
            time = 0;
        }
    }
}