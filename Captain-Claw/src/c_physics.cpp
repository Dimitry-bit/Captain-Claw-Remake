#include <cmath>
#include "SFML/Graphics.hpp"

#include "c_physics.h"
#include "scene_manager.h"

// Note(Tony): Should be related to the world not physics
const float dampMultiplier = 4.0f;
const float gravityMultiplier = 0.08f;

bool CheckCollision(c_collider_t& a, c_collider_t& b, sf::Vector2i* hitNormal)
{
    sf::Vector2i normal = sf::Vector2i(0, 0);
    if (hitNormal) {
        *hitNormal = normal;
    }

    sf::FloatRect aBounds = ColliderGetGlobalBounds(a);
    sf::FloatRect bBounds = ColliderGetGlobalBounds(b);

    sf::Vector2f aHalfExtent = sf::Vector2f(aBounds.width / 2.0f, aBounds.height / 2.0f);
    sf::Vector2f bHalfExtent = sf::Vector2f(bBounds.width / 2.0f, bBounds.height / 2.0f);

    sf::Vector2f aPos = sf::Vector2f(aBounds.left, aBounds.top);
    sf::Vector2f bPos = sf::Vector2f(bBounds.left, bBounds.top);

    float dX = bPos.x - aPos.x;
    float dY = bPos.y - aPos.y;
    float intersectX = abs((int) dX) - (bHalfExtent.x + aHalfExtent.x);
    float intersectY = abs((int) dY) - (bHalfExtent.y + aHalfExtent.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
//        inertia = std::clamp(inertia, 0.0f, 1.0f);

        if (intersectX > intersectY) {
            if (dX > 0.0f) {
                normal = sf::Vector2i(-1, 0);
//                a.move(intersectX * (1.0f - inertia), 0.0f);
//                b.move(-intersectX * inertia, 0.0f);
            } else {
                normal = sf::Vector2i(1, 0);
//                a.move(-intersectX * (1.0f - inertia), 0.0f);
//                b.move(intersectX * inertia, 0.0f);
            }
        } else {
            if (dY > 0.0f) {
                normal = sf::Vector2i(0, -1);
//                a.move(0.0f, intersectY * (1.0f - inertia));
//                b.move(0.0f, -intersectY * inertia);
            } else {
                normal = sf::Vector2i(0, 1);
//                a.move(0.0f, -intersectY * (1.0f - inertia));
//                b.move(0.0f, intersectY * inertia);
            }
        }

        if (hitNormal) {
            *hitNormal = normal;
        }

        return true;
    }

    return false;
}

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
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            entity_t* tileEntity = SceneGetTileWithIndex(world, 1, x, y);
            if (!tileEntity) {
                continue;
            }
            c_collider_t* tileCollider = &tileEntity->collider;

            tileCollider->transform = tileEntity->transform;
            tileCollider->transform.move(tileCollider->offset);

            if (collider->isTrigger || tileCollider->isTrigger) {
                continue;
            }

            sf::Vector2i normal;
            if (CheckCollision(*collider, tileEntity->collider, &normal)) {
                if (tileEntity->tile.type == TILE_CLIMBABLE || tileEntity->tile.type == TILE_CLEAR) {
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
                        physics->velocity.x = 0.0f;
                    }
                } else if (normal.x < 0) {
                    if (physics->velocity.x > 0.0f) {
                        physics->velocity.x = 0.0f;
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
            physics->velocity.y += 9.8f * gravityMultiplier;
        }

        physics->acceleration.x += -dampMultiplier * physics->velocity.x;
        sf::Vector2f displacement = 0.5f * physics->acceleration * powf(deltaTime, 2) + physics->velocity * deltaTime;
        physics->velocity += physics->acceleration * deltaTime;

        transform->move(displacement);
    }
}

c_collider_t PhysicsCreateCollider(const sf::Vector2f& size, const sf::Vector2f& offset, bool isTrigger)
{
    return c_collider_t{
        .offset = offset,
        .size = size,
        .isTrigger = isTrigger,
    };
}

void DrawCollider(const c_collider_t& collider)
{
    sf::FloatRect rect = ColliderGetGlobalBounds(collider);
    DrawOutlineFloatRect(sf::FloatRect(rect.left - rect.width / 2.0f, rect.top - rect.height / 2.0f,
                                       rect.width, rect.height), sf::Color::Green);
}

sf::FloatRect ColliderGetLocalBounds(const c_collider_t& self)
{
    const float width = self.size.x;
    const float height = self.size.y;

    return {0.0f, 0.0f, width, height};
}

sf::FloatRect ColliderGetGlobalBounds(const c_collider_t& self)
{
    sf::FloatRect localBounds = ColliderGetLocalBounds(self);
    // NOTE(Tony): Not sure if this is the right way to translate scale, but hey it works!
    localBounds.width *= (self.transform.getScale().x < 0) ? -1.0f : 1.0f;
    localBounds.height *= (self.transform.getScale().y < 0) ? -1.0f : 1.0f;
    return self.transform.getTransform().transformRect(localBounds);
}