#include "ecs.h"
#include "entity_components.h"
#include "entity_inits.h"
#include "c_physics.h"
#include "c_collider.h"
#include "resource_manager.h"
#include "asset_constants.h"

void TileInit(ECS* ecs, entity_t* entity)
{
    EntitySet(entity, C_TILE, &entity->tile);

    if (entity->tile.type == TILE_CLIMBABLE) {
        c_collider_t tileCollider = ColliderCreate(entity->render.sprite, true);
        EntitySet(entity, C_COLLIDER, &tileCollider);
    } else if (entity->tile.type != TILE_CLEAR) {
        c_collider_t tileCollider = ColliderCreate(entity->render.sprite, false);
        EntitySet(entity, C_COLLIDER, &tileCollider);
    }

    entity->tile.isLadderTop = false;
    if (entity->render.sprite.getTexture() == &ResTextureGet("tilesets/LEVEL1_TILES/ACTION/006")) {
        entity->tile.isLadderTop = true;
    }
}

void EnemyInit(ECS* ecs, entity_t* entity)
{
    ECSSetFlag(ecs, entity->ID, C_ENEMY);
    entity->enemy.startPos = entity->transform.getPosition();

    const sf::IntRect& texRect = entity->render.sprite.getTextureRect();
    c_collider_t swordCollider = ColliderCreate(sf::Vector2f(50.0f, 50.0f),
                                                sf::Vector2f(texRect.width, texRect.height), true);
    // NOTE(Tony): Random number for testing only
    c_damageable_t damageable = {
        .swordCollider = swordCollider,
        .pistolOffset = {30.0f, 30.0f},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, entity->ID, C_DAMAGEABLE, &damageable);

    c_collider_t hitCollider = ColliderCreate(entity->render.sprite, true);
    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void CheckpointInit(ECS* ecs, entity_t* entity)
{
    const sf::IntRect& texRect = entity->render.sprite.getTextureRect();
    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(texRect.width * 2.0f, texRect.height),
                                              sf::Vector2f(texRect.width, texRect.height) / 2.0f, true);

    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void PickupInit(ECS* ecs, entity_t* entity)
{
    c_collider_t hitCollider = ColliderCreate(entity->render.sprite, true);
    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}

void PlatformInit(ECS* ecs, entity_t* entity)
{
    entity->platform.starting = entity->transform.getPosition();
    entity->platform.offset = entity->platform.a;
    c_collider_t hitCollider = ColliderCreate(entity->render.sprite, false);
    ECSAdd(ecs, entity->ID, C_COLLIDER, &hitCollider);
}