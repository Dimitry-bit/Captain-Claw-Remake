#include "c_render.h"
#include "renderer.h"

void DrawEntities(const std::unordered_set<unsigned long long>& entityIDs, ECS* ecs)
{
    for (auto& eID: entityIDs) {
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);
        DrawEntity(render);
    }
}

void DrawEntity(const c_render_t* render)
{
    if (!render) {
        return;
    }

    if (render->type == RENDER_SPRITE) {
        rWindow->draw(render->sprite);
    } else if (render->type == RENDER_RECTANGLE) {
        rWindow->draw(render->rectangleShape);
    } else if (render->type == RENDER_CIRCLE) {
        rWindow->draw(render->circleShape);
    }
}