#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "combat_system.h"
#include "vector_math.h"
#include "resource_manager.h"

using namespace sf;

Bullet bullets[50]; // TODO(Remo): Pull array size in separate #define
int bulletIndex = 0;

void CombatDeath(Player &actor) {
    //actor dead
}

void CombatDamage(Player &actor, int damage) {
    actor.health -= damage;
    // TODO(Remo): Add Animation
    if (actor.health <= 0) {
        CombatDeath(actor);
    }
}

void CombatAttack(Player enemy[], Player &claw, int damage, int arrSize) {
    if (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))  //sword
    {
        for (int i = 0; i < arrSize; ++i) {
            if (claw.swordCollider.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds()))
            {
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

void BulletCreate(const Vector2f &position, const Vector2f &direction){
    BulletInit(bullets[bulletIndex], position, direction);
    bulletIndex++;
    bulletIndex %= 50;
}

void BulletsUpdate(Player enemies[], int arrSize, float deltaTime) {
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
            }
//          else if () {
//              bullets[j].isActive = false;
//              //out of  screen
//            }
        }
    }
}