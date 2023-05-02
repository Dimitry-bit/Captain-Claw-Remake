#pragma once

#include <unordered_set>

#include "ecs.h"
#include "entity_components.h"

void ColliderSync(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs);
bool CheckCollision(c_collider_t& a, c_collider_t& b, sf::Vector2i* hitNormal);
c_collider_t ColliderCreate(const sf::Vector2f& size, const sf::Vector2f& offset, bool isTrigger);
c_collider_t ColliderCreate(const sf::Sprite& sprite, bool isTrigger);
sf::FloatRect ColliderGetLocalBounds(const c_collider_t& self);
sf::FloatRect ColliderGetGlobalBounds(const c_collider_t& self);

void DrawCollider(const c_collider_t& collider);