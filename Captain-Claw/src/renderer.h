#pragma once

#include "SFML/Graphics.hpp"

struct render_context_t {
    sf::View worldView;
    sf::View uiView;
};

extern sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight);
void DrawOutlineFloatRect(const sf::FloatRect& rect, sf::Color color = sf::Color::Green);
void DrawOutlinePoint(const sf::Vector2f& position, float radius, int pointCount = 10,
                      sf::Color color = sf::Color::Cyan, const sf::RenderStates& states = sf::RenderStates::Default);