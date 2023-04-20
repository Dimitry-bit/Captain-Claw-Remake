#pragma once

#include "SFML/Graphics.hpp"

struct render_context_t {
    sf::View worldView;
    sf::View uiView;
};

extern sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight);