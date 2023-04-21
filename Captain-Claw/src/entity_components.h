#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

enum entity_components_t {
    C_NONE = 0,
    C_TILE,
    C_PICKUP,
    C_CHECKPOINT,
    C_ENEMY,
    C_PLATFORM,
    C_SOUND,
};

enum tile_types_t {
    TILE_CLEAR = 0,
    TILE_SOLID,
    TILE_DEATH,
    TILE_CLIMBABLE,
    TILE_GROUND,
};

struct c_tile_t {
    tile_types_t type;
};

enum pickup_types_t {
    PICKUP_NONE = 0,
    PICKUP_AMMO,
    PICKUP_HEALTH,
    PICKUP_TREASURE,
};

struct c_pickup_t {
    pickup_types_t type;
    int value;
};

enum enemy_types_t {
    ENEMY_NONE = 0,
    ENEMY_SOLDIER,
    ENEMY_OFFICER,
    ENEMY_RAT,
};

struct c_enemy_t {
    enemy_types_t type;
    sf::Vector2f min;
    sf::Vector2f max;
};

enum platform_types_t {
    PLATFORM_ELEVATOR = 0,
    PLATFORM_CRUMBLINGPEG,
};

struct c_platform_t {
    platform_types_t type;
    sf::Vector2f a;
    sf::Vector2f b;
    int time; // time in millisecond
};

struct c_checkpoint_t {
    bool keepInventory;
};

struct c_sound_t {
    string soundFile;
    bool isLoop;
};

enum render_types_t {
    RENDER_NONE = 0,
    RENDER_SPRITE,
    RENDER_RECTANGLE,
    RENDER_CIRCLE,
};

struct c_render_t {
    render_types_t type;
    string graphicsID;
    int drawLayer;
    sf::Sprite sprite;
    sf::RectangleShape rectangleShape;
    sf::CircleShape circleShape;
};