#include <iostream>
#include "SFML/Graphics.hpp"

#include "ecs.h"
#include "entity_components.h"
#include "scene_manager.h"

void PhysicsUpdate(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs, scene_context_t* world,
                   float deltaTime);