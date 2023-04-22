#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "combat_system.h"
#include "vector_math.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "renderer.h"

using namespace sf;

Bullet bullets[50]; // TODO(Remo): Pull array size in separate #define
int bulletIndex = 0;

void CombatDeath(Player &actor) {
    //death animation
}

void CombatDamage(Player &actor, int damage) {
    actor.health -= damage;
    CombatDamageAnimation(actor);
    if (actor.health <= 0) {
        CombatDeath(actor);
    }
}

void CombatAttack(Player enemy[], Player &claw, int damage, int arrSize) {
    if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))  //sword
    {
        for (int i = 0; i < arrSize; ++i) {
            if (claw.swordCollider.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds())) {
                CombatDamage(enemy[i], damage);
            }
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Num1))   //pistol
    {
        // TODO(Remo): BulletCreate()
    }
}

void BulletInit(Bullet &bullet, const Vector2f &vector, const Vector2f &direction) {
    const Texture &texture = ResTextureGet("objects/GAME/BULLETS/CLAWBULLET1");
    bullet.sprite.setTexture(texture);
    bullet.sprite.setPosition(vector);
    bullet.direction = VectorNormalize(direction);
    bullet.isActive = true;
}

void BulletCreate(const Vector2f &position, const Vector2f &direction) {
    BulletInit(bullets[bulletIndex], position, direction);
    bulletIndex++;
    bulletIndex %= 50;
}

void BulletsUpdate(Player enemies[], int arrSize, float deltaTime) {
    Vector2f getCenter = rWindow->getView().getCenter(),
            getSize = rWindow->getView().getSize();
    for (int i = 0; i < 50; ++i) {
        if (bullets[i].isActive == false) {
            continue;
        }
        bullets[i].sprite.move(bullets[i].direction * deltaTime * 200.0f);
    }

    for (int i = 0; i < arrSize; ++i) {
        for (int j = 0; j < 50; ++j) {
            if (bullets[j].isActive == false) {
                continue;
            }
            if (enemies[i].sprite.getGlobalBounds().intersects(bullets[j].sprite.getGlobalBounds())) {
                bullets[j].isActive = false;
                CombatDamage(enemies[i], 50);
            } else if ( (bullets[j].sprite.getPosition().x > getSize.x / 2.0f + getCenter.x) ||
                    (bullets[j].sprite.getPosition().x < getCenter.x - getSize.x / 2.0f) ||
                    (bullets[j].sprite.getPosition().y > getSize.y / 2.0f + getCenter.y) ||
                    (bullets[j].sprite.getPosition().y < getCenter.y - getSize.y / 2.0f) ) {
                bullets[j].isActive = false;
                //out of  screen
            }
        }
    }
}

void CombatDamageAnimation(Player &deadClawPlayer) {
    Animation deadClawAnimation;
    AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_HURT), false, 1);
    AnimAnimatorInit(&deadClawPlayer.animator, &deadClawPlayer.sprite);
    AnimPlay(&deadClawPlayer.animator, &deadClawAnimation);
}