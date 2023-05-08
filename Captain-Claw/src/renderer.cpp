#include "SFML/Graphics.hpp"

#include "renderer.h"
#include "scene_manager.h"

sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight)
{
    renderContext->uiView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    renderContext->worldView.setSize(rWidth, rHeight);
    renderContext->worldView.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
    renderContext->worldView.zoom(0.8f);
}

sf::IntRect RendererCalculateCulling(const scene_context_t* world)
{
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

    return {fromX, fromY, toX, toY};
}

void DrawOutlineFloatRect(const sf::FloatRect& rect, sf::Color color, const sf::RenderStates& states)
{
    sf::Vertex vertices[6] = {};
    for (auto& vertex: vertices) {
        vertex.color = color;
    }

    vertices[0].position = sf::Vector2f(rect.left, rect.top);
    vertices[1].position = sf::Vector2f(rect.left + rect.width, rect.top);
    vertices[2].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    vertices[3].position = sf::Vector2f(rect.left, rect.top + rect.height);
    vertices[4].position = sf::Vector2f(rect.left, rect.top);
    vertices[5].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);

    rWindow->draw(vertices, 6, sf::LineStrip, states);
}

void DrawOutlinePoint(const sf::Vector2f& position, float radius, sf::Color color, const sf::RenderStates& states)
{
    sf::CircleShape origin(radius, 10);
    origin.setOrigin(radius, radius);
    origin.setPosition(position);
    origin.setFillColor(sf::Color::Transparent);
    origin.setOutlineColor(color);
    origin.setOutlineThickness(1.0f);

    rWindow->draw(origin, states);
}