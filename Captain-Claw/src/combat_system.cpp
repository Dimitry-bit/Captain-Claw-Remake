#include "combat_system.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

void CombatClawMelee(Player enemy,Player claw){

    if (claw.sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds())){

        enemy.health-=10;

    }

}