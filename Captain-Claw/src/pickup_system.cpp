#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "pickup_system.h"
player gold_counting_coins(player captain_claw,sf::Sprite coins[50],sf::Sprite claw)
{
    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(coins[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=100;
        }
    }
    return captain_claw;
}
player gold_counting_dragons(player captain_claw , sf::Sprite dragons[50], sf::Sprite claw)
{

    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(dragons[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=10000;
        }
    }
    return captain_claw;
}
player gold_counting_golds_bars(player captain_claw , sf::Sprite gold_bar[50],sf::Sprite claw)
{
    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(gold_bar[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=500;
        }
    }
    return captain_claw;
}
player gold_counting_cross(player captain_claw , sf::Sprite cross[50],sf::Sprite claw)
{

    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(cross[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=5000;
        }
    }
    return captain_claw;
}
player gold_counting_cups(player captain_claw, sf:: Sprite cups[50],sf::Sprite claw)
{
    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(cups[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=2500;
        }
    }
    return captain_claw;
}
player gold_counting_rings(player captain_claw, sf::Sprite rings[50],sf::Sprite claw)
{

    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(rings[i].getGlobalBounds()))
        {
            captain_claw.gold_counter+=1500;
        }
    }
    return captain_claw;
}