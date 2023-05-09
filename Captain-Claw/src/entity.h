#pragma once

#include <string>
#include <list>

#include "entity_components.h"
#include "animation.h"

using std::string;

struct entity_t {
    unsigned int type;
    string logic;
    unsigned long long ID;

    Animator animator;

    sf::Transformable transform;
    c_player_t player;
    c_inventory_t inventory;
    c_damageable_t damageable;
    c_render_t render;
    c_tile_t tile;
    c_platform_t platform;
    c_checkpoint_t checkpoint;
    c_pickup_t pickup;
    c_enemy_t enemy;
    c_physics_t physics;
    c_collider_t collider;
};

entity_t* EntityAlloc();
void EntityDealloc(entity_t** entity);
void EntityInit(entity_t* self, const string& logic, const render_types_t& rType, const string& graphicID = "");

bool EntityHas(entity_t* self, const entity_components_t& cType);
void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue);