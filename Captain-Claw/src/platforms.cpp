#include <cmath>
#include <unordered_set>
#include "platforms.h"
#include "vector_math.h"
#include "c_collider.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"

const float elevatorSpeed = 100.0f;
const float arriveThreshold = 10.0f;
const float crumblePeriod = 0.3f;

std::vector<unsigned long long> toRemove = {};

void PlatformUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                    ECS* ecs, float deltaTime)
{
    c_collider_t* playerCollider = (c_collider_t*) ECSGet(ecs, playerID, C_COLLIDER);
    sf::Transformable* playerTransform = (sf::Transformable*) ECSGet(ecs, playerID, C_TRANSFORM);

    for (unsigned long long eID: entityIDs) {
        c_platform_t* platform = (c_platform_t*) ECSGet(ecs, eID, C_PLATFORM);
        c_collider_t* platformCollider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);
        Animator* animator = (Animator*) ECSGet(ecs, eID, C_ANIMATOR);

        if (platform->type == PLATFORM_ELEVATOR) {
            sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, eID, C_TRANSFORM);

            sf::Vector2f destination = (platform->starting + platform->offset) - transform->getPosition();
            if (fabs(destination.x) <= arriveThreshold && fabs(destination.y) <= arriveThreshold) {
                if (platform->offset == platform->b) {
                    platform->offset = platform->a;
                } else {
                    platform->offset = platform->b;
                }
            }

            destination = VectorNormalize(destination);
            transform->move(destination * elevatorSpeed * deltaTime);
            sf::Vector2i normal;
            if (CheckCollision(*playerCollider, *platformCollider, &normal)) {
                if (normal.y < 0) {
                    playerTransform->move(destination * elevatorSpeed * deltaTime);
                }
            }
        } else if (platform->type == PLATFORM_CRUMBLINGPEG) {
            sf::Vector2i normal;
            if (CheckCollision(*playerCollider, *platformCollider, &normal)) {
                if (normal.y < 0) {
                    platform->timer += deltaTime;
                    if (platform->timer >= crumblePeriod) {
                        if (animator->state != ANIMATOR_STATE_PLAYING) {
                            Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(OBJ_CRUMBLINGPEG), false);
                            AnimPlay(animator, &animation);
                        }
                    }
                }
            } else {
                platform->timer = 0.0f;
            }

            if (animator->state == ANIMATOR_STATE_PLAYING && AnimGetNormalizedTime(animator) >= 0.9f) {
                toRemove.emplace_back(eID);
            }
        }
    }

    for (auto& eID: toRemove) {
        ECSEntityDealloc(ecs, eID);
    }
    toRemove.clear();
}