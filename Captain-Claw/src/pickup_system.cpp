#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "pickup_system.h"
#include "animation.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "sound_sys.h"

const int size = 200;
pickup_t *pickups[size];
sf::Sprite glitter;
Animator animator;
void PickupUpdate(Player &captain_claw) {
    for (int i = 0; i < size; i++) {
        if (pickups[i] == nullptr) {
            continue;
        }
        if (!captain_claw.sprite.getGlobalBounds().intersects(pickups[i]->gfx.getGlobalBounds())) {
            continue;
        }
        switch (pickups[i]->type) {
            case PICKUP_TREASURE:
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
        //TODO(TONY):REMOVE FROM SCENE

    }
}

void PickupAddGold(Player &captain_claw, pickup_t &gold) {
    captain_claw.gold_counter += gold.value;
    gold.isEnabled = false;
    AnimAnimatorInit(&animator, &glitter);
    Animation animation_store = AnimAnimationCreate(&ResSpriteSheetGet(VFX_GLITTER), false);
    AnimPlay(&animator, &animation_store);
    glitter.setPosition(gold.gfx.getPosition());
    //TODO(TONY): ORIGIN
    //TODO(TONY): DRAW GLITTER
    const sf::SoundBuffer& sound_gold = ResSoundBuffGet(WAV_GAME_COIN );
    SoundPlay( &sound_gold );
}

void PickupAddHealth(Player &captain_claw, pickup_t &health) {
    captain_claw.health += health.value;
    health.isEnabled = false;
    const sf::SoundBuffer& sound_health = ResSoundBuffGet(WAV_GAME_FOODITEM );
    SoundPlay( &sound_health );
}

void PickupAddAmmo(Player &captain_claw, pickup_t &ammo) {
    captain_claw.ammo_pistol += ammo.value;
    ammo.isEnabled = false;
    const sf::SoundBuffer& sound_ammo = ResSoundBuffGet(WAV_GAME_AMMUNITION );
    SoundPlay( &sound_ammo );
}

void PickupAddLives(Player &captain_claw, pickup_t &live) {
    captain_claw.lives += live.value;
    live.isEnabled = false;
    const sf::SoundBuffer& sound_live = ResSoundBuffGet(WAV_GAME_EXTRALIFE );
    SoundPlay( &sound_live );
}
