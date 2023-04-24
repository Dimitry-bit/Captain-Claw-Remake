#include <random>

#include "c_enemy.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "sound_sys.h"
#include "vector_math.h"

const char* soldierHitSounds[] = {
    WAV_SOLDIER_HIT1,
    WAV_SOLDIER_HIT2,
    WAV_SOLDIER_HIT3,
    WAV_SOLDIER_HIT4,
};
const int soldierHitSoundsCount = 4;

const char* officerHitSounds[] = {
    WAV_OFFICER_HIT1,
    WAV_OFFICER_HIT2,
};
const int officerHitSoundsCount = 2;

const int maxIdlePeriod = 5;
const float hitPeriod = 0.8f;
const float soliderSpeed = 100.0f;
const float arriveThreshold = 15.0f;

// TODO(Tony): Pull types and their associated hit/death sounds and animations in a map

void EnemyAIStateIDLE(c_enemy_t* enemy, Animator* animator, float deltaTime);
void EnemyAIStateMoving(c_enemy_t* enemy, Animator* animator, c_render_t* render, float deltaTime);
void EnemyAIStateAttack(unsigned long long playerID, c_enemy_t* enemy, Animator* animator, c_render_t* render);
void EnemyAIStateHit(c_enemy_t* enemy, Animator* animator);
void EnemyAIStateRecovering(c_enemy_t* enemy, Animator* animator, float deltaTime);
void EnemyAIStateDeath(c_enemy_t* enemy, Animator* animator);

float RandomFloat(float min, float max);

void EnemyAIUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                   ECS* ecs, float deltaTime)
{
    for (auto& eID: entityIDs) {
        c_enemy_t* enemy = (c_enemy_t*) ECSGet(ecs, eID, C_ENEMY);
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);
        Animator* animator = (Animator*) ECSGet(ecs, eID, C_ANIMATOR);

        switch (enemy->state) {
            case ENEMY_STATE_IDLE: {
                EnemyAIStateIDLE(enemy, animator, deltaTime);
            }
                break;
            case ENEMY_STATE_MOVING: {
                EnemyAIStateMoving(enemy, animator, render, deltaTime);
            }
                break;
            case ENEMY_STATE_ATTACKING: {
                EnemyAIStateAttack(playerID, enemy, animator, render);
            }
                break;
            case ENEMY_STATE_HIT: {
                EnemyAIStateHit(enemy, animator);
            }
                break;
            case ENEMY_STATE_RECOVERING: {
                EnemyAIStateRecovering(enemy, animator, deltaTime);
            }
                break;
            case ENEMY_STATE_DEATH: {
                EnemyAIStateDeath(enemy, animator);
            }
                break;
        }
    }
}

void EnemyAIStateIDLE(c_enemy_t* enemy, Animator* animator, float deltaTime)
{
    if (enemy->idlePeriod <= 0.0f) {
        enemy->idlePeriod = rand() % maxIdlePeriod;
    }

    if (AnimGetRunningAnimName(animator) != CHAR_SOLDIER_IDLE) {
        Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_IDLE), true, 0.4f);
        AnimPlay(animator, &anim);
    }

    enemy->idleTimer += deltaTime;
    if (enemy->idleTimer >= enemy->idlePeriod) {
        enemy->idleTimer = 0.0f;
        enemy->idlePeriod = rand() % maxIdlePeriod;

        float posX = RandomFloat(-enemy->min.x, enemy->max.x);
        enemy->targetPoint.x = enemy->startPos.x + posX;
        enemy->state = ENEMY_STATE_MOVING;
    }
}

void EnemyAIStateMoving(c_enemy_t* enemy, Animator* animator, c_render_t* render, float deltaTime)
{
    if (AnimGetRunningAnimName(animator) != CHAR_SOLDIER_WALK) {
        Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_WALK), true);
        AnimPlay(animator, &anim);
    }

    float displacement = fabsf(enemy->targetPoint.x - render->sprite.getPosition().x);
    if (displacement > arriveThreshold) {
        sf::Vector2f direction = VectorNormalize(enemy->targetPoint - render->sprite.getPosition());
        direction.y = 0;

        sf::Vector2f scale = render->sprite.getScale();
        if (enemy->targetPoint.x > render->sprite.getPosition().x) {
            if (scale.x > 0) {
                scale.x *= -1;
                render->sprite.setScale(scale.x, scale.y);
            }
        } else if (scale.x < 0) {
            scale.x *= -1;
            render->sprite.setScale(scale.x, scale.y);
        }

        VectorNormalize(direction);
        render->sprite.move(direction * deltaTime * soliderSpeed);
    } else {
        enemy->state = ENEMY_STATE_IDLE;
    }
}

void EnemyAIStateAttack(unsigned long long playerID, c_enemy_t* enemy, Animator* animator, c_render_t* render)
{
    if (AnimGetRunningAnimName(animator) != CHAR_SOLDIER_WALK) {
        Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_GUN_ATTACK), true);
        AnimPlay(animator, &anim);
    }
}

void EnemyAIStateHit(c_enemy_t* enemy, Animator* animator)
{
    Animation animation;
    int htiSoundIndex = rand();
    if (enemy->type == ENEMY_SOLDIER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_HURT_STANCES), false, 1);
        htiSoundIndex %= soldierHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(soldierHitSounds[htiSoundIndex]), false);
    } else if (enemy->type == ENEMY_OFFICER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_HURT_STANCES), false, 1);
        htiSoundIndex %= officerHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(officerHitSounds[htiSoundIndex]), false);
    }

    AnimPlay(animator, &animation);
    enemy->state = ENEMY_STATE_RECOVERING;
}

void EnemyAIStateRecovering(c_enemy_t* enemy, Animator* animator, float deltaTime)
{
    enemy->hitTimer += deltaTime;
    if (AnimGetNormalizedTime(animator) >= 1.0f && enemy->hitTimer > hitPeriod) {
        enemy->hitTimer = 0.0f;
        enemy->state = ENEMY_STATE_IDLE;
    }
}

void EnemyAIStateDeath(c_enemy_t* enemy, Animator* animator)
{
    Animation animation;
    int htiSoundIndex = rand();
    if (enemy->type == ENEMY_SOLDIER && AnimGetRunningAnimName(animator) != CHAR_SOLDIER_DEATH) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_DEATH), false, 1);
        htiSoundIndex %= soldierHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(soldierHitSounds[htiSoundIndex]), false);
        AnimPlay(animator, &animation);
    } else if (enemy->type == ENEMY_OFFICER && AnimGetRunningAnimName(animator) != CHAR_OFFICER_DEATH) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_DEATH), false, 1);
        htiSoundIndex %= officerHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(officerHitSounds[htiSoundIndex]), false);
        AnimPlay(animator, &animation);
    }
}

float RandomFloat(float min, float max)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;
    return (random * range) + min;
}