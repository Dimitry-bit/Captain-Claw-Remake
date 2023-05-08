#include <cmath>
#include "SFML/Graphics.hpp"

#include "c_physics.h"
#include "c_collider.h"
#include "scene_manager.h"
#include "renderer.h"

// Note(Tony): Should be related to the world not physics
const float dampMultiplier = 4.0f;
const float gravityMultiplier = 1.0f;

void CollisionResponse(unsigned long long eID, ECS* ecs, scene_context_t* world)
{
    // TODO(Tony): Cleanup collision response code
    if (!ECSHas(ecs, eID, C_COLLIDER)) {
        return;
    }

    c_physics_t* physics = (c_physics_t*) ECSGet(ecs, eID, C_PHYSICS);
    sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, eID, C_TRANSFORM);
    c_collider_t* collider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);

    collider->transform = *transform;
    collider->transform.move(collider->offset);

    physics->isGrounded = false;
    physics->isClimb = false;

    // NOTE(Tony): Note sure if I should use culling here (BOOST FPS)
    sf::IntRect cullBox = RendererCalculateCulling(world);
    for (int y = cullBox.top; y < cullBox.height; ++y) {
        for (int x = cullBox.left; x < cullBox.width; ++x) {
            entity_t* tileEntity = SceneGetTileWithIndex(world, 1, x, y);
            if (!tileEntity) {
                continue;
            }
            c_collider_t* tileCollider = &tileEntity->collider;

            tileCollider->transform = tileEntity->transform;
            tileCollider->transform.move(tileCollider->offset);

            if (tileEntity->tile.type == TILE_CLEAR) {
                continue;
            }

            sf::Vector2i normal;
            if (CheckCollision(*collider, tileEntity->collider, &normal, transform, &tileEntity->transform, 0.0f)) {
                if (!physics->isClimb) {
                    physics->isClimb = tileEntity->tile.type == TILE_CLIMBABLE;
                }

                if (tileEntity->tile.type == TILE_CLIMBABLE) {
                    physics->isGrounded = true;
                }

                if (collider->isTrigger || tileCollider->isTrigger) {
                    continue;
                }

                if (tileEntity->tile.type == TILE_CLEAR) {
                    continue;
                }

                if (normal.y < 0) {
                    if (physics->isGrounded)
                        continue;

                        if (physics->velocity.y > 0.0f) {
                            physics->velocity.y = 0.0f;
                        }

                    physics->isGrounded = true;
                } else if (normal.y > 0) {
                    if (physics->velocity.y < 0.0f) {
                        physics->velocity.y = 0.0f;
                    }
                } else if (normal.x > 0) {
                    if (physics->velocity.x < 0.0f) {
//                        physics->velocity.x = 0.0f;
                    }
                } else if (normal.x < 0) {
                    if (physics->velocity.x > 0.0f) {
//                        physics->velocity.x = 0.0f;
                    }
                }
            }
        }
    }
}

void PhysicsUpdate(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs, scene_context_t* world, float deltaTime)
{
    for (auto& eID: entityIDS) {
        c_physics_t* physics = (c_physics_t*) ECSGet(ecs, eID, C_PHYSICS);
        sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, eID, C_TRANSFORM);

        CollisionResponse(eID, ecs, world);

        if (physics->useGravity && !physics->isGrounded) {
            physics->velocity.y += 980.0f * gravityMultiplier * physics->gravityScale * deltaTime;
        }

        physics->acceleration.x += -dampMultiplier * physics->velocity.x;
        sf::Vector2f displacement = 0.5f * physics->acceleration * powf(deltaTime, 2) + physics->velocity * deltaTime;
        physics->velocity += physics->acceleration * deltaTime;

        transform->move(displacement);
    }
}