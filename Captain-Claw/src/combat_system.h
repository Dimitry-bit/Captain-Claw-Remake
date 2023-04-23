#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "animation.h"

using namespace sf;

struct Bullet {
    Sprite sprite;
    Vector2f direction;
    bool isActive;
};
struct Actor {
    sf::Sprite sprite;
    sf::RectangleShape swordCollider;
    int health = 100;
    int lives;
    Animator animator;
    int type;
    unsigned int soundID;
};

void CombatDamage(Actor &actor, int damage);
void CombatAttack(Actor enemy[], Actor &claw, int damage, int numberOfEnemies,float deltaTime);
void CombatDeath(Actor &actor);
void CombatDamageAnimation(Actor &damagedPlayer);
void CombatDeadAnimation(Actor &deadPlayer);
void CombatDamageSFX(Actor &actor);
void CombatDeathSFX(Actor &actor);

void BulletInit(Bullet &bullet, const Vector2f &vector, const Vector2f &direction);
void BulletCreate(const Vector2f &position, const Vector2f &direction);
void BulletsUpdate(Actor enemies[], int numberOfEnemies, float deltaTime);