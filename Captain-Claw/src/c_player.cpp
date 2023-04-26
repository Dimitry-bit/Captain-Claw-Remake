#include "c_player.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "sound_sys.h"
#include "combat_system.h"

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
const float playerSpeed = 700.0f;
const float jumpHeight = 200.0f;
const float stopMovingThreshold = 35.0f;

void PlayerUpdate(entity_t* player, float deltaTime)
{
    Animator* animator = &player->animator;
    c_player_t* playerComponent = &player->player;

    player->physics.acceleration = {};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player->physics.acceleration.x = 1.0f;

        sf::Vector2f scale = player->render.sprite.getScale();
        if (scale.x < 0.0f) {
            player->render.sprite.setScale(-1.0f * scale.x, scale.y);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player->physics.acceleration.x = -1.0f;

        sf::Vector2f scale = player->render.sprite.getScale();
        if (scale.x > 0.0f) {
            player->render.sprite.setScale(-1.0f * scale.x, scale.y);
        }
    }
    player->physics.acceleration *= playerSpeed;

    // TODO(Tony): Find a better way
    if (player->physics.velocity.x <= stopMovingThreshold && player->physics.velocity.x >= -stopMovingThreshold) {
        player->player.state = PLAYER_STATE_IDLE;
    } else {
        player->player.state = PLAYER_STATE_MOVING;
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
    entity_t* claw = ecs->entitiesRegistry[playerID];

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        && attackTimer >= attackPeriod) {
        for (auto& eID: entityIDs) {
            c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

            // TODO(Tony): Lerp collider size for smoother effect
            sf::FloatRect swordHitCollider;
            swordHitCollider.width = claw->damageable.swordCollider.width;
            swordHitCollider.height = claw->damageable.swordCollider.height;
            swordHitCollider.left = claw->render.sprite.getPosition().x;
            swordHitCollider.top = claw->render.sprite.getPosition().y + claw->damageable.swordCollider.top;
            if (claw->render.sprite.getScale().x > 0) {
                swordHitCollider.left += claw->damageable.swordCollider.left;
            } else {
                swordHitCollider.left -= (claw->damageable.swordCollider.left + claw->damageable.swordCollider.width);
            }

            if (swordHitCollider.intersects(render->sprite.getGlobalBounds())) {
                CombatDamage(eID, ecs, swordDmg);
            }
        }

        Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_SWORD_ATTACK), false);
        AnimPlay(&claw->animator, &pistolAnimation);
        SoundPlay(&ResSoundBuffGet(WAV_CLAW_SWORDSWISH));
        claw->player.state = PLAYER_STATE_ATTACK;
        attackTimer = 0.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && attackTimer >= attackPeriod) {
        if (claw->inventory.ammo_pistol > 0) {
            sf::Vector2f direction = {};
            if (claw->render.sprite.getScale().x > 0) {
                direction.x = 1;
            } else if (claw->render.sprite.getScale().x < 0) {
                direction.x = -1;
            }
            sf::Vector2f bulletPos;
            bulletPos.y = claw->damageable.pistolOffset.y + claw->render.sprite.getPosition().y;
            bulletPos.x = claw->render.sprite.getPosition().x;
            bulletPos.x += direction.x * claw->damageable.pistolOffset.x;

            BulletCreate(bulletPos, direction, false);
            claw->inventory.ammo_pistol--;

            Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_PISTOL_ATTACK), false);
            AnimPlay(&claw->animator, &pistolAnimation);
            claw->player.state = PLAYER_STATE_ATTACK;
        } else {
            SoundPlay(&ResSoundBuffGet(WAV_CLAW_DRYGUNSHOT1));
        }

        attackTimer = 0.0f;
    }
#if 0
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && attackTimer >= attackPeriod) {

        sf::Vector2f direction = {};
        if (claw->render.sprite.getScale().x > 0) {
            direction.x = 1;
        } else if (claw->render.sprite.getScale().x < 0) {
            direction.x = -1;
        }
        sf::Vector2f bulletPos;
        bulletPos.y = claw->damageable.pistolOffset.y + claw->render.sprite.getPosition().y;
        bulletPos.x = claw->render.sprite.getPosition().x;
        bulletPos.x += direction.x * claw->damageable.pistolOffset.x;
        BulletCreate(bulletPos, direction, true);
        attackTimer = 0.0f;
    }
#endif
}