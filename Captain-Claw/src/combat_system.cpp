#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "combat_system.h"
#include "vector_math.h"
#include "resource_manager.h"
#include "asset_constants.h"
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
    bool isEnemy;
};

const int bulletDmg = 50;
const float bulletSpeed = 800.0f;
const int bulletArrSize = 50;
Bullet bullets[bulletArrSize];
int bulletIndex = 0;

const float removePeriod = 5.0f;
float removeTimer;

std::unordered_set<unsigned long long> entitiesMarkedForRemoval;

void BulletInit(Bullet& bullet, const Vector2f& vector, const Vector2f& direction, bool isEnemy);
void BulletsUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                   ECS* ecs, float deltaTime);

void CombatDamage(unsigned long long eID, ECS* ecs, int damage)
{
    c_damageable_t* actor = (c_damageable_t*) ECSGet(ecs, eID, C_DAMAGEABLE);
    c_enemy_t* enemyData = nullptr;
    c_player_t* playerData = nullptr;

    if (actor->health <= 0) {
        return;
    }

    if (ECSHas(ecs, eID, C_ENEMY)) {
        enemyData = (c_enemy_t*) ECSGet(ecs, eID, C_ENEMY);
        enemyData->state = ENEMY_STATE_HIT;
    } else if (ECSHas(ecs, eID, C_PLAYER)) {
        playerData = (c_player_t*) ECSGet(ecs, eID, C_PLAYER);
        playerData->state = PLAYER_STATE_HIT;
    }

    actor->health -= damage;
    if (actor->health <= 0) {
        actor->lives -= 1;

        if (actor->lives <= 0) {
            if (enemyData) {
                enemyData->state = ENEMY_STATE_DEATH;
                entitiesMarkedForRemoval.insert(eID);
            } else if (playerData) {
                playerData->state = PLAYER_STATE_DEATH;
            }
        } else {
            actor->health = 100;
        }
    }
}

void CombatSystemUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                        ECS* ecs, float deltaTime)
{
    removeTimer += deltaTime;

    if (removeTimer >= removePeriod) {
        removeTimer = 0;
        for (auto& eID: entitiesMarkedForRemoval) {
            ECSEntityDealloc(ecs, eID);
        }
        entitiesMarkedForRemoval.clear();
    }

    BulletsUpdate(playerID, entityIDs, ecs, deltaTime);
}

void BulletInit(Bullet& bullet, const Vector2f& vector, const Vector2f& direction, bool isEnemy)
{
    const Texture& texture = ResTextureGet(BULLET_GFX);
    bullet.sprite.setTexture(texture);
    bullet.sprite.setPosition(vector);
    bullet.direction = VectorNormalize(direction);
    bullet.isActive = true;
    bullet.isEnemy = isEnemy;
}

void BulletCreate(const Vector2f& position, const Vector2f& direction, bool isEnemy)
{
    BulletInit(bullets[bulletIndex], position, direction, isEnemy);
    bulletIndex++;
    bulletIndex %= bulletArrSize;
    SoundPlay(&ResSoundBuffGet(WAV_CLAW_GUNSHOT));
}

void BulletsUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                   ECS* ecs, float deltaTime)
{
    Vector2f getCenter = rWindow->getView().getCenter();
    Vector2f getSize = rWindow->getView().getSize();

    entity_t* player = ecs->entitiesRegistry[playerID];
    for (int i = 0; i < bulletArrSize; ++i) {
        if (!bullets[i].isActive)
            continue;

        bullets[i].sprite.move(bullets[i].direction * deltaTime * bulletSpeed);

        if (bullets[i].isEnemy) {
            Vector2f bulletPos = bullets[i].sprite.getPosition();
            if (bullets[i].sprite.getGlobalBounds().intersects(player->render.sprite.getGlobalBounds())) {
                CombatDamage(playerID, ecs, bulletDmg);
                bullets[i].isActive = false;
            } else if ((bulletPos.x > getSize.x / 2.0f + getCenter.x) ||
                (bulletPos.x < getCenter.x - getSize.x / 2.0f) ||
                (bulletPos.y > getSize.y / 2.0f + getCenter.y) ||
                (bulletPos.y < getCenter.y - getSize.y / 2.0f)) {
                bullets[i].isActive = false;
            }
        }

        rWindow->draw(bullets[i].sprite);
    }

    for (auto& eID: entityIDs) {
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

        for (int i = 0; i < bulletArrSize; ++i) {
            if (!bullets[i].isActive || bullets[i].isEnemy) {
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