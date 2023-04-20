#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "pickup_system.h"

const int size = 200;
pickup_t *pickups[size];

void PickupUpdate(Player &captain_claw) {
    for (int i = 0; i < size; i++) {
        if (pickups[i] == nullptr) {
            continue;
        }
        if (!captain_claw.sprite.getGlobalBounds().intersects(pickups[i]->gfx.getGlobalBounds())) {
            continue;
        }
        switch (pickups[i]->type) {
            case PICKUP_GOLD:
                PickupAddGold(captain_claw, *pickups[i]);
                break;
            case PICKUP_HEALTH:
                PickupAddHealth(captain_claw, *pickups[i]);
                break;
            case PICKUP_AMMO:
                PickupAddAmmo(captain_claw, *pickups[i]);
                break;
            case PICKUP_LIVE:
                PickupAddLives(captain_claw, *pickups[i]);
                break;
        }
        pickups[i] = nullptr;
        //TOD(TONY):REMOVE FROM SCENE

    }
}

void PickupAddGold(Player &captain_claw, pickup_t &gold) {
    captain_claw.gold_counter += gold.value;
    gold.isEnabled = false;
}

void PickupAddHealth(Player &captain_claw, pickup_t &health) {
    captain_claw.health += health.value;
    health.isEnabled = false;
}

void PickupAddAmmo(Player &captain_claw, pickup_t &ammo) {
    captain_claw.ammo_pistol += ammo.value;
    ammo.isEnabled = false;
}

void PickupAddLives(Player &captain_claw, pickup_t &live) {
    captain_claw.lives += live.value;
    live.isEnabled = false;
}