#include "SFML/Graphics.hpp"

#include "renderer.h"

sf::RenderWindow* rWindow;

void RendererInit(render_context_t* renderContext, int rWidth, int rHeight)
{
    renderContext->uiView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    renderContext->worldView.setSize(rWidth, rHeight);
    renderContext->worldView.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
}