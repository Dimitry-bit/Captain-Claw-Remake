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

Player gold_counting_coins(Player &captain_claw, const sf::Sprite coins[50], sf::Sprite claw);
Player gold_counting_dragons(Player &captain_claw , const sf::Sprite dragons[50], sf::Sprite claw);
Player gold_counting_golds_bars(Player &captain_claw , const sf::Sprite gold_bar[50], sf::Sprite claw);
Player gold_counting_cross(Player &captain_claw , const sf::Sprite cross[50], sf::Sprite claw);
Player gold_counting_cups(Player &captain_claw, const sf:: Sprite cups[50], sf::Sprite claw);
Player gold_counting_rings(Player &captain_claw, const sf::Sprite rings[50], sf::Sprite claw);

// The previous Functions For gold counting

Player health_counting (Player &captain_claw, const sf::Sprite sword[50], const sf::Sprite bullets[50], sf::Sprite claw);
Player health_raise_1(Player &captain_claw, const sf::Sprite medicine_potion_1[15], sf::Sprite claw);
Player health_raise_2(Player &captain_claw, const sf::Sprite medicine_potion_2[15], sf::Sprite claw);
Player health_raise_3(Player &captain_claw, const sf::Sprite medicine_potion_3[15], sf::Sprite claw);

// The previous Functions For Health

Player ammo(Player &captain_claw);
Player ammo_raise_1(Player &captain_claw, const sf::Sprite pistol_shot[15], sf::Sprite claw);
Player ammo_raise_2(Player &captain_claw, const sf::Sprite pistol_shot_bag[10], sf::Sprite claw);
Player ammo_raise_3(Player &captain_claw, const sf::Sprite pistol_death_bag[5], sf::Sprite claw);

// The previous Functions For Ammo

Player lives(Player &captain_claw);
Player lives(Player &captain_claw, sf::Sprite extra_live [5], sf::Sprite claw);

// The previous Function For claw lives