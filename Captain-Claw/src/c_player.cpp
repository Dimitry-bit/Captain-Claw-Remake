#include <cmath>

#include "c_player.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "sound_sys.h"
#include "combat_system.h"
#include "c_collider.h"
#include "transform_utils.h"

const char* clawHitSounds[] = {
    WAV_CLAW_HIT1,
    WAV_CLAW_HIT2,
    WAV_CLAW_HIT3,
    WAV_CLAW_HITATHIH,
    WAV_CLAW_HITATLOW
};
const int clawHitSoundsCount = 5;

const float recoverPeriod = 0.5f;
float recoverTimer = 0.0f;
const float attackPeriod = 0.5f;
float attackTimer;
const int swordDmg = 25;

const float playerSpeed = 1000.0f;
const float stopMovingThreshold = 35.0f;

float timeInAir = 0.0f;
const float jumpImpulseTime = 0.1f;
const float jumpImpulseVel = 270.0f;
const float jumpAccel = 25.0f;
const float MAX_AIR_TIME = 0.4f;

// NOTE(Tony): hmmmmm
const float climbSpeed = 200000.0f;

void PlayerLocomotion(c_player_t* player, c_physics_t* physics, sf::Transformable* transform,
                      c_render_t* render, float deltaTime)
{
    physics->acceleration = {};

    if (physics->isGrounded) {
        timeInAir = 0.0f;
    } else {
        timeInAir += deltaTime;
    }

    if (physics->isClimb) {
        physics->gravityScale = 0.0f;
        physics->velocity.y = 0;
    } else {
        physics->gravityScale = 1.0f;
    }

    if (player->state == PLAYER_STATE_IDLE || player->state == PLAYER_STATE_MOVING) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            physics->acceleration.x = 1.0f;

            sf::Vector2f scale = transform->getScale();
            if (scale.x < 0.0f) {
                transform->setScale(-1.0f * scale.x, scale.y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            physics->acceleration.x = -1.0f;

            sf::Vector2f scale = transform->getScale();
            if (scale.x > 0.0f) {
                transform->setScale(-1.0f * scale.x, scale.y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (timeInAir < jumpImpulseTime) {
                physics->velocity.y = -jumpImpulseVel;
                physics->useGravity = false;
            } else if (timeInAir < MAX_AIR_TIME) {
                physics->acceleration.y = -jumpAccel;
            } else {
                physics->useGravity = true;
            }
        } else {
            physics->useGravity = true;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Up) && physics->isClimb) {
//            physics->acceleration.y = -1.0f * playerSpeed;
            physics->velocity.y = -1.0f * climbSpeed * deltaTime;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Down) && physics->isClimb) {
//            physics->acceleration.y = 1.0f * playerSpeed;
            physics->velocity.y = 1.0f * climbSpeed * deltaTime;
        }
    }

    physics->acceleration.x *= (physics->isGrounded) ? playerSpeed : 0.8f * playerSpeed;
}

void PlayerUpdate(entity_t* player, float deltaTime)
{
    Animator* animator = &player->animator;
    c_player_t* playerComponent = &player->player;
    sf::Transformable* transform = &player->transform;

    PlayerLocomotion(playerComponent, &player->physics, transform, &player->render, deltaTime);

    // TODO(Tony): Find a better way
    if (playerComponent->state != PLAYER_STATE_ATTACK) {
        if (abs(player->physics.velocity.x) <= stopMovingThreshold) {
            playerComponent->state = PLAYER_STATE_IDLE;
        } else {
            playerComponent->state = PLAYER_STATE_MOVING;
        }
    }

    switch (player->player.state) {
        case PLAYER_STATE_IDLE: {
            if (AnimGetRunningAnimName(animator) != CHAR_CLAW_IDLE) {
                Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_IDLE), true);
                AnimPlay(animator, &anim);
            }
        }
            break;
        case PLAYER_STATE_MOVING: {
            if (AnimGetRunningAnimName(animator) != CHAR_CLAW_RUN) {
                Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_RUN), true);
                AnimPlay(animator, &anim);
            }
        }
            break;
        case PLAYER_STATE_HIT: {
            Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_HURT), false);
            SoundPlay(&ResSoundBuffGet(clawHitSounds[rand() % clawHitSoundsCount]));
            AnimPlay(animator, &animation);
            playerComponent->state = PLAYER_STATE_RECOVER;
        }
            break;
        case PLAYER_STATE_RECOVER: {
            recoverTimer += deltaTime;
            if (AnimGetNormalizedTime(animator) >= 1.0f && recoverTimer >= recoverPeriod) {
                recoverTimer = 0.0f;
                playerComponent->state = PLAYER_STATE_IDLE;
            }
        }
            break;
        case PLAYER_STATE_ATTACK: {
            if (AnimGetNormalizedTime(animator) >= 1.0f) {
                playerComponent->state = PLAYER_STATE_IDLE;
            }
        }
            break;
        case PLAYER_STATE_DEATH: {
            if (AnimGetRunningAnimName(animator) != CHAR_CLAW_DEATH) {
                Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_DEATH), false);
                SoundPlay(&ResSoundBuffGet(clawHitSounds[rand() % clawHitSoundsCount]));
                AnimPlay(animator, &animation);
            }
        }
            break;
        case PLAYER_STATE_JUMP: {
            if (AnimGetRunningAnimName(animator) != CHAR_CLAW_JUMP) {
                Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_JUMP), false);
                AnimPlay(animator, &animation);
            }
        }
            break;
        case PLAYER_STATE_FALLING: {
            if (AnimGetRunningAnimName(animator) != CHAR_CLAW_FALLING) {
                Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_FALLING));
                AnimPlay(animator, &animation);
            }
        }
            break;
        case Player_STATE_LANDING: {
            playerComponent->state = PLAYER_STATE_IDLE;
        }
            break;
    }
}

void PlayerStateAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                       ECS* ecs, float deltaTime)
{
    attackTimer += deltaTime;
    c_damageable_t* playerDamageable = (c_damageable_t*) ECSGet(ecs, playerID, C_DAMAGEABLE);
    c_player_t* player = (c_player_t*) ECSGet(ecs, playerID, C_PLAYER);
    c_inventory_t* inventory = (c_inventory_t*) ECSGet(ecs, playerID, C_INVENTORY);
    sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, playerID, C_TRANSFORM);
    Animator* animator = (Animator*) ECSGet(ecs, playerID, C_ANIMATOR);

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        && attackTimer >= attackPeriod) {
        for (auto& eID: entityIDs) {
            c_collider_t* enemyCollider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);

            // TODO(Tony): Lerp collider size for smoother effect
            if (CheckCollision(playerDamageable->swordCollider, *enemyCollider, nullptr)) {
                CombatDamage(eID, ecs, swordDmg);
            }
        }

        Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_SWORD_ATTACK), false);
        AnimPlay(animator, &pistolAnimation);
        SoundPlay(&ResSoundBuffGet(WAV_CLAW_SWORDSWISH));
        player->state = PLAYER_STATE_ATTACK;
        attackTimer = 0.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && attackTimer >= attackPeriod) {
        if (inventory->ammo_pistol > 0) {
            sf::Vector2f bulletPos = transform->getTransform().transformPoint(playerDamageable->pistolOffset);
            BulletCreate(bulletPos, TransformForward(*transform), false);
            inventory->ammo_pistol--;

            Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_PISTOL_ATTACK), false);
            AnimPlay(animator, &pistolAnimation);
            player->state = PLAYER_STATE_ATTACK;
        } else {
            SoundPlay(&ResSoundBuffGet(WAV_CLAW_DRYGUNSHOT1));
        }

        attackTimer = 0.0f;
    }
#if 0
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && attackTimer >= attackPeriod) {

        sf::Vector2f direction = {};
        if (transform->getScale().x > 0) {
            direction.x = 1;
        } else if (transform->getScale().x < 0) {
            direction.x = -1;
        }
        sf::Vector2f bulletPos = transform->getTransform().transformPoint(playerDamageable->pistolOffset);
        BulletCreate(bulletPos, direction, true);
        attackTimer = 0.0f;
    }
#endif
}