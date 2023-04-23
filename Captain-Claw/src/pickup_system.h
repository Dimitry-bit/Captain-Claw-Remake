#pragma once

#include <unordered_set>

#include "ecs.h"
#include "entity.h"

struct Player {
    sf::Sprite sprite;
    sf::RectangleShape swordCollider;
    int health = 100;
    Animator animator;
};

void PickupAddGold(entity_t& captain_claw, c_pickup_t& gold);
void PickupAddHealth(entity_t& captain_claw, c_pickup_t& health);
void PickupAddAmmo(entity_t& captain_claw, c_pickup_t& ammo);
void PickupAddLives(entity_t& captain_claw, c_pickup_t& live);
void PickupUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs, ECS* ecs);