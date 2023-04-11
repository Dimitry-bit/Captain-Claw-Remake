#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
struct player
{
    int gold_counter =0 ;
    int health = 100 ;
    int ammo = 10 ;
}captain_claw;
player gold_counting_coins(player captain_claw,sf::Sprite coins[50],sf::Sprite claw);
player gold_counting_dragons(player captain_claw , sf::Sprite dragons[50], sf::Sprite claw);
player gold_counting_golds_bars(player captain_claw , sf::Sprite gold_bar[50],sf::Sprite claw);
player gold_counting_cross(player captain_claw , sf::Sprite cross[50],sf::Sprite claw);
player gold_counting_cups(player captain_claw, sf:: Sprite cups[50],sf::Sprite claw);
player gold_counting_rings(player captain_claw, sf::Sprite rings[50],sf::Sprite claw);






