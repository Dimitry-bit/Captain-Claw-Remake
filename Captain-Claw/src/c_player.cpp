#include <cmath>
#include <unordered_map>

#include "c_player.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "sound_sys.h"
#include "combat_system.h"
#include "c_collider.h"
#include "transform_utils.h"
#include "renderer.h"
#include "vector_math.h"
#include "scene_manager.h"

const char* clawHitSounds[] = {
    WAV_CLAW_HIT1,
    WAV_CLAW_HIT2,
    WAV_CLAW_HIT3,
    WAV_CLAW_HITATHIH,
    WAV_CLAW_HITATLOW
};
const int clawHitSoundsCount = 5;

const char* clawWalkSounds[] = {
    WAV_CLAW_RIGHTFOOT1,
    WAV_CLAW_LEFTFOOT1
};
const int clawWalkSoundsCount = 2;
float walkSoundTimer;

const float recoverPeriod = 0.5f;
float recoverTimer = 0.0f;
const float attackPeriod = 0.5f;
float attackTimer = 0.0f;
const int swordDmg = 25;

const float playerSpeed = 1300.0f;
const float stopMovingThreshold = 35.0f;

float timeInAir = 0.0f;
const float jumpImpulseTime = 0.1f;
const float jumpImpulseVel = 250.0f;
const float jumpAccel = 25.0f;
const float MAX_AIR_TIME = 0.4f;

// FIXME(Tony): hmm! This could be frame dependent
const float climbSpeed = 10000.0f;

typedef void (* state_update)(ECS*, unsigned long long, float);
typedef void (* state_invoke)(ECS*, unsigned long long);

void PlayerUpdateIdle(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateMove(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateClimbing(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateJump(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateLanding(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateFalling(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateAttack(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateRecover(ECS* ecs, unsigned long long id, float deltaTime);
void PlayerUpdateDeath(ECS* ecs, unsigned long long id, float deltaTime);

void PlayerStartIdle(ECS* ecs, unsigned long long id);
void PlayerStartMove(ECS* ecs, unsigned long long id);
void PlayerStartClimbing(ECS* ecs, unsigned long long id);
void PlayerStartAttack(ECS* ecs, unsigned long long id);
void PlayerStartHit(ECS* ecs, unsigned long long id);

void PlayerExitClimbing(ECS* ecs, unsigned long long id);
void PlayerExitAttack(ECS* ecs, unsigned long long id);

const std::unordered_map<player_state_t, state_invoke> stateOnEnter = {
    {PLAYER_STATE_IDLE, PlayerStartIdle},
    {PLAYER_STATE_MOVING, PlayerStartMove},
    {Player_STATE_CLIMBING, PlayerStartClimbing},
    {PLAYER_STATE_JUMP, nullptr},
    {Player_STATE_LANDING, nullptr},
    {PLAYER_STATE_FALLING, nullptr},
    {PLAYER_STATE_ATTACK, PlayerStartAttack},
    {PLAYER_STATE_HIT, PlayerStartHit},
    {PLAYER_STATE_RECOVER, nullptr},
    {PLAYER_STATE_DEATH, nullptr},
};

const std::unordered_map<player_state_t, state_update> stateUpdate = {
    {PLAYER_STATE_IDLE, PlayerUpdateIdle},
    {PLAYER_STATE_MOVING, PlayerUpdateMove},
    {Player_STATE_CLIMBING, PlayerUpdateClimbing},
    {PLAYER_STATE_JUMP, PlayerUpdateJump},
    {Player_STATE_LANDING, PlayerUpdateLanding},
    {PLAYER_STATE_FALLING, PlayerUpdateFalling},
    {PLAYER_STATE_ATTACK, PlayerUpdateAttack},
    {PLAYER_STATE_HIT, nullptr},
    {PLAYER_STATE_RECOVER, PlayerUpdateRecover},
    {PLAYER_STATE_DEATH, PlayerUpdateDeath},
};

const std::unordered_map<player_state_t, state_invoke> stateOnExit = {
    {PLAYER_STATE_IDLE, nullptr},
    {PLAYER_STATE_MOVING, nullptr},
    {Player_STATE_CLIMBING, PlayerExitClimbing},
    {PLAYER_STATE_JUMP, nullptr},
    {Player_STATE_LANDING, nullptr},
    {PLAYER_STATE_FALLING, nullptr},
    {PLAYER_STATE_ATTACK, PlayerExitAttack},
    {PLAYER_STATE_HIT, nullptr},
    {PLAYER_STATE_RECOVER, nullptr},
    {PLAYER_STATE_DEATH, nullptr},
};

scene_context_t* worldRef;

void PlayerFSMUpdate(ECS* ecs, unsigned long long id, c_player_t* player, float deltaTime);

void PlayerUpdate(scene_context_t* world, unsigned long long id, float deltaTime)
{
    worldRef = world;
    ECS* ecs = &world->ecs;
    sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, id, C_TRANSFORM);
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
    c_physics_t* physics = (c_physics_t*) ECSGet(ecs, id, C_PHYSICS);

    physics->acceleration = {};

    if (physics->isGrounded) {
        timeInAir = 0.0f;
    } else {
        timeInAir += deltaTime;
    }

    if (cPlayer->state == Player_STATE_CLIMBING&&physics->isClimb) {
        physics->gravityScale = 0.0f;
        physics->velocity.y = 0;
    } else {
        physics->gravityScale = 1.0f;
    }

    if (cPlayer->state == PLAYER_STATE_IDLE || cPlayer->state == PLAYER_STATE_MOVING
        || cPlayer->state == Player_STATE_CLIMBING) {
        if (cPlayer->state != Player_STATE_CLIMBING) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                physics->acceleration.x = 1.0f;

                if (cPlayer->state != Player_STATE_CLIMBING) {
                    sf::Vector2f scale = transform->getScale();
                    if (scale.x < 0.0f) {
                        transform->setScale(-1.0f * scale.x, scale.y);
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                physics->acceleration.x = -1.0f;

                if (cPlayer->state != Player_STATE_CLIMBING) {
                    sf::Vector2f scale = transform->getScale();
                    if (scale.x > 0.0f) {
                        transform->setScale(-1.0f * scale.x, scale.y);
                    }
                }
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
            physics->velocity.y = -1.0f * climbSpeed * deltaTime;
            PlayerFSMSwitch(ecs, id, Player_STATE_CLIMBING);
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Down) && physics->isClimb) {
            physics->velocity.y = 1.0f * climbSpeed * deltaTime;
            PlayerFSMSwitch(ecs, id, Player_STATE_CLIMBING);
        }
    }

    physics->acceleration.x *= (physics->isGrounded) ? playerSpeed : 0.8f * playerSpeed;

    // TODO(Tony): Find a better way
    if (abs(physics->velocity.x) <= stopMovingThreshold) {
        PlayerFSMSwitch(ecs, id, PLAYER_STATE_IDLE);
    } else {
        PlayerFSMSwitch(ecs, id, PLAYER_STATE_MOVING);
    }

    PlayerFSMUpdate(ecs, id, cPlayer, deltaTime);
}

void PlayerStartIdle(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);

    if (cPlayer->state == PLAYER_STATE_MOVING) {
        cPlayer->state = PLAYER_STATE_IDLE;
    }
}

void PlayerUpdateIdle(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetRunningAnimName(animator) != CHAR_CLAW_IDLE) {
        Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_IDLE), true);
        AnimPlay(animator, &anim);
    }
}

void PlayerStartMove(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);

    if (cPlayer->state == PLAYER_STATE_IDLE) {
        cPlayer->state = PLAYER_STATE_MOVING;
    }
}

void PlayerUpdateMove(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetRunningAnimName(animator) != CHAR_CLAW_RUN) {
        Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_RUN), true);
        AnimPlay(animator, &anim);
    }

    walkSoundTimer += deltaTime;
    if (walkSoundTimer >= 0.3f) {
        walkSoundTimer = 0.0f;
        SoundPlay(&ResSoundBuffGet(clawWalkSounds[rand() % clawWalkSoundsCount]), false);
    }
}

void PlayerStartClimbing(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (cPlayer->state == PLAYER_STATE_IDLE || cPlayer->state == PLAYER_STATE_MOVING) {
        cPlayer->state = Player_STATE_CLIMBING;

        if (AnimGetRunningAnimName(animator) != CHAR_CLAW_CLIMBING) {
            Animation anim = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_CLIMBING), true);
            AnimPlay(animator, &anim);
        }
    }
}

void PlayerUpdateClimbing(ECS* ecs, unsigned long long id, float deltaTime)
{
    c_physics_t* physics = (c_physics_t*) ECSGet(ecs, id, C_PHYSICS);
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);
    sf::Transformable* transfrom = (sf::Transformable*) ECSGet(ecs, id, C_TRANSFORM);

    if (physics->isClimb) {
        if (abs(physics->velocity.y) == 0) {
            AnimPause(animator);
        } else if (animator->state == ANIMATOR_STATE_PAUSED) {
            AnimResume(animator);
        }

        entity_t* closestTile = nullptr;
        sf::IntRect cullBox = RendererCalculateCulling(worldRef);
        for (int y = cullBox.top; y < cullBox.height; ++y) {
            for (int x = cullBox.left; x < cullBox.width; ++x) {
                entity_t* tile = SceneGetTileWithIndex(worldRef, 1, x, y);
                if (!tile) {
                    continue;
                }
                if (!closestTile) {
                    closestTile = tile;
                }

                if (tile->tile.type == TILE_CLIMBABLE) {
                    if (fabs(tile->transform.getPosition().x - transfrom->getPosition().x)
                        < fabs(closestTile->transform.getPosition().x - transfrom->getPosition().x)) {
                        closestTile = tile;
                    }
                }
            }
        }

        float disp = (closestTile->transform.getPosition().x + worldRef->tileSize / 2.0f);
        transfrom->setPosition(disp, transfrom->getPosition().y);

    } else {
        PlayerFSMSwitch(ecs, id, PLAYER_STATE_IDLE);
    }
}

void PlayerExitClimbing(ECS* ecs, unsigned long long id)
{
    c_physics_t* physics = (c_physics_t*) ECSGet(ecs, id, C_PHYSICS);

    if (!physics->isClimb) {
        c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
        cPlayer->state = PLAYER_STATE_IDLE;
    }
}

void PlayerUpdateJump(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetRunningAnimName(animator) != CHAR_CLAW_JUMP) {
        Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_JUMP), false);
        AnimPlay(animator, &animation);
    }
}

void PlayerUpdateLanding(ECS* ecs, unsigned long long id, float deltaTime)
{
    PlayerFSMSwitch(ecs, id, PLAYER_STATE_IDLE);
}

void PlayerUpdateFalling(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetRunningAnimName(animator) != CHAR_CLAW_FALLING) {
        Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_FALLING));
        AnimPlay(animator, &animation);
    }
}

void PlayerStartAttack(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);

    if (cPlayer->state == PLAYER_STATE_IDLE || cPlayer->state == PLAYER_STATE_MOVING) {
        cPlayer->state = PLAYER_STATE_ATTACK;
    }
}

void PlayerUpdateAttack(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetNormalizedTime(animator) >= 1.0f) {
        PlayerFSMSwitch(ecs, id, PLAYER_STATE_IDLE);
    }
}

void PlayerExitAttack(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetNormalizedTime(animator) >= 1.0f) {
        cPlayer->state = PLAYER_STATE_IDLE;
    }
}

void PlayerStartHit(ECS* ecs, unsigned long long id)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
    cPlayer->state = PLAYER_STATE_HIT;

    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);
    Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_HURT), false);
    SoundPlay(&ResSoundBuffGet(clawHitSounds[rand() % clawHitSoundsCount]));
    AnimPlay(animator, &animation);

    PlayerFSMSwitch(ecs, id, PLAYER_STATE_RECOVER);
}

void PlayerUpdateRecover(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    recoverTimer += deltaTime;
    if (AnimGetNormalizedTime(animator) >= 1.0f && recoverTimer >= recoverPeriod) {
        recoverTimer = 0.0f;
        PlayerFSMSwitch(ecs, id, PLAYER_STATE_IDLE);
    }
}

void PlayerUpdateDeath(ECS* ecs, unsigned long long id, float deltaTime)
{
    Animator* animator = (Animator*) ECSGet(ecs, id, C_ANIMATOR);

    if (AnimGetRunningAnimName(animator) != CHAR_CLAW_DEATH) {
        Animation animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_DEATH), false);
        SoundPlay(&ResSoundBuffGet(clawHitSounds[rand() % clawHitSoundsCount]));
        AnimPlay(animator, &animation);
    }
}

void PlayerFSMUpdate(ECS* ecs, unsigned long long id, c_player_t* player, float deltaTime)
{
    if (stateUpdate.at(player->state)) {
        stateUpdate.at(player->state)(ecs, id, deltaTime);
    }
}

void PlayerStateAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                       ECS* ecs, float deltaTime)
{
    attackTimer += deltaTime;
    c_damageable_t* playerDamageable = (c_damageable_t*) ECSGet(ecs, playerID, C_DAMAGEABLE);
    c_inventory_t* inventory = (c_inventory_t*) ECSGet(ecs, playerID, C_INVENTORY);
    sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, playerID, C_TRANSFORM);
    Animator* animator = (Animator*) ECSGet(ecs, playerID, C_ANIMATOR);
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, playerID, C_PLAYER);

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        && attackTimer >= attackPeriod) {
        PlayerFSMSwitch(ecs, playerID, PLAYER_STATE_ATTACK);

        if (cPlayer->state != PLAYER_STATE_ATTACK) {
            return;
        }

        for (auto& eID: entityIDs) {
            c_collider_t* enemyCollider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);

            // TODO(Tony): Lerp collider size for smoother effect
            if (CheckCollision(playerDamageable->swordCollider, *enemyCollider, nullptr)) {
                CombatDamage(eID, ecs, swordDmg);
            }
        }

        Animation swordAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_SWORD_ATTACK), false);
        AnimPlay(animator, &swordAnimation);
        SoundPlay(&ResSoundBuffGet(WAV_CLAW_SWORDSWISH));

        attackTimer = 0.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && attackTimer >= attackPeriod) {
        PlayerFSMSwitch(ecs, playerID, PLAYER_STATE_ATTACK);
        if (cPlayer->state != PLAYER_STATE_ATTACK) {
            return;
        }

        if (inventory->ammo_pistol > 0) {
            sf::Vector2f bulletPos = transform->getTransform().transformPoint(playerDamageable->pistolOffset);
            BulletCreate(bulletPos, TransformForward(*transform), false);
            inventory->ammo_pistol--;

            Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_PISTOL_ATTACK), false);
            AnimPlay(animator, &pistolAnimation);
        } else {
            SoundPlay(&ResSoundBuffGet(WAV_CLAW_DRYGUNSHOT1));
        }

        attackTimer = 0.0f;
    }
}

void PlayerFSMSwitch(ECS* ecs, unsigned long long id, player_state_t state)
{
    c_player_t* cPlayer = (c_player_t*) ECSGet(ecs, id, C_PLAYER);
    if (stateOnExit.at(cPlayer->state)) {
        stateOnExit.at(cPlayer->state)(ecs, id);
    }

    if (stateOnEnter.at(state)) {
        stateOnEnter.at(state)(ecs, id);
    }
}

void PlayerCameraFollow(const sf::Transformable* transform, render_context_t* renderContext, float deltaTime)
{
    const float speed = 230.0f;
    const float moveThresholdX = 5.0f;
    const float moveThresholdY = 3.0f;
    const float maxThreshold = 500.0f;
    float absDeltaX = fabs(transform->getPosition().x - renderContext->worldView.getCenter().x);
    float absDeltaY = fabs(transform->getPosition().y - renderContext->worldView.getCenter().y);

    if (absDeltaX >= maxThreshold || absDeltaY >= maxThreshold) {
        renderContext->worldView.setCenter(transform->getPosition());
    }

    if (absDeltaX >= moveThresholdX) {
        sf::Vector2f moveTemp = transform->getPosition() - renderContext->worldView.getCenter();
        moveTemp = VectorNormalize(moveTemp);
        renderContext->worldView.move(moveTemp.x * speed * deltaTime, 0.0f);
    }

    if (absDeltaY >= moveThresholdY) {
        sf::Vector2f moveTemp = transform->getPosition() - renderContext->worldView.getCenter();
        renderContext->worldView.move(0.0f, moveTemp.y * deltaTime);
    }
}