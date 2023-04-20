#include <cmath>

#include "scene_manager.h"
#include "scene_serialization.h"
#include "renderer.h"
#include "resource_manager.h"
#include "entity.h"

void SceneAllocAssets(scene_context_t* world)
{
    // This is fine if we only load 1--2 levels


    AssetPushType(ASSET_SPRITESHEET);
    ResLoadFromFile("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResLoadFromFile("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/SUPERCHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/CRATES.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("characters/OFFICER/OFFICER_IDLE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("characters/SOLDIER/SOLDIER_IDLE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    AssetPopType();
    ResTextureLoadFromSpriteSheet("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResTextureLoadFromSpriteSheet("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/SUPERCHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/CRATES.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("characters/OFFICER/OFFICER_IDLE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("characters/SOLDIER/SOLDIER_IDLE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);

    // Load level1
    SceneDeserialize(world, "../resources/LEVELS/LEVEL1.cscene");
}

void SceneDealloc(scene_context_t* world)
{
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            EntityDealloc(&world->tileGrid[y * world->tileGridWidth + x]);
        }
    }

    for (auto& object: world->objects) {
        EntityDealloc(&object);
    }
    world->objects.clear();

    printf("[INFO][SceneManager]: Scene deallocated successfully.\n");
}

int ScenePopulatedTilesCount(const scene_context_t* world)
{
    int count = 0;
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            if (SceneGetTileWithIndex(world, x, y)) {
                count++;
            }
        }
    }

    return count;
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
        result = world->tileGrid[gridPosY * world->tileGridWidth + gridPosX];
    }

    return result;
}

entity_t* SceneGetTileWithIndex(const scene_context_t* world, int x, int y)
{
    entity_t* result = nullptr;
    if (x >= 0 && x < world->tileGridWidth &&
        y >= 0 && y < world->tileGridHeight) {
        result = world->tileGrid[y * world->tileGridWidth + x];
    }

    return result;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, int x, int y)
{
    if (!SceneIsValidTile(world, x, y)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return false;
    }

    entity_t* e = SceneGetTileWithIndex(world, x, y);
    if (e) {
        SceneRemoveEntity(world, e);
        EntityDealloc(&e);
    }

    world->tileGrid[y * world->tileGridWidth + x] = entity;
    printf("[INFO][SceneManager]: Tile Placed.\n");
    return true;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, const sf::Vector2i& tilePos)
{
    return SceneAddTile(world, entity, tilePos.x, tilePos.y);
}

void SceneAddEntity(scene_context_t* world, entity_t* entity)
{
    world->objects.push_back(entity);
    printf("[INFO][SceneManager]: Object Placed.\n");
}

entity_t* SceneRemoveEntity(scene_context_t* world, const entity_t* entity)
{
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            entity_t* e = SceneGetTileWithIndex(world, x, y);
            if (e == entity) {
                printf("[INFO][SceneManager]: Entity removed.\n");
                world->tileGrid[y * world->tileGridWidth + x] = nullptr;
                return e;
            }
        }
    }

    for (auto it = world->objects.begin(); it != world->objects.end(); ++it) {
        if (*it == entity) {
            entity_t* obj = *it;
            world->objects.erase(it);
            printf("[INFO][SceneManager]: Entity removed.\n");
            return obj;
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

bool SceneIsTileOccupied(const scene_context_t* world, int x, int y)
{
    if (!(x >= 0 && x < world->tileGridWidth) || !(y >= 0 && y < world->tileGridHeight)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return true;
    }

    return SceneGetTileWithIndex(world, x, y);
}

bool SceneIsTileOccupied(const scene_context_t* world, const sf::Vector2i& pos)
{
    return SceneIsTileOccupied(world, pos.x, pos.y);
}

bool SceneIsEntityHitAny(const scene_context_t* world, float x, float y, entity_t** out)
{
    if (SceneIsEntityHitObj(world, x, y, out)) {
        return true;
    }

    if (SceneIsEntityHitTile(world, x, y, out)) {
        return true;
    }

    return false;
}

bool SceneIsEntityHitAny(const scene_context_t* world, const sf::Vector2f& point, entity_t** out)
{
    return SceneIsEntityHitAny(world, point.x, point.y, out);
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

bool SceneIsEntityHitObj(const scene_context_t* world, float x, float y, entity_t** out)
{
    entity_t* e = nullptr;
    bool isHit = false;

    for (auto it = world->objects.rbegin(); it != world->objects.rend(); ++it) {
        entity_t* entity = *it;
        if (entity->render.sprite.getGlobalBounds().contains(x, y)) {
            e = entity;
            isHit = true;
            break;
        }
    }

    if (out) {
        *out = e;
    }

    return isHit;
}

bool SceneIsEntityHitObj(const scene_context_t* world, const sf::Vector2f& point, entity_t** out)
{
    return SceneIsEntityHitObj(world, point.x, point.y, out);
}

void DrawWorld(const scene_context_t* world)
{
    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;
    const int size = world->tileSize;

    // Simple tile culling (Improve FPS)
    int fromX = (drawCenter.x - width) / size - 2;
    int toX = (drawCenter.x + width) / size + 2;
    int fromY = (drawCenter.y - height) / size - 2;
    int toY = (drawCenter.y + height) / size + 2;
    fromX = std::clamp(fromX, 0, (int) world->tileGridWidth - 1);
    toX = std::clamp(toX, 0, (int) world->tileGridHeight - 1);
    fromY = std::clamp(fromY, 0, (int) world->tileGridHeight - 1);
    toY = std::clamp(toY, 0, (int) world->tileGridHeight - 1);

    for (int x = fromX; x < toX; ++x) {
        for (int y = fromY; y < toY; ++y) {
            entity_t* tile = SceneGetTileWithIndex(world, x, y);
            if (!tile)
                continue;

            DrawEntity(tile);
        }
    }

    for (auto& entity: world->objects) {
        DrawEntity(entity);
    }
}