#include <cstdio>
#include <string>

#include "scene_serialization.h"
#include "entity.h"

static void ReadEntityData(scene_context_t* world, entity_t* entity, FILE* fp);

static void ReadEntityData(scene_context_t* world, entity_t* entity, FILE* fp)
{
    fread(&entity->type, sizeof(entity->type), 1, fp);

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
    ECSAdd(&world->ecs, entity->ID, C_RENDER, &entity->render);

    switch (entity->type) {
        case C_NONE:break;
        case C_TILE: {
            fread(&entity->tile.type, sizeof(entity->tile.type), 1, fp);
            EntitySet(entity, entity->type, &entity->tile);
        }
            break;
        case C_PICKUP: {
            fread(&entity->pickup.type, sizeof(entity->pickup.type), 1, fp);
            fread(&entity->pickup.value, sizeof(entity->pickup.value), 1, fp);
            ECSAdd(&world->ecs, entity->ID, entity->type, &entity->pickup);
        }
            break;
        case C_CHECKPOINT: {
            fread(&entity->checkpoint.keepInventory, sizeof(entity->checkpoint.keepInventory), 1, fp);
            ECSAdd(&world->ecs, entity->ID, entity->type, &entity->checkpoint);
        }
            break;
        case C_ENEMY: {
            fread(&entity->enemy.type, sizeof(entity->enemy.type), 1, fp);
            fread(&entity->enemy.min.x, sizeof(entity->enemy.min.x), 2, fp);
            fread(&entity->enemy.max.x, sizeof(entity->enemy.max.x), 2, fp);
            ECSAdd(&world->ecs, entity->ID, entity->type, &entity->enemy);
        }
            break;
        case C_PLATFORM: {
            fread(&entity->platform.type, sizeof(entity->platform.type), 1, fp);
            fread(&entity->platform.a.x, sizeof(entity->platform.a.x), 2, fp);
            fread(&entity->platform.b.x, sizeof(entity->platform.b.x), 2, fp);
            fread(&entity->platform.time, sizeof(entity->platform.time), 1, fp);
            ECSAdd(&world->ecs, entity->ID, entity->type, &entity->platform);
        }
            break;
        case C_SOUND:break;
        case C_RENDER:break;
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