#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

struct c_physics_t {
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    bool isKinematic;
    bool useGravity;
    bool isGrounded;
    bool isClimb;
    float linearDrag;
    float gravityScale;
};

struct c_collider_t {
    sf::Transformable transform;
    sf::Vector2f offset;
    sf::Vector2f size;
    bool isTrigger;
};

enum entity_components_t {
    C_NONE = 0,
    C_TILE = 1 << 0,
    C_PICKUP = 1 << 1,
    C_CHECKPOINT = 1 << 2,
    C_ENEMY = 1 << 3,
    C_PLATFORM = 1 << 4,
    C_RENDER = 1 << 5,
    C_DAMAGEABLE = 1 << 6,
    C_INVENTORY = 1 << 7,
    C_ANIMATOR = 1 << 8,
    C_PLAYER = 1 << 9,
    C_PHYSICS = 1 << 10,
    C_COLLIDER = 1 << 11,
    C_TRANSFORM = 1 << 12,
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
    bool isLadderTop;
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

enum enemy_ai_states {
    ENEMY_STATE_IDLE = 0,
    ENEMY_STATE_MOVING,
    ENEMY_STATE_ATTACKING,
    ENEMY_STATE_HIT,
    ENEMY_STATE_RECOVERING,
    ENEMY_STATE_DEATH,
};

struct c_enemy_t {
    enemy_types_t type;
    enemy_ai_states state;
    float idlePeriod;
    float idleTimer;
    float hitTimer;
    sf::Vector2f startPos;
    sf::Vector2f targetPoint;
    sf::Vector2f min;
    sf::Vector2f max;
};

enum platform_types_t {
    PLATFORM_ELEVATOR = 0,
    PLATFORM_CRUMBLINGPEG,
};

struct c_platform_t {
    platform_types_t type;
    sf::Vector2f starting;
    sf::Vector2f a;
    sf::Vector2f b;
    sf::Vector2f offset;
    int time; // time in millisecond
    float timer;
};

struct c_checkpoint_t {
    bool keepInventory;
    bool isActive;
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
    sf::Sprite sprite;
    sf::RectangleShape rectangleShape;
    sf::CircleShape circleShape;
};

struct c_inventory_t {
    int gold_counter;
    int ammo_pistol;
};

struct c_damageable_t {
    c_collider_t swordCollider;
    sf::Vector2f pistolOffset;
    int health;
    int lives;
};

enum player_state_t {
    PLAYER_STATE_IDLE = 0,
    PLAYER_STATE_MOVING,
    PLAYER_STATE_HIT,
    PLAYER_STATE_RECOVER,
    PLAYER_STATE_ATTACK,
    PLAYER_STATE_DEATH,
    PLAYER_STATE_JUMP,
    PLAYER_STATE_FALLING,
    Player_STATE_LANDING,
    Player_STATE_CLIMBING,
};

struct c_player_t {
    player_state_t state;
};