#pragma once

#include <unordered_set>

#include "ecs.h"
#include "entity_components.h"

void DrawEntities(const std::unordered_set<unsigned long long>& entityIDs, ECS* ecs);
void DrawEntity(const c_render_t* render, const sf::RenderStates& states = sf::RenderStates::Default);