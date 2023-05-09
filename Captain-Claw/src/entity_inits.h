#pragma once

#include "ecs.h"
#include "entity.h"

void TileInit(ECS* ecs, entity_t* entity);
void EnemyInit(ECS* ecs, entity_t* entity);
void CheckpointInit(ECS* ecs, entity_t* entity);
void PickupInit(ECS* ecs, entity_t* entity);
void PlatformInit(ECS* ecs, entity_t* entity);