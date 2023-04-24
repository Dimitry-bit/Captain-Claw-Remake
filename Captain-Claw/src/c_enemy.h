#pragma once

#include <unordered_set>

#include "ecs.h"

void EnemyAIUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                   ECS* ecs, float deltaTime);