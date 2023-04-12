#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
struct player
{
    int gold_counter =0 ;
    int health = 100 ;
    int ammo_pistol = 10 ;
    int lives=6;
}captain_claw;
player gold_counting_coins(player &captain_claw,const sf::Sprite coins[50],const sf::Sprite claw);
player gold_counting_dragons(player &captain_claw ,const sf::Sprite dragons[50],const sf::Sprite claw);
player gold_counting_golds_bars(player &captain_claw ,const sf::Sprite gold_bar[50],const sf::Sprite claw);
player gold_counting_cross(player &captain_claw ,const sf::Sprite cross[50],const sf::Sprite claw);
player gold_counting_cups(player &captain_claw,const sf:: Sprite cups[50],const sf::Sprite claw);
player gold_counting_rings(player &captain_claw,const sf::Sprite rings[50],const sf::Sprite claw);

// The previous Functions For gold counting

player health_counting (player &captain_claw,const sf::Sprite sword[50],const sf::Sprite bullets[50],const sf::Sprite claw);
player health_raise_1(player &captain_claw,const sf::Sprite medicine_potion_1[15],const sf::Sprite claw);
player health_raise_2(player &captain_claw,const sf::Sprite medicine_potion_2[15],const sf::Sprite claw);
player health_raise_3(player &captain_claw,const sf::Sprite medicine_potion_3[15],const sf::Sprite claw);

// The previous Functions For Health

player ammo(player &captain_claw);
player ammo_raise_1(player &captain_claw,const sf::Sprite pistol_shot[15],const sf::Sprite claw);
player ammo_raise_2(player &captain_claw, const sf::Sprite pistol_shot_bag[10],const sf::Sprite claw);
player ammo_raise_3(player &captain_claw, const sf::Sprite pistol_death_bag[5], const sf::Sprite claw);

// The previous Functions For Ammo





