#include "c_checkpoint.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "sound_sys.h"
#include "c_collider.h"

unsigned long long latestCheckPointID;

void CheckPointUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs, ECS* ecs)
{
    entity_t* claw = ecs->entitiesRegistry[playerID];

    for (auto& eID: entityIDs) {
        c_checkpoint_t* checkpoint = (c_checkpoint_t*) ECSGet(ecs, eID, C_CHECKPOINT);
        Animator* animator = (Animator*) ECSGet(ecs, eID, C_ANIMATOR);
        c_collider_t* collider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);

        if (checkpoint->isActive) {
            continue;
        }

        if (CheckCollision(claw->collider, *collider, nullptr)) {
            if (animator->state == ANIMATOR_STATE_STOPPED) {
                Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(OBJ_SUPERCHECKPOINT_FLAGRISE), false);
                AnimPlay(animator, &animation);
                SoundPlay(&ResSoundBuffGet(WAV_GAME_FLAGRISE));
                latestCheckPointID = eID;
            }
        }

        if (animator->state == ANIMATOR_STATE_PLAYING && AnimGetNormalizedTime(animator) >= 1.0f) {
            Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(OBJ_SUPERCHECKPOINT_FLAGWAVE), true);
            AnimPlay(animator, &animation);
            SoundPlay(&ResSoundBuffGet(WAV_GAME_FLAGWAVE));
            checkpoint->isActive = true;
        }
    }

    // NOTE(Tony): Maybe this should be moved into a separate file ?
    if (claw->damageable.health <= 0 && claw->damageable.lives <= 0) {
        sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, latestCheckPointID, C_TRANSFORM);
        claw->transform.setPosition(transform->getPosition());
        claw->damageable.health = 100;
        claw->damageable.lives = 6;
    }
}