#include "ecs.h"
#include "entity_components.h"
#include "entity_inits.h"
#include "c_physics.h"
#include "c_collider.h"

void TileInit(ECS* ecs, entity_t* entity)
{
    EntitySet(entity, C_TILE, &entity->tile);
    sf::Vector2f size = sf::Vector2f(entity->render.sprite.getGlobalBounds().width,
                                     entity->render.sprite.getGlobalBounds().height);
    sf::Vector2f offset = size * 0.5f;
    c_collider_t tileCollider = ColliderCreate(size, offset, false);
    EntitySet(entity, C_COLLIDER, &tileCollider);
}

void EnemyInit(ECS* ecs, entity_t* entity)
{
    ECSSetFlag(ecs, entity->ID, C_ENEMY);
    entity->enemy.startPos = entity->transform.getPosition();

    c_collider_t
        swordCollider = ColliderCreate(sf::Vector2f(50.0f, 50.0f),
                                       sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                    entity->render.sprite.getTextureRect().height), true);
    // NOTE(Tony): Random number for testing only
    c_damageable_t damageable = {
        .swordCollider = swordCollider,
        .pistolOffset = {30.0f, 30.0f},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, entity->ID, C_DAMAGEABLE, &damageable);

    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height),
                                              sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height) / 2.0f,
                                              true);

    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void CheckpointInit(ECS* ecs, entity_t* entity)
{
    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(entity->render.sprite.getTextureRect().width * 2.0f,
                                                           entity->render.sprite.getTextureRect().height),
                                              sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height) / 2.0f,
                                              true);

    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void PickupInit(ECS* ecs, entity_t* entity)
{
    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height),
                                              sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height) / 2.0f,
                                              true);

    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void PlatformInit(ECS* ecs, entity_t* entity)
{
    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height),
                                              sf::Vector2f(entity->render.sprite.getTextureRect().width,
                                                           entity->render.sprite.getTextureRect().height) / 2.0f,
                                              true);

    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);

}