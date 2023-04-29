#include <cstdio>
#include <string>

#include "scene_serialization.h"
#include "entity.h"
#include "c_physics.h"

static void ReadEntityData(scene_context_t* world, entity_t* entity, FILE* fp);

static void ReadEntityData(scene_context_t* world, entity_t* entity, FILE* fp)
{
    // NOTE(Tony): Silly hack for alignment
    unsigned int type;
    fread(&type, sizeof(type), 1, fp);
    entity->type = 0;
    if (type != 0) {
        entity->type = 1 << (type - 1);
    }

    int len;
    fread(&len, sizeof(len), 1, fp);
    entity->logic.resize(len);
    fread(entity->logic.data(), sizeof(*entity->logic.data()), len, fp);

    entity_transform transform;
    fread(&transform.position.x, sizeof(transform.position.x), 2, fp);
    fread(&transform.origin.x, sizeof(transform.origin.x), 2, fp);

    fread(&entity->render.type, sizeof(entity->render.type), 1, fp);
    fread(&len, sizeof(len), 1, fp);
    entity->render.graphicsID.resize(len);
    fread(entity->render.graphicsID.data(), sizeof(*entity->render.graphicsID.data()), len, fp);
//    fread(&entity->render.drawLayer, sizeof(entity->render.drawLayer), 1, fp);
    EntityInit(entity, entity->logic, entity->render.type, entity->render.graphicsID);
    EntitySetPos(entity, transform.position);
    EntitySetOrigin(entity, transform.origin);

    switch (entity->type) {
        case C_NONE:break;
        case C_TILE: {
            fread(&entity->tile.type, sizeof(entity->tile.type), 1, fp);
            EntitySet(entity, C_TILE, &entity->tile);

            sf::Vector2f size = sf::Vector2f(entity->render.sprite.getGlobalBounds().width,
                                             entity->render.sprite.getGlobalBounds().height);
            sf::Vector2f offset = size * 0.5f;
            c_collider_t tileCollider = PhysicsCreateCollider(size, offset, false);
            EntitySet(entity, C_COLLIDER, &tileCollider);
        }
            break;
        case C_PICKUP: {
            fread(&entity->pickup.type, sizeof(entity->pickup.type), 1, fp);
            fread(&entity->pickup.value, sizeof(entity->pickup.value), 1, fp);
            ECSAdd(&world->ecs, entity->ID, C_PICKUP, &entity->pickup);
        }
            break;
        case C_CHECKPOINT: {
            fread(&entity->checkpoint.keepInventory, sizeof(entity->checkpoint.keepInventory), 1, fp);
            entity->checkpoint.isActive = false;
            ECSAdd(&world->ecs, entity->ID, C_CHECKPOINT, &entity->checkpoint);
        }
            break;
        case C_ENEMY: {
            fread(&entity->enemy.type, sizeof(entity->enemy.type), 1, fp);
            fread(&entity->enemy.min.x, sizeof(entity->enemy.min.x), 2, fp);
            fread(&entity->enemy.max.x, sizeof(entity->enemy.max.x), 2, fp);
            ECSAdd(&world->ecs, entity->ID, C_ENEMY, &entity->enemy);
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

            ECSAdd(&world->ecs, entity->ID, C_DAMAGEABLE, &damageable);
        }
            break;
        case C_PLATFORM: {
            fread(&entity->platform.type, sizeof(entity->platform.type), 1, fp);
            fread(&entity->platform.a.x, sizeof(entity->platform.a.x), 2, fp);
            fread(&entity->platform.b.x, sizeof(entity->platform.b.x), 2, fp);
            fread(&entity->platform.time, sizeof(entity->platform.time), 1, fp);
            ECSAdd(&world->ecs, entity->ID, C_PLATFORM, &entity->platform);
        }
            break;
        case C_SOUND:break;
        case C_RENDER:break;
    }

    if (!(entity->type & C_TILE)) {
        ECSAdd(&world->ecs, entity->ID, C_RENDER, &entity->render);
    }
}

void SceneDeserialize(scene_context_t* world, const std::string& file)
{
    FILE* fp = fopen(file.c_str(), "rb");

    if (!fp) {
        printf("[ERROR][SceneSerialization]: Could not open %s for reading.\n", file.c_str());
        return;
    }

    fread(&world->tileGridWidth, sizeof(world->tileGridWidth), 1, fp);
    fread(&world->tileGridHeight, sizeof(world->tileGridHeight), 1, fp);
    fread(&world->tileSize, sizeof(world->tileSize), 1, fp);
    fread(&world->tileMapCount, sizeof(world->tileMapCount), 1, fp);
    for (int i = 0; i < world->tileMapCount; ++i) {
        int count;
        fread(&count, sizeof(count), 1, fp);
        SceneSetTileIndex(world, i);
        while (count-- > 0) {
            int x, y;
            fread(&x, sizeof(x), 1, fp);
            fread(&y, sizeof(y), 1, fp);
            entity_t* entity = EntityAlloc();
            if (entity) {
                ReadEntityData(world, entity, fp);
                SceneAddTile(world, entity, x, y);
            }
        }
    }

    int count;
    fread(&count, sizeof(count), 1, fp);
    while (count--) {
        entity_t* entity = ECSEntityAlloc(&world->ecs);
        if (entity) {
            ReadEntityData(world, entity, fp);
//            SceneAddEntity(world, entity);
        }
    }

    fclose(fp);
}