#pragma once

#include "ecs.h"
#include "entity_components.h"

void CheckPointUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs, ECS* ecs);