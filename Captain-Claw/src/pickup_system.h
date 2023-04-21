#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

struct Player
{
    sf::Sprite sprite;
    sf::RectangleShape swordCollider;
    int gold_counter =0 ;
    int health = 100 ;
    int ammo_pistol = 10 ;
    int lives=6;
};
enum pickup_type
{
    PICKUP_GOLD=0,
    PICKUP_HEALTH,
    PICKUP_AMMO,
    PICKUP_LIVE
};
struct pickup_t
{
    sf::Sprite gfx ;
    int value ;
    bool isEnabled ;
    pickup_type type ;
};

void PickupAddGold(Player &captain_claw, pickup_t& gold);
void PickupAddHealth(Player &captain_claw, pickup_t & health);
void PickupAddAmmo(Player &captain_claw, pickup_t & ammo);
void PickupAddLives(Player &captain_claw, pickup_t & live);
void PickupUpdate (Player &captain_claw);