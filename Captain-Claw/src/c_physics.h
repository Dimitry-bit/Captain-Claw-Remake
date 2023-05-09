#include <iostream>

#include "ecs.h"
#include "entity_components.h"
#include "scene_manager.h"

void PhysicsUpdate(std::unordered_set<unsigned long long>& entityIDS,
                   std::unordered_set<unsigned long long>& platformIDs,
                   ECS* ecs, scene_context_t* world, float deltaTime);