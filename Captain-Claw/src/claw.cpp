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
#include "c_collider.h"
#include "menu.h"

#define DEBUG 0

// TODO(Tony): Fix deltaTime
const sf::Time fixedDeltaTime = sf::seconds(1.0f / 60.0f);
sf::Time timeSinceLastUpdate = sf::Time::Zero;

bool isGamePaused = true;

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
    SceneSetTileIndex(&world, 1);
    for (auto& component: world.ecs.componentList) {
        if (component.second.systemType == C_COLLIDER) {
            ColliderSync(component.second.entityIDs, &world.ecs);
        }
    }

    ClawAlloc(&world.ecs);
    MenuInit();

    unsigned int ambientHandle = SoundPlay(&ResSoundBuffGet(WAV_AMBIENT_TITLE), false);
    sf::Clock deltaClock;
    while (window.isOpen()) {
        if (SoundGetStatus(ambientHandle) != sf::Sound::Status::Playing) {
            ambientHandle = SoundPlay(&ResSoundBuffGet(WAV_AMBIENT_MAPLOOP), true);
        }

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

    captainClaw->animator.adjustOrigin = false;
    Animation clawIdle = AnimAnimationCreate(&spriteSheet);
    AnimPlay(&captainClaw->animator, &clawIdle);

    captainClaw->transform.setPosition(480.0f, 5850.0f);

    c_collider_t hitCollider = ColliderCreate(sf::Vector2f(70.0f, 115.0f), sf::Vector2f(0.0f, -115.0f / 2.0f), false);
    ECSAdd(ecs, captainClaw->ID, C_COLLIDER, &hitCollider);

    c_collider_t swordCollider = ColliderCreate(sf::Vector2f(100.0f, 100.0f),
                                                sf::Vector2f(hitCollider.size.x, -hitCollider.size.y) / 2.0f, true);

    c_damageable_t damageable = {
        .swordCollider= swordCollider,
        .pistolOffset = {30.0f, -90.0f,},
        .health = 100,
        .lives = 1,
    };
    ECSAdd(ecs, captainClaw->ID, C_DAMAGEABLE, &damageable);

    c_physics_t physics = {.isKinematic = false, .useGravity = true};
    ECSAdd(ecs, captainClaw->ID, C_PHYSICS, &physics);

    ECSSetFlag(ecs, captainClaw->ID, C_INVENTORY);
    captainClaw->inventory.ammo_pistol = 1000;
    ECSSetFlag(ecs, captainClaw->ID, C_PLAYER);
    captainClaw->transform.setScale(1, 1);
}

void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime)
{
    rWindow->setView(renderContext->worldView);
    rWindow->clear();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        isGamePaused = true;
    }

    HandleEvent(renderContext);
    if (!isGamePaused) {
        AnimSystemUpdate(deltaTime.asSeconds());
        SoundSystemUpdate();

        timeSinceLastUpdate += deltaTime;
        while (timeSinceLastUpdate >= fixedDeltaTime) {
            timeSinceLastUpdate -= fixedDeltaTime;

            PlayerUpdate(&world->ecs, captainClaw->ID, fixedDeltaTime.asSeconds());

            for (auto& component: world->ecs.componentList) {
                switch (component.second.systemType) {
                    case C_PHYSICS: {
                        PhysicsUpdate(component.second.entityIDs, &world->ecs, world, fixedDeltaTime.asSeconds());
                    }
                        break;
                    case C_COLLIDER: {
                        ColliderSync(component.second.entityIDs, &world->ecs);
                    }
                        break;
                }
            }

            PlayerCameraFollow(&captainClaw->transform, renderContext, fixedDeltaTime.asSeconds());
        }
    }

    // Render first pass
    sf::IntRect cullBox = RendererCalculateCulling(world);
    for (int i = 0; i < world->tileMapCount - 1; ++i) {
        for (int y = cullBox.top; y < cullBox.height; ++y) {
            for (int x = cullBox.left; x < cullBox.width; ++x) {
                entity_t* tile = SceneGetTileWithIndex(world, i, x, y);
                if (!tile) {
                    continue;
                }

                DrawEntity(&tile->render, tile->transform.getTransform() * tile->render.sprite.getTransform());
            }
        }
    }

    // Render second pass
    for (auto& component: world->ecs.componentList) {
        if (component.second.systemType == C_RENDER) {
            DrawEntities(component.second.entityIDs, &world->ecs);
        }
    }

    if (!isGamePaused) {
        for (auto& component: world->ecs.componentList) {
            switch (component.second.systemType) {
                case C_PICKUP: {
                    PickupUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs);
                }
                    break;
                case C_CHECKPOINT: {
                    CheckPointUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs);
                }
                    break;
                case C_ENEMY: {
                    EnemyAIUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
                    PlayerStateAttack(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
                    CombatSystemUpdate(captainClaw->ID, component.second.entityIDs, &world->ecs, deltaTime.asSeconds());
                }
                    break;
            }
        }
    }

    DrawEntity(&captainClaw->render, captainClaw->transform.getTransform() * captainClaw->render.sprite.getTransform());
    // Render third pass
    for (int y = cullBox.top; y < cullBox.height; ++y) {
        for (int x = cullBox.left; x < cullBox.width; ++x) {
            entity_t* tile = SceneGetTileWithIndex(world, world->tileMapCount - 1, x, y);
            if (!tile)
                continue;

            DrawEntity(&tile->render, tile->transform.getTransform() * tile->render.sprite.getTransform());
        }
    }

#if DEBUG
    SceneDrawDebug(world);
    DrawCollider(captainClaw->collider);
    DrawCollider(captainClaw->damageable.swordCollider);
    DrawOutlinePoint(sf::Vector2f(0.0f, 0.0f), 3.0f, sf::Color::Cyan,
                     captainClaw->collider.transform.getTransform());
    DrawOutlinePoint(sf::Vector2f(0.0f, 0.0f), 3.0f, sf::Color::Magenta, captainClaw->transform.getTransform());

    DrawOutlineFloatRect(captainClaw->render.sprite.getGlobalBounds(), sf::Color::White,
                         captainClaw->transform.getTransform() * captainClaw->render.sprite.getTransform());
    DrawOutlinePoint(sf::Vector2f(0.0f, 0.0f), 3.0f, sf::Color::White,
                     captainClaw->transform.getTransform() * captainClaw->render.sprite.getTransform());
#endif

    rWindow->setView(renderContext->uiView);
    // TODO(Tony): Draw UI stuff
    if (isGamePaused) {
        MenuUpdate(deltaTime.asSeconds());
    }

    rWindow->display();
}