#pragma once

#include "SFML/Graphics.hpp"
#include "scene_manager.h"

struct render_context_t {
    sf::View worldView;
    sf::View uiView;
};

extern sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight);
sf::IntRect RendererCalculateCulling(const scene_context_t* world);

void DrawOutlineFloatRect(const sf::FloatRect& rect, sf::Color color = sf::Color::White,
                          const sf::RenderStates& states = sf::RenderStates::Default);
void DrawOutlinePoint(const sf::Vector2f& position, float radius, sf::Color color = sf::Color::Cyan,
                      const sf::RenderStates& states = sf::RenderStates::Default);