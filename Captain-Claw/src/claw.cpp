#include "SFML/System.hpp"

#include "claw.h"
#include "renderer.h"
#include "resource_manager.h"
#include "scene_manager.h"
#include "animation.h"
#include "sound_sys.h"
#include "c_render.h"
#include "entity.h"
#include "asset_constants.h"
#include "pickup_system.h"
#include "combat_system.h"
#include "c_checkpoint.h"
#include "c_enemy.h"
#include "c_player.h"
#include "c_physics.h"

void HandleEvent(render_context_t* renderContext);
void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime);
void ClawAlloc(ECS* ecs);

entity_t* captainClaw;

void ClawMain()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT), WINDOW_NAME);
    rWindow = &window;

    render_context_t renderContext = {};
    RendererInit(&renderContext, WINDOW_START_WIDTH, WINDOW_START_HEIGHT);

    scene_context_t world = {};
    world.tileGridWidth = MAX_GRID_WIDTH;
    world.tileGridHeight = MAX_GRID_HEIGHT;
    world.tileSize = 64; // TODO(Tony): replace by #define
    world.tileMapCount = 3;
    entity_t* tileGrid00[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {};
    entity_t* tileGrid01[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {};
    entity_t* tileGrid02[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {};
    tilemap_t tileMaps[3] = {
        {(entity_t**) tileGrid00},
        {(entity_t**) tileGrid01},
        {(entity_t**) tileGrid02}
    };
    world.tileMaps = tileMaps;
    world.tileMapIndex = 0;
    world.ecs = {};
    SceneAllocAssets(&world);
    ClawAlloc(&world.ecs);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        UpdateAndRender(&renderContext, &world, deltaClock.restart());
    }

    SceneDealloc(&world);
    ResUnloadAll();
}

void HandleEvent(render_context_t* renderContext)
{
    sf::Event event;
    while (rWindow->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            rWindow->close();
        } else if (event.type == sf::Event::Resized) {
            // Needed for window resizing
            RendererInit(renderContext, event.size.width, event.size.height);
        }
    }
}

void ClawAlloc(ECS* ecs)
{
    captainClaw = ECSEntityAlloc(ecs);
    const spriteSheet_t& spriteSheet = ResSpriteSheetGet(CHAR_CLAW_IDLE);
    captainClaw->render.sprite.setTexture(spriteSheet.texture);
    captainClaw->render.type = RENDER_SPRITE;
    ECSAdd(ecs, captainClaw->ID, C_RENDER, &captainClaw->render);
    Animation clawIdle = AnimAnimationCreate(&spriteSheet);
    AnimPlay(&captainClaw->animator, &clawIdle);
    captainClaw->render.sprite.setPosition(300.0f, 250.0f);

    c_inventory_t inv{};
    ECSAdd(ecs, captainClaw->ID, C_INVENTORY, &inv);

    c_damageable_t damageable = {
        .swordCollider= {10, -90.0f, 120, 90},
        .pistolOffset = {30.0f, -90.0f,},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, captainClaw->ID, C_DAMAGEABLE, &damageable);

    c_player_t player = {.state = PLAYER_STATE_IDLE};
    ECSAdd(ecs, captainClaw->ID, C_PLAYER, &player);
    c_physics_t physics = {.isKinematic = false, .useGravity = true};
    ECSAdd(ecs, captainClaw->ID, C_PHYSICS, &physics);
}

void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime)
{
    SceneSetTileIndex(world, 1);
    HandleEvent(renderContext);
    AnimSystemUpdate(deltaTime.asSeconds());
    SoundSystemUpdate();
    PlayerUpdate(captainClaw, deltaTime.asSeconds());

#if 0
    captainClaw->render.sprite.setPosition(rWindow->mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition(*rWindow))));
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        captainClaw->render.sprite.setScale(-captainClaw->render.sprite.getScale().x,
                                            captainClaw->render.sprite.getScale().y);
    }
#endif

    rWindow->clear();
    rWindow->setView(renderContext->worldView);

    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;

    int fromX = (drawCenter.x - width) / world->tileSize - 2;
    int toX = (drawCenter.x + width) / world->tileSize + 2;
    int fromY = (drawCenter.y - height) / world->tileSize - 2;
    int toY = (drawCenter.y + height) / world->tileSize + 2;

    fromX = std::clamp(fromX, 0, (int) world->tileGridWidth - 1);
    toX = std::clamp(toX, 0, (int) world->tileGridWidth);
    fromY = std::clamp(fromY, 0, (int) world->tileGridHeight - 1);
    toY = std::clamp(toY, 0, (int) world->tileGridHeight);

    // Render first pass
    for (int i = 0; i < world->tileMapCount - 1; ++i) {
        for (int x = fromX; x < toX; ++x) {
            for (int y = fromY; y < toY; ++y) {
                entity_t* tile = SceneGetTileWithIndex(world, i, x, y);
                if (!tile)
                    continue;

                DrawEntity(tile);
            }
        }
    }

    // Render second pass
    for (auto& component: world->ecs.componentList) {
        switch (component.second.systemType) {
            case C_NONE:break;
            case C_TILE:break;
            case C_PICKUP: {
                PickupUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs);
            }
                break;
            case C_CHECKPOINT: {
                CheckPointUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs);
            }
                break;
            case C_ENEMY: {
                PlayerStateAttack(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
                EnemyAIUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
                CombatSystemUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
            }
                break;
            case C_PLATFORM:break;
            case C_SOUND:break;
            case C_RENDER: {
                DrawEntity(component.second.entityIDs, &world->ecs);
            }
                break;
            case C_DAMAGEABLE:break;
            case C_INVENTORY:break;
            case C_ANIMATOR:break;
            case C_PLAYER:break;
            case C_PHYSICS: {
                PhysicsUpdate(component.second.entityIDs, &world->ecs, world, deltaTime.asSeconds());
            }
                break;
        }
    }

    // Render third pass
    for (int x = fromX; x < toX; ++x) {
        for (int y = fromY; y < toY; ++y) {
            entity_t* tile = SceneGetTileWithIndex(world, world->tileMapCount - 1, x, y);
            if (!tile)
                continue;

            DrawEntity(tile);
        }
    }

#if 0
    sf::CircleShape pistolPoint;
    pistolPoint.setRadius(5.0f);
    pistolPoint.setFillColor(Color::White);
    sf::Vector2f pistolPointPos;
    pistolPointPos.y = captainClaw->damageable.pistolOffset.y + captainClaw->render.sprite.getPosition().y;
    pistolPointPos.x = captainClaw->render.sprite.getPosition().x;
    if (captainClaw->render.sprite.getScale().x > 0) {
        pistolPointPos.x += captainClaw->damageable.pistolOffset.x;
    } else if (captainClaw->render.sprite.getScale().x < 0) {
        pistolPointPos.x -= captainClaw->damageable.pistolOffset.x;
    }
    pistolPoint.setPosition(pistolPointPos);

    sf::RectangleShape swordCollider;
    swordCollider.setFillColor(sf::Color(255, 255, 255, 50));
    swordCollider.setSize(sf::Vector2f(captainClaw->damageable.swordCollider.width,
                                       captainClaw->damageable.swordCollider.height));
    sf::Vector2f pos;
    pos = captainClaw->render.sprite.getPosition();
    pos.y += captainClaw->damageable.swordCollider.top;
    if (captainClaw->render.sprite.getScale().x > 0) {
        pos.x += captainClaw->damageable.swordCollider.left;
    } else {
        pos.x -= (captainClaw->damageable.swordCollider.width + captainClaw->damageable.swordCollider.left);
    }
    swordCollider.setPosition(pos);
    rWindow->draw(pistolPoint);
    rWindow->draw(swordCollider);
#endif
    sf::CircleShape origin(5.0f);
    origin.setOrigin(5.0f, 5.0f);
    origin.setPosition(captainClaw->render.sprite.getPosition());
    rWindow->draw(origin);
#if 1
    sf::RectangleShape collider;
    collider.setPosition(captainClaw->render.sprite.getGlobalBounds().left,
                         captainClaw->render.sprite.getGlobalBounds().top);
    collider.setSize(sf::Vector2f(captainClaw->render.sprite.getGlobalBounds().width,
                                  captainClaw->render.sprite.getGlobalBounds().height));
    collider.setFillColor(sf::Color::Transparent);
    collider.setOutlineColor(sf::Color::Red);
    collider.setOutlineThickness(3.0f);
    rWindow->draw(collider);
#endif

    rWindow->setView(renderContext->uiView);
    // TODO(Tony): Draw UI stuff

    rWindow->display();
}