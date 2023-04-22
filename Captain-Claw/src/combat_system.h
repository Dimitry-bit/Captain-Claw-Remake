#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "animation.h"

#include "pickup_system.h"

using namespace sf;

struct Bullet {
    Sprite sprite;
    Vector2f direction;
    bool isActive;
};

void CombatDamage(Player enemy[], Player &claw, int damage, int arrSize);
void CombatAttack(Player enemy[], Player &claw, int damage, int arrSize);
void CombatDeath(Player &actor);
void CombatDamageAnimation(Player &deadClawPlayer);

void BulletInit(Bullet &bullet, const Vector2f &vector, const Vector2f &direction);
void BulletCreate(const Vector2f &position, const Vector2f &direction);
void BulletsUpdate(Player enemies[], int arrSize, float deltaTime);