#pragma once

#include "ecs.h"
#include "entity.h"
#include "entity_components.h"

void PlayerUpdate(entity_t* player, float deltaTime);
void PlayerStateAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                       ECS* ecs, float deltaTime);