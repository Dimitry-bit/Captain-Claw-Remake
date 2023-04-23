#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "combat_system.h"
#include "vector_math.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "animation.h"
#include "renderer.h"
#include "sound_sys.h"

#define ENEMY_0 0
#define ENEMY_1 1
#define CLAW 2

using namespace sf;

Bullet bullets[50]; // TODO(Remo): Pull array size in separate #define
int bulletIndex = 0;

void CombatDamage(Actor &actor, int damage) {
    actor.health -= damage;
    CombatDamageSFX(actor);
    CombatDamageAnimation(actor);
    if (actor.health <= 0) {
        actor.lives -= 1;
        if (actor.lives <= 0) {
            CombatDeath(actor);
        } else
            actor.health = 100;
    }
}

void CombatAttack(Actor enemy[], Actor &claw, int damage, int numberOfEnemies, float deltaTime) {
    BulletsUpdate(enemy, numberOfEnemies, deltaTime);
    if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))  //sword
    {
        for (int i = 0; i < numberOfEnemies; ++i) {
            if (claw.swordCollider.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds())) {
                CombatDamage(enemy[i], damage);
            }
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Num1))   //pistol
    {
        Vector2f direction = {};
        if (claw.sprite.getScale().x > 0) {
            direction.x = 1;
        } else if (claw.sprite.getScale().x < 0) {
            direction.x = -1;
        }
        BulletCreate(claw.sprite.getPosition(), direction);
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

void BulletsUpdate(Actor enemies[], int numberOfEnemies, float deltaTime) {
    Vector2f getCenter = rWindow->getView().getCenter(),
            getSize = rWindow->getView().getSize();
    for (int i = 0; i < 50; ++i) {
        if (bullets[i].isActive == false) {
            continue;
        }
        bullets[i].sprite.move(bullets[i].direction * deltaTime * 200.0f);
    }

    for (int i = 0; i < numberOfEnemies; ++i) {
        for (int j = 0; j < 50; ++j) {
            if (bullets[j].isActive == false) {
                continue;
            }
            if (enemies[i].sprite.getGlobalBounds().intersects(bullets[j].sprite.getGlobalBounds())) {
                bullets[j].isActive = false;
                CombatDamage(enemies[i], 50);
            } else if ((bullets[j].sprite.getPosition().x > getSize.x / 2.0f + getCenter.x) ||
                       (bullets[j].sprite.getPosition().x < getCenter.x - getSize.x / 2.0f) ||
                       (bullets[j].sprite.getPosition().y > getSize.y / 2.0f + getCenter.y) ||
                       (bullets[j].sprite.getPosition().y < getCenter.y - getSize.y / 2.0f)) {
                bullets[j].isActive = false;
                //out of  screen
            }
        }
    }
}

void CombatDamageAnimation(Actor &damagedPlayer) {
    Animation damagedAnimation;
    if (damagedPlayer.type == CLAW) {
        damagedAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_HURT), false, 1);
    } else if (damagedPlayer.type == ENEMY_0) {
        damagedAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_HURT_STANCES), false, 1);
    } else if (damagedPlayer.type == ENEMY_1) {
        damagedAnimation = AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_HURT_STANCES), false, 1);
    }
    AnimAnimatorInit(&damagedPlayer.animator, &damagedPlayer.sprite);
    AnimPlay(&damagedPlayer.animator, &damagedAnimation);
}

void CombatDeadAnimation(Actor &deadPlayer) {
    Animation deadAnimation;
    if (deadPlayer.type == CLAW) {
        AnimAnimationCreate(&ResSpriteSheetGet(CHAR_CLAW_DEATH), false, 1);
    }
    if (deadPlayer.type == ENEMY_0) {
        AnimAnimationCreate(&ResSpriteSheetGet(CHAR_SOLDIER_DEATH), false, 1);
    }
    if (deadPlayer.type == ENEMY_1) {
        AnimAnimationCreate(&ResSpriteSheetGet(CHAR_OFFICER_DEATH), false, 1);
    }
    AnimAnimatorInit(&deadPlayer.animator, &deadPlayer.sprite);
    AnimPlay(&deadPlayer.animator, &deadAnimation);
}

void CombatDeathSFX(Actor &actor) {
    if (actor.type == CLAW) {
        SoundPlay(&ResSoundBuffGet(WAV_CLAW_DEATH), false);
    } else if (actor.type == ENEMY_0) {
        SoundPlay(&ResSoundBuffGet(WAV_SOLDIER_DEATH), false);
    } else if (actor.type == ENEMY_1) {
        SoundPlay(&ResSoundBuffGet(WAV_OFFICER_DEATH), false);
    }
}

void CombatDeath(Actor &actor) {
    CombatDeadAnimation(actor);
    CombatDeathSFX(actor);
    //TODO(TONY): remove the actor from the scene
}

void CombatDamageSFX(Actor &actor) {
    if (actor.type == CLAW) {
        SoundPlay(&ResSoundBuffGet(WAV_CLAW_HIT1), false);
    } else if (actor.type == ENEMY_0) {
        SoundPlay(&ResSoundBuffGet(WAV_SOLDIER_HIT1), false);
    } else if (actor.type == ENEMY_1) {
        SoundPlay(&ResSoundBuffGet(WAV_OFFICER_HIT1), false);
    }
}