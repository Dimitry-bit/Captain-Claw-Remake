#include "SFML/Graphics.hpp"

#include "Player Movement.h"
#include "scene_manager.h"

const float dampMultiplier = 5.5f;

// TODO(Tony): Pull in a separate file
float Squaref(float a)
{
    return a * a;
}

void EntityMove(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs, scene_context_t* world, float deltaTime)
{
    for (auto& eID: entityIDS) {
        c_physics_t* physics = (c_physics_t*) ECSGet(ecs, eID, C_PHYSICS);
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

        physics->acceleration += -dampMultiplier * physics->velocity;
        sf::Vector2f displacement = 0.5f * physics->acceleration * Squaref(deltaTime) + physics->velocity * deltaTime;
        physics->velocity += physics->acceleration * deltaTime;

        render->sprite.move(displacement);
    }
}