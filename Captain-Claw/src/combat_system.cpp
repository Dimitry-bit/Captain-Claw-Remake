#include <unordered_set>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "combat_system.h"
#include "vector_math.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "renderer.h"
#include "sound_sys.h"
#include "ecs.h"
#include "entity_components.h"

using namespace sf;

#define BULLET_GFX "objects/GAME/BULLETS/CLAWBULLET1"

struct Bullet {
    Sprite sprite;
    Vector2f direction;
    bool isActive;
};

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

const char* clawHitSounds[] = {
    WAV_CLAW_HIT1,
    WAV_CLAW_HIT2,
    WAV_CLAW_HIT3,
    WAV_CLAW_HITATHIH,
    WAV_CLAW_HITATLOW
};
const int clawHitSoundsCount = 5;

const int swordDmg = 25;
const int bulletDmg = 50;
const float bulletSpeed = 800.0f;
const int bulletArrSize = 50;
Bullet bullets[bulletArrSize];
int bulletIndex = 0;

const float removePeriod = 5.0f;
float removeTimer;
const float attackPeriod = 0.5f;
float swordAttackTimer;
float pistolAttackTimer;

std::unordered_set<unsigned long long> entitiesMarkedForRemoval;

void CombatDeath(unsigned long long eID, c_enemy_t* enemyData, Animator* animator);
void CombatDamage(unsigned long long eID, ECS* ecs, int damage);

void BulletInit(Bullet& bullet, const Vector2f& vector, const Vector2f& direction);
void BulletCreate(const Vector2f& position, const Vector2f& direction);
void BulletsUpdate(std::unordered_set<unsigned long long>& entityIDs, ECS* ecs, float deltaTime);

void CombatDeath(unsigned long long eID, c_enemy_t* enemyData, Animator* animator)
{
    Animation animation;
    if (enemyData->type == ENEMY_SOLDIER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_DEATH), false, 1);
    } else if (enemyData->type == ENEMY_OFFICER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_DEATH), false, 1);
    } else {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_DEATH), false, 1);
    }

    AnimPlay(animator, &animation);
    entitiesMarkedForRemoval.insert(eID);
}

void CombatDamage(unsigned long long eID, ECS* ecs, int damage)
{
    c_enemy_t* enemyData = (c_enemy_t*) ECSGet(ecs, eID, C_ENEMY);
    Animator* animator = (Animator*) ECSGet(ecs, eID, C_ANIMATOR);
    c_damageable_t* actor = (c_damageable_t*) ECSGet(ecs, eID, C_DAMAGEABLE);

    if (actor->health <= 0) {
        return;
    }

    // FIXME(Tony): stop duplicate sounds
    Animation animation;
    int htiSoundIndex = rand();
    if (enemyData->type == ENEMY_SOLDIER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_HURT_STANCES), false, 1);
        htiSoundIndex %= soldierHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(soldierHitSounds[htiSoundIndex]), false);
    } else if (enemyData->type == ENEMY_OFFICER) {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_HURT_STANCES), false, 1);
        htiSoundIndex %= officerHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(officerHitSounds[htiSoundIndex]), false);
    } else {
        animation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_HURT), false, 1);
        htiSoundIndex %= clawHitSoundsCount;
        SoundPlay(&ResSoundBuffGet(clawHitSounds[htiSoundIndex]), false);
    }
    AnimPlay(animator, &animation);

    actor->health -= damage;
    if (actor->health <= 0) {
        actor->lives -= 1;

        if (actor->lives <= 0) {
            CombatDeath(eID, enemyData, animator);
        } else {
            actor->health = 100;
        }
    }
}

void CombatAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                  ECS* ecs, float deltaTime)
{
    swordAttackTimer += deltaTime;
    pistolAttackTimer += deltaTime;
    removeTimer += deltaTime;

    entity_t* claw = ecs->entitiesRegistry[playerID];

    if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
        && swordAttackTimer >= attackPeriod) {
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

        swordAttackTimer = 0.0f;
    } else if (Keyboard::isKeyPressed(Keyboard::Num1) && pistolAttackTimer >= attackPeriod) {
        if (claw->inventory.ammo_pistol > 0) {
            Vector2f direction = {};
            if (claw->render.sprite.getScale().x > 0) {
                direction.x = 1;
            } else if (claw->render.sprite.getScale().x < 0) {
                direction.x = -1;
            }
            sf::Vector2f bulletPos;
            bulletPos.y = claw->damageable.pistolOffset.y + claw->render.sprite.getPosition().y;
            bulletPos.x = claw->render.sprite.getPosition().x;
            bulletPos.x += direction.x * claw->damageable.pistolOffset.x;

            BulletCreate(bulletPos, direction);
            claw->inventory.ammo_pistol--;

            Animation pistolAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_PISTOL_ATTACK), false);
            AnimPlay(&claw->animator, &pistolAnimation);
        } else {
            SoundPlay(&ResSoundBuffGet(WAV_CLAW_DRYGUNSHOT1));
        }

        pistolAttackTimer = 0.0f;
    }

    if (removeTimer >= removePeriod) {
        removeTimer = 0;
        for (auto& eID: entitiesMarkedForRemoval) {
            ECSEntityDealloc(ecs, eID);
        }
        entitiesMarkedForRemoval.clear();
    }

    BulletsUpdate(entityIDs, ecs, deltaTime);
}

void BulletInit(Bullet& bullet, const Vector2f& vector, const Vector2f& direction)
{
    const Texture& texture = ResTextureGet(BULLET_GFX);
    bullet.sprite.setTexture(texture);
    bullet.sprite.setPosition(vector);
    bullet.direction = VectorNormalize(direction);
    bullet.isActive = true;
}

void BulletCreate(const Vector2f& position, const Vector2f& direction)
{
    BulletInit(bullets[bulletIndex], position, direction);
    bulletIndex++;
    bulletIndex %= bulletArrSize;
    SoundPlay(&ResSoundBuffGet(WAV_CLAW_GUNSHOT));
}

void BulletsUpdate(std::unordered_set<unsigned long long>& entityIDs, ECS* ecs, float deltaTime)
{
    Vector2f getCenter = rWindow->getView().getCenter();
    Vector2f getSize = rWindow->getView().getSize();

    for (int i = 0; i < bulletArrSize; ++i) {
        if (!bullets[i].isActive)
            continue;

        bullets[i].sprite.move(bullets[i].direction * deltaTime * bulletSpeed);
        rWindow->draw(bullets[i].sprite);
    }

    for (auto& eID: entityIDs) {
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

        for (int i = 0; i < bulletArrSize; ++i) {
            if (!bullets[i].isActive) {
                continue;
            }

            Vector2f bulletPos = bullets[i].sprite.getPosition();
            if (bullets[i].sprite.getGlobalBounds().intersects(render->sprite.getGlobalBounds())) {
                CombatDamage(eID, ecs, bulletDmg);
                bullets[i].isActive = false;
            } else if ((bulletPos.x > getSize.x / 2.0f + getCenter.x) ||
                (bulletPos.x < getCenter.x - getSize.x / 2.0f) ||
                (bulletPos.y > getSize.y / 2.0f + getCenter.y) ||
                (bulletPos.y < getCenter.y - getSize.y / 2.0f)) {
                bullets[i].isActive = false;
            }
        }
    }
}