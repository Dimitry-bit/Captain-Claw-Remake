#include "c_render.h"
#include "renderer.h"
#include "c_physics.h"

void DrawEntities(const std::unordered_set<unsigned long long>& entityIDs, ECS* ecs)
{
    for (auto& eID: entityIDs) {
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);
        sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, eID, C_TRANSFORM);
        DrawEntity(render, transform->getTransform() * render->sprite.getTransform());

        if (ECSHas(ecs, eID, C_COLLIDER)) {
            c_collider_t* collider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);
            DrawCollider(*collider);
        }
    }
}

void DrawEntity(const c_render_t* render, const sf::RenderStates& states)
{
    if (!render) {
        return;
    }

    if (render->type == RENDER_SPRITE) {
        rWindow->draw(render->sprite, states);
    } else if (render->type == RENDER_RECTANGLE) {
        rWindow->draw(render->rectangleShape, states);
    } else if (render->type == RENDER_CIRCLE) {
        rWindow->draw(render->circleShape, states);
    }
}