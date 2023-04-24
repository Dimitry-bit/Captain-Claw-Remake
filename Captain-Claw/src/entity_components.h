#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

enum entity_components_t {
    C_NONE = 0,
    C_TILE = 1 << 0,
    C_PICKUP = 1 << 1,
    C_CHECKPOINT = 1 << 2,
    C_ENEMY = 1 << 3,
    C_PLATFORM = 1 << 4,
    C_SOUND = 1 << 5,
    C_RENDER = 1 << 6,
    C_DAMAGEABLE = 1 << 7,
    C_INVENTORY = 1 << 8,
    C_ANIMATOR = 1 << 9,
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
    PICKUP_LIVE,
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
    bool isActive;
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

struct c_inventory_t {
    int gold_counter;
    int ammo_pistol;
};

struct c_damageable_t {
    sf::FloatRect swordCollider;
    sf::Vector2f pistolOffset;
    int health;
    int lives;
};