#pragma once

#include <unordered_set>

#include "ecs.h"
#include "entity_components.h"

void DrawEntity(const std::unordered_set<unsigned long long>& entityIDs, ECS* ecs);