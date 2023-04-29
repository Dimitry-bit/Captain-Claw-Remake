#include "ecs.h"
#include "entity_components.h"
#include "entity_inits.h"
#include "c_physics.h"

void TileInit(ECS* ecs, entity_t* entity)
{
    EntitySet(entity, C_TILE, &entity->tile);
    sf::Vector2f size = sf::Vector2f(entity->render.sprite.getGlobalBounds().width,
                                     entity->render.sprite.getGlobalBounds().height);
    sf::Vector2f offset = size * 0.5f;
    c_collider_t tileCollider = PhysicsCreateCollider(size, offset, false);
    EntitySet(entity, C_COLLIDER, &tileCollider);
}

void EnemyInit(ECS* ecs, entity_t* entity)
{
    ECSAdd(ecs, entity->ID, C_ENEMY, &entity->enemy);
    entity->enemy.state = ENEMY_STATE_IDLE;
    entity->enemy.idleTimer = 0;
    entity->enemy.idlePeriod = 0;
    entity->enemy.targetPoint = sf::Vector2f(0.0f, 0.0f);
    entity->enemy.startPos = entity->render.sprite.getPosition();

    // NOTE(Tony): Random number for testing only
    c_damageable_t damageable = {
        .swordCollider = {0, 0, 50, 50},
        .pistolOffset = {30.0f, 30.0f},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, entity->ID, C_DAMAGEABLE, &damageable);
}

void PlayerInit(ECS* ecs, entity_t* entity)
{

}

void PlatformInit(ECS* ecs, entity_t* entity)
{
    ECSAdd(ecs, entity->ID, C_PLATFORM, &entity->platform);
}

void CheckpointInit(ECS* ecs, entity_t* entity)
{
    entity->checkpoint.isActive = false;
    ECSAdd(ecs, entity->ID, C_CHECKPOINT, &entity->checkpoint);
}

void PickupInit(ECS* ecs, entity_t* entity)
{
    ECSAdd(ecs, entity->ID, C_PICKUP, &entity->pickup);
}