#include "scene_manager.h"
#include "scene_serialization.h"
#include "asset_constants.h"
#include "renderer.h"
#include "resource_manager.h"
#include "entity.h"

void SceneAllocAssets(scene_context_t* world)
{
    // This is fine if we only load 1--2 levels

    AssetPushType(ASSET_FONT);
    ResLoadFromFile(FONT_JETBRAINS);
    AssetPopType();

    AssetPushType(ASSET_SPRITESHEET); // ASSET_PUSH
    ResLoadFromFile(TILE_LEVEL1_TILES, ASSET_TAG_TILE);
    ResLoadFromFile(OBJ_LEVEL1_OBJECTS, ASSET_TAG_OBJ | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_SPRITESHEET_GAME, ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResLoadFromFile(OBJ_CHECKPOINT, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile(OBJ_SUPERCHECKPOINT, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile(OBJ_CRUMBLINGPEG, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile(OBJ_EXTRALIFE, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile(OBJ_HANDS4, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_MANICAL1, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_TORCH, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_TORSHSTAND, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_CRATES, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile(OBJ_COINS, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile(OBJ_DYNAMITELIT1, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile(OBJ_DYNAMITELIT2, ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile(OBJ_WARP, ASSET_TAG_ANIMATION);
    ResLoadFromFile(OBJ_VERTWARP, ASSET_TAG_ANIMATION);
    ResLoadFromFile(OBJ_BOSSWARP, ASSET_TAG_ANIMATION);

    ResLoadFromFile(CHAR_OFFICER_IDLE, ASSET_TAG_ANIMATION);

    ResLoadFromFile(CHAR_SOLDIER_IDLE, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_SOLDIER_WALK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_SOLDIER_GUN_ATTACK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_SOLDIER_HURT_STANCES, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_SOLDIER_HURT_STANCES_2, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_SOLDIER_DEATH, ASSET_TAG_ANIMATION);

    ResLoadFromFile(CHAR_CLAW_IDLE, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_CLIMBING, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_FALLING, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_RUN, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_HURT, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_JUMP, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_CROTCH_PISTOL_ATTACK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_CROTCH_SWORD_ATTACK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_MELEE_ATTACK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_PISTOL_ATTACK, ASSET_TAG_ANIMATION);
    ResLoadFromFile(CHAR_CLAW_SWORD_ATTACK, ASSET_TAG_ANIMATION);

    ResLoadFromFile(VFX_GLITTER, ASSET_TAG_ANIMATION);
    ResLoadFromFile(VFX_GLITTERGREEN, ASSET_TAG_ANIMATION);
    ResLoadFromFile(VFX_GLITTERRED, ASSET_TAG_ANIMATION);

    ResLoadFromFile(UI_DYNAMITE, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_HEALTHHEART, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_PISTOL, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_POWERUPS, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_STOPWATCH, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_TREASURECHEST, ASSET_TAG_ANIMATION);
    ResLoadFromFile(UI_LIVESHEAD, ASSET_TAG_ANIMATION);
    AssetPopType(); // ASSET_POP

    ResTextureLoadFromSpriteSheet(TILE_LEVEL1_TILES);
    ResTextureLoadFromSpriteSheet(OBJ_LEVEL1_OBJECTS);
    ResTextureLoadFromSpriteSheet(OBJ_SPRITESHEET_GAME);
    ResTextureLoadFromSpriteSheet(OBJ_CHECKPOINT);
    ResTextureLoadFromSpriteSheet(OBJ_SUPERCHECKPOINT);
    ResTextureLoadFromSpriteSheet(OBJ_CRUMBLINGPEG);
    ResTextureLoadFromSpriteSheet(OBJ_EXTRALIFE);
    ResTextureLoadFromSpriteSheet(OBJ_HANDS4);
    ResTextureLoadFromSpriteSheet(OBJ_MANICAL1);
    ResTextureLoadFromSpriteSheet(OBJ_TORCH);
    ResTextureLoadFromSpriteSheet(OBJ_TORSHSTAND);
    ResTextureLoadFromSpriteSheet(OBJ_CRATES);
    ResTextureLoadFromSpriteSheet(OBJ_COINS);
    ResTextureLoadFromSpriteSheet(CHAR_OFFICER_IDLE);
    ResTextureLoadFromSpriteSheet(CHAR_SOLDIER_IDLE);
    ResTextureLoadFromSpriteSheet(CHAR_CLAW_IDLE);

    AssetPushType(ASSET_SOUNDBUFFER);
    ResLoadFromFile(WAV_AMBIENT_CAVEMEDIUM2);
    ResLoadFromFile(WAV_CLAW_DRYGUNSHOT1);
    ResLoadFromFile(WAV_CLAW_FALLDEATH);
    ResLoadFromFile(WAV_CLAW_GUNSHOT);
    ResLoadFromFile(WAV_CLAW_LAND);
    ResLoadFromFile(WAV_CLAW_LEFTFOOT1);
    ResLoadFromFile(WAV_CLAW_RIGHTFOOT1);
    ResLoadFromFile(WAV_CLAW_LEFTSWING1);
    ResLoadFromFile(WAV_CLAW_LIGHTSWORD);
    ResLoadFromFile(WAV_CLAW_SWORDSWISH);
    ResLoadFromFile(WAV_CLAW_UPPERCUT1);
    ResLoadFromFile(WAV_CLAW_VOICE_1);
    ResLoadFromFile(WAV_CLAW_HIT1);
    ResLoadFromFile(WAV_CLAW_HIT2);
    ResLoadFromFile(WAV_CLAW_HIT3);
    ResLoadFromFile(WAV_CLAW_HITATHIH);
    ResLoadFromFile(WAV_CLAW_HITATLOW);
    ResLoadFromFile(WAV_OFFICER_HIT1);
    ResLoadFromFile(WAV_OFFICER_HIT2);
    ResLoadFromFile(WAV_SOLDIER_HIT1);
    ResLoadFromFile(WAV_SOLDIER_HIT2);
    ResLoadFromFile(WAV_SOLDIER_HIT3);
    ResLoadFromFile(WAV_SOLDIER_HIT4);
    ResLoadFromFile(WAV_SOLDIER_BASH_ATTACK);
    ResLoadFromFile(WAV_SOLDIER_GUN_ATTACK);
    ResLoadFromFile(WAV_GAME_HIT1);
    ResLoadFromFile(WAV_GAME_HIT2);
    ResLoadFromFile(WAV_GAME_HIT3);
    ResLoadFromFile(WAV_GAME_HIT4);
    ResLoadFromFile(WAV_GAME_COIN);
    ResLoadFromFile(WAV_GAME_AMMUNITION);
    ResLoadFromFile(WAV_GAME_TREASURE);
    ResLoadFromFile(WAV_GAME_CROSS);
    ResLoadFromFile(WAV_GAME_EXTRALIFE);
    ResLoadFromFile(WAV_GAME_RINGS);
    ResLoadFromFile(WAV_GAME_SCEPTER);
    ResLoadFromFile(WAV_GAME_PICKUP1);
    ResLoadFromFile(WAV_GAME_PICKUP2);
    ResLoadFromFile(WAV_GAME_FOODITEM);
    ResLoadFromFile(WAV_GAME_FLAGRISE);
    ResLoadFromFile(WAV_GAME_FLAGWAVE);
    ResLoadFromFile(WAV_GAME_DEATHTILE);
    ResLoadFromFile(WAV_GAME_KILLOFF);
    ResLoadFromFile(WAV_GAME_PEGCRUMBLE);
    ResLoadFromFile(WAV_GAME_SOLHITHI);
    ResLoadFromFile(WAV_GAME_SOLHITL2);
    AssetPopType();

    // Load level1
    SceneDeserialize(world, LEVEL_ROCA);
}

void SceneDealloc(scene_context_t* world)
{
    for (int i = 0; i < world->tileMapCount; ++i) {
        tilemap_t* tileMap = &world->tileMaps[i];
        for (int y = 0; y < world->tileGridHeight; ++y) {
            for (int x = 0; x < world->tileGridWidth; ++x) {
                EntityDealloc(&tileMap->tileGrid[y * world->tileGridWidth + x]);
            }
        }
    }

    ECSDealloc(&world->ecs);

    printf("[INFO][SceneManager]: Scene deallocated successfully.\n");
}

sf::Vector2u SceneGetGridPos(const scene_context_t* world, const sf::Vector2f& viewPos)
{
    return sf::Vector2u(viewPos / (float) world->tileSize);
}

sf::Vector2f SceneGetTileStartPos(const scene_context_t* world, const sf::Vector2f& viewPos)
{
    sf::Vector2u girdPos = SceneGetGridPos(world, viewPos);
    sf::Vector2f tileStartPos(girdPos.x * world->tileSize, girdPos.y * world->tileSize);
    return tileStartPos;
}

entity_t* SceneGetTileWithPos(const scene_context_t* world, const sf::Vector2f& pos)
{
    return SceneGetTileWithPos(world, pos.x, pos.y);
}

entity_t* SceneGetTileWithPos(const scene_context_t* world, float x, float y)
{
    entity_t* result = nullptr;
    int gridPosX = (int) (x / (float) world->tileSize);
    int gridPosY = (int) (y / (float) world->tileSize);

    if (gridPosX >= 0 && gridPosX < world->tileGridWidth &&
        gridPosY >= 0 && gridPosY < world->tileGridHeight) {
        tilemap_t* tileMap = &world->tileMaps[world->tileMapIndex];
        result = tileMap->tileGrid[gridPosY * world->tileGridWidth + gridPosX];
    }

    return result;
}

entity_t* SceneGetTileWithIndex(const scene_context_t* world, int tileMapIndex, int x, int y)
{
    entity_t* result = nullptr;
    if (x >= 0 && x < world->tileGridWidth &&
        y >= 0 && y < world->tileGridHeight) {
        tilemap_t* tileMap = &world->tileMaps[tileMapIndex];
        result = tileMap->tileGrid[y * world->tileGridWidth + x];
    }

    return result;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, int x, int y)
{
    if (!SceneIsValidTile(world, x, y)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return false;
    }

    entity_t* e = SceneGetTileWithIndex(world, world->tileMapIndex, x, y);
    if (e) {
        SceneRemoveEntity(world, e, world->tileMapIndex);
        EntityDealloc(&e);
    }

    tilemap_t* tileMap = &world->tileMaps[world->tileMapIndex];
    tileMap->tileGrid[y * world->tileGridWidth + x] = entity;
    printf("[INFO][SceneManager]: Tile Placed.\n");
    return true;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, const sf::Vector2i& tilePos)
{
    return SceneAddTile(world, entity, tilePos.x, tilePos.y);
}

entity_t* SceneRemoveEntity(scene_context_t* world, const entity_t* entity, int tileMapIndex)
{
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            entity_t* e = SceneGetTileWithIndex(world, tileMapIndex, x, y);
            if (e == entity) {
                printf("[INFO][SceneManager]: Entity removed.\n");
                tilemap_t* tileMap = &world->tileMaps[tileMapIndex];
                tileMap->tileGrid[y * world->tileGridWidth + x] = nullptr;
                return e;
            }
        }
    }

    return nullptr;
}

bool SceneIsValidTile(const scene_context_t* world, int x, int y)
{
    return (x >= 0 && x < world->tileGridWidth) && (y >= 0 && y < world->tileGridHeight);
}

bool SceneIsValidTile(const scene_context_t* world, const sf::Vector2i& pos)
{
    return SceneIsValidTile(world, pos.x, pos.y);
}

bool SceneIsEntityHitTile(const scene_context_t* world, float x, float y, entity_t** out)
{
    entity_t* e = SceneGetTileWithPos(world, x, y);
    bool isHit = (e != nullptr);

    if (out) {
        *out = e;
    }

    return isHit;
}

bool SceneIsEntityHitTile(const scene_context_t* world, const sf::Vector2f& point, entity_t** out)
{
    return SceneIsEntityHitTile(world, point.x, point.y, out);
}

void SceneSetTileIndex(scene_context_t* world, int index)
{
    if (index < 0 || index >= world->tileMapCount) {
        printf("[ERROR][SceneManager]: invalid tilemap index %d.\n", index);
        return;
    }

    world->tileMapIndex = index;
}