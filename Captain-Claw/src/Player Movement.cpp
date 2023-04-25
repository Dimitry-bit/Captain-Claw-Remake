#include <iostream>
#include "SFML/Graphics.hpp"
#include "Player Movement.h"
#include <math.h>
#include <scene_manager.h>

bool IsValidMove(const sf::Vector2f& pos, float groundH)
{
    return (pos.y < groundH);
}
bool isValid(const scene_context_t* world, float x, float y) {
    entity_t *tile = SceneGetTileWithPos(world, x, y);
    if (tile != nullptr) {
        return (tile->tile.type == TILE_GROUND || tile->tile.type == TILE_CLIMBABLE || tile->tile.type == TILE_CLEAR);
    }
}

void MoveUnchecked(sf::Sprite& player,float& deltaTime, float jumpHeight,scene_context_t* world)
{


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        float x = player.getPosition().x + 5 * deltaTime;
        float y = player.getPosition().y;
        if (isValid(world,x,y))
            player.move(5* deltaTime,0);
        if (player.getScale().x>=0)
            player.setScale(player.getScale().x,player.getScale().y);
        else
            player.setScale(player.getScale().x*-1,player.getScale().y);

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        float x = player.getPosition().x - 5* deltaTime;
        float y = player.getPosition().y;
        if(isValid(world,x,y))
            player.move(-5.0f * deltaTime,0);
        if (player.getScale().x>=0)
            player.setScale(player.getScale().x*-1,player.getScale().y);
        else
            player.setScale(player.getScale().x,player.getScale().y);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        float x = player.getPosition().x;
        float y = player.getPosition().y - 5* deltaTime;
        if(isValid(world,x,y))
           player.move(0,- 5.0f * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        float x = player.getPosition().x;
        float y = player.getPosition().y + 5* deltaTime;
        if(isValid(world,x,y))
            player.move(0,5.0f * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        player.move(0,-sqrtf(5.0f * 981.0f * jumpHeight));
        player.move(0,981.0f * deltaTime);
    }
}