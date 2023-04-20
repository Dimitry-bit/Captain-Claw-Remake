#include <iostream>
#include "SFML/Graphics.hpp"
#include "Player Movement.h"

sf::Clock cl;
float dt;
float multiplier = 30.f;
float movespeed=5.f;

sf::Vector2f upvelocity = {0,0};
float groundheight=440.f;
float gravity=-1;
bool grounded=false;



void movement(sf:: Sprite& x, float& d,float& mul,float sp,float groundH,sf::Vector2f upvel){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        x.move(sp*d*mul,0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        x.move(-sp*d*mul,0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
       upvel.y=-sp*d*mul;
        if(x.getPosition().y < groundH){
            upvel.y -= gravity*d;
        }
        else {
            upvel.y=0;
        }
    }
d =cl.restart().asSeconds();
}