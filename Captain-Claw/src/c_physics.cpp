#include "SFML/Graphics.hpp"

#include "c_physics.h"
#include "scene_manager.h"

const float dampMultiplier = 4.0f;
const float gravityMultiplier = 0.08f;

// TODO(Tony): Pull in a separate file
float Squaref(float a)
{
    return a * a;
}

bool CheckCollision(sf::Sprite& a, sf::Sprite& b, float inertia, sf::Vector2i* hitNormal)
{
    sf::Vector2i normal = sf::Vector2i(0, 0);
    if (hitNormal) {
        *hitNormal = normal;
    }

    sf::FloatRect bGlobalBounds = b.getGlobalBounds();
    sf::Vector2f bHalfExtent = sf::Vector2f(bGlobalBounds.width / 2.0f, bGlobalBounds.height / 2.0f);
    sf::Vector2f bPos(bGlobalBounds.left + bHalfExtent.x, bGlobalBounds.top + bHalfExtent.y);

    sf::FloatRect aGlobalBounds = a.getGlobalBounds();
    sf::Vector2f aHalfExtent = sf::Vector2f(aGlobalBounds.width / 2.0f, aGlobalBounds.height / 2.0f);
    sf::Vector2f aPos(aGlobalBounds.left + aHalfExtent.x, aGlobalBounds.top + aHalfExtent.y);

    float dX = bPos.x - aPos.x;
    float dY = bPos.y - aPos.y;
    float intersectX = abs(dX) - (bHalfExtent.x + aHalfExtent.x);
    float intersectY = abs(dY) - (bHalfExtent.y + aHalfExtent.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
        inertia = std::clamp(inertia, 0.0f, 1.0f);

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

void PhysicsUpdate(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs, scene_context_t* world, float deltaTime)
{
    // TODO(Tony): Cleanup collision response code
    for (auto& eID: entityIDS) {
        c_physics_t* physics = (c_physics_t*) ECSGet(ecs, eID, C_PHYSICS);
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

        physics->isGrounded = false;
        for (int y = 0; y < world->tileGridHeight; ++y) {
            for (int x = 0; x < world->tileGridWidth; ++x) {
                entity_t* tileEntity = SceneGetTileWithIndex(world, 1, x, y);
                if (!tileEntity) {
                    continue;
                }

                sf::Vector2i normal;
                if (CheckCollision(render->sprite, tileEntity->render.sprite, 0.0f, &normal)) {
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

        if (physics->useGravity && !physics->isGrounded) {
            physics->velocity.y += 9.8f * gravityMultiplier;
        }

        physics->acceleration.x += -dampMultiplier * physics->velocity.x;
        sf::Vector2f displacement = 0.5f * physics->acceleration * Squaref(deltaTime) + physics->velocity * deltaTime;
        physics->velocity += physics->acceleration * deltaTime;

        render->sprite.move(displacement);
    }
}