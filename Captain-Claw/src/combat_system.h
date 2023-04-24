#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "animation.h"
#include "ecs.h"

using namespace sf;

void CombatAttack(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs,
                  ECS* ecs, float deltaTime);