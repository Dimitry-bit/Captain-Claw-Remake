#include <cstdio>
#include <string>

#include "scene_serialization.h"
#include "entity.h"
#include "entity_inits.h"

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

    sf::Vector2f pos;
    fread(&pos.x, sizeof(pos.x), 2, fp);
    entity->transform.setPosition(pos);
    sf::Vector2f origin;
    fread(&origin.x, sizeof(origin.x), 2, fp);
//    entity->transform.setOrigin(origin);

    fread(&entity->render.type, sizeof(entity->render.type), 1, fp);
    fread(&len, sizeof(len), 1, fp);
    entity->render.graphicsID.resize(len);
    fread(entity->render.graphicsID.data(), sizeof(*entity->render.graphicsID.data()), len, fp);
    EntityInit(entity, entity->logic, entity->render.type, entity->render.graphicsID);

    switch (entity->type) {
        case C_TILE: {
            fread(&entity->tile.type, sizeof(entity->tile.type), 1, fp);
            TileInit(&world->ecs, entity);
        }
            break;
        case C_PICKUP: {
            fread(&entity->pickup.type, sizeof(entity->pickup.type), 1, fp);
            fread(&entity->pickup.value, sizeof(entity->pickup.value), 1, fp);
            ECSSetFlag(&world->ecs, entity->ID, C_PICKUP);
        }
            break;
        case C_CHECKPOINT: {
            fread(&entity->checkpoint.keepInventory, sizeof(entity->checkpoint.keepInventory), 1, fp);
            ECSSetFlag(&world->ecs, entity->ID, C_CHECKPOINT);
        }
            break;
        case C_ENEMY: {
            fread(&entity->enemy.type, sizeof(entity->enemy.type), 1, fp);
            fread(&entity->enemy.min.x, sizeof(entity->enemy.min.x), 2, fp);
            fread(&entity->enemy.max.x, sizeof(entity->enemy.max.x), 2, fp);
            EnemyInit(&world->ecs, entity);
        }
            break;
        case C_PLATFORM: {
            fread(&entity->platform.type, sizeof(entity->platform.type), 1, fp);
            fread(&entity->platform.a.x, sizeof(entity->platform.a.x), 2, fp);
            fread(&entity->platform.b.x, sizeof(entity->platform.b.x), 2, fp);
            fread(&entity->platform.time, sizeof(entity->platform.time), 1, fp);
            ECSSetFlag(&world->ecs, entity->ID, C_PLATFORM);
        }
            break;
        default:break;
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
        }
    }

    fclose(fp);
}