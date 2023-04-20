#include "SFML/System.hpp"

#include "claw.h"
#include "renderer.h"
#include "resource_manager.h"
#include "scene_manager.h"

void HandleEvent(render_context_t* renderContext);
void UpdateAndRender(render_context_t* renderContext, scene_context_t* world, sf::Time deltaTime);

void ClawMain()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT), WINDOW_NAME);
    rWindow = &window;

    render_context_t renderContext = {};
    RendererInit(&renderContext, WINDOW_START_WIDTH, WINDOW_START_HEIGHT);

    scene_context_t world = {};
    world.tileGridWidth = MAX_GRID_SIZE;
    world.tileGridHeight = MAX_GRID_SIZE;
    world.tileSize = 64;
    entity_t* tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {};
    world.tileGrid = (entity_t**) tileGrid;
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

    rWindow->clear();
    rWindow->setView(renderContext->worldView);
    DrawWorld(world);

    rWindow->setView(renderContext->uiView);
    // TODO(Tony): Draw UI stuff

    rWindow->display();
}