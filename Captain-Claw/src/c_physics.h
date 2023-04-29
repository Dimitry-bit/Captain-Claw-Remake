#include <iostream>

#include "ecs.h"
#include "entity_components.h"
#include "scene_manager.h"

void PhysicsUpdate(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs, scene_context_t* world,
                   float deltaTime);

c_collider_t PhysicsCreateCollider(const sf::Vector2f& size, const sf::Vector2f& offset, bool isTrigger);
void DrawCollider(const c_collider_t& collider);