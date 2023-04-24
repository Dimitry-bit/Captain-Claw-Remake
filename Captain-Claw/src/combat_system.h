#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "animation.h"
#include "ecs.h"

using namespace sf;

void CombatSystemUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                        ECS* ecs, float deltaTime);
void CombatDamage(unsigned long long eID, ECS* ecs, int damage);

void BulletCreate(const Vector2f& position, const Vector2f& direction, bool isEnemy);