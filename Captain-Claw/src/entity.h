#pragma once

#include <string>
#include <list>

#include "entity_components.h"
#include "animation.h"

using std::string;

struct entity_t {
    entity_components_t type;
    string logic;
    unsigned long long ID;

    sf::RectangleShape swordCollider;
    int gold_counter = 0;
    int health = 100;
    int ammo_pistol = 10;
    int lives = 6;

    Animator animator;
    c_render_t render;
    c_tile_t tile;
    c_platform_t platform;
    c_checkpoint_t checkpoint;
    c_pickup_t pickup;
    c_enemy_t enemy;
    c_sound_t sound;
};

struct entity_transform {
    sf::FloatRect bounds;
    sf::Vector2f position;
    sf::Vector2f origin;
};

entity_t* EntityAlloc();
void EntityDealloc(entity_t** entity);
void EntityInit(entity_t* self, const string& logic, const render_types_t& rType, const string& graphicID = "");

bool EntityHas(entity_t* self, const entity_components_t& cType);
void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue);
void EntitySetPos(entity_t* self, float x, float y);
void EntitySetPos(entity_t* self, const sf::Vector2f& pos);
void EntitySetOrigin(entity_t* self, float x, float y);
void EntitySetOrigin(entity_t* self, const sf::Vector2f& origin);
entity_transform EntityGetTransform(const entity_t* self);
void EntityUpdate(entity_t* self, const entity_t* to);

void DrawEntity(const entity_t* entity);