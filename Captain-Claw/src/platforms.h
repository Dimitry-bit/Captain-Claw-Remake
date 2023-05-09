#include <iostream>
#include "ecs.h"

void PlatformUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs, ECS* ecs,
                    float deltaTime);