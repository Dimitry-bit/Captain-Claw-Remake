#pragma once

#include "ecs.h"
#include "entity.h"
#include "entity_components.h"
#include "renderer.h"

void PlayerUpdate(scene_context_t* world, unsigned long long id, float deltaTime);
void PlayerStateAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                       ECS* ecs, float deltaTime);
void PlayerCameraFollow(const sf::Transformable* transform, render_context_t* renderContext, float deltaTime);

void PlayerFSMSwitch(ECS* ecs, unsigned long long id, player_state_t state);