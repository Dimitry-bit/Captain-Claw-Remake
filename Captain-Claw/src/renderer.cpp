#include "SFML/Graphics.hpp"

#include "renderer.h"

sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight)
{
    renderContext->uiView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    renderContext->worldView.setSize(rWidth, rHeight);
    renderContext->worldView.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
}

void DrawOutlineFloatRect(const sf::FloatRect& rect, sf::Color color)
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

    rWindow->draw(vertices, 6, sf::LineStrip);
}

void DrawOutlinePoint(const sf::Vector2f& position, float radius, int pointCount, sf::Color color,
                      const sf::RenderStates& states)
{
    sf::CircleShape origin(radius, pointCount);
    origin.setOrigin(radius, radius);
    origin.setPosition(position);
    origin.setFillColor(sf::Color::Transparent);
    origin.setOutlineColor(color);
    origin.setOutlineThickness(1.0f);

    rWindow->draw(origin, states);
}