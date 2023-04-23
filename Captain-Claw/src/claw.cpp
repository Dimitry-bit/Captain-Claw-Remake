#include "SFML/System.hpp"

#include "claw.h"
#include "renderer.h"
#include "resource_manager.h"
#include "scene_manager.h"
#include "animation.h"
#include "sound_sys.h"
#include "c_render.h"

void HandleEvent(render_context_t* renderContext);
void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime);

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

void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime)
{
    HandleEvent(renderContext);
    AnimSystemUpdate(deltaTime.asSeconds());
    SoundSystemUpdate();

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
            case C_PICKUP:break;
            case C_CHECKPOINT:break;
            case C_ENEMY:break;
            case C_PLATFORM:break;
            case C_SOUND:break;
            case C_RENDER: {
                DrawEntity(component.second.entityIDs, &world->ecs);
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

    rWindow->setView(renderContext->uiView);
    // TODO(Tony): Draw UI stuff

    rWindow->display();
}