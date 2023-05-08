#include <iostream>
#include "scene_manager.h"
#include "entity.h"

void PlatformMovingVertical(c_platform_t *platform,float deltaTime,sf::Transformable *transform);
void PlatformMovingHorizontal(c_platform_t *platform,float deltaTime,sf::Transformable *transform)