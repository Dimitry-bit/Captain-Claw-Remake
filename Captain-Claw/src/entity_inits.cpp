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
    ECSSetFlag(ecs, entity->ID, C_ENEMY);
    entity->enemy.startPos = entity->transform.getPosition();

    // NOTE(Tony): Random number for testing only
    c_damageable_t damageable = {
        .swordCollider = {0, 0, 50, 50},
        .pistolOffset = {30.0f, 30.0f},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, entity->ID, C_DAMAGEABLE, &damageable);
}