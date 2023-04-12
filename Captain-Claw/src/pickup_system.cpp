#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "pickup_system.h"
player gold_counting_coins(player &captain_claw,const sf::Sprite coins[50],const sf::Sprite claw)
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
player gold_counting_dragons(player &captain_claw ,const sf::Sprite dragons[50],const sf::Sprite claw)
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
player gold_counting_golds_bars(player &captain_claw ,const sf::Sprite gold_bar[50],const sf::Sprite claw)
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
player gold_counting_cross(player &captain_claw ,const sf::Sprite cross[50] ,const sf::Sprite claw)
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
player gold_counting_cups(player &captain_claw,const sf:: Sprite cups[50],const sf::Sprite claw)
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
player gold_counting_rings(player &captain_claw,const sf::Sprite rings[50],const sf::Sprite claw)
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
player health_counting(player &captain_claw,const sf::Sprite sword[50],const sf::Sprite bullets[50], const sf::Sprite claw)
{

    for (int i = 0; i <= 49 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(sword[i].getGlobalBounds())||claw.getGlobalBounds().intersects(bullets[i].getGlobalBounds()))
        {
            captain_claw.health=captain_claw.health-10;
        }
    }
    return captain_claw;
}
player health_raise_1(player &captain_claw,const sf::Sprite medicine_potion_1[15],const sf::Sprite claw)
{
    for (int i = 0; i <= 14 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(medicine_potion_1[i].getGlobalBounds()))
        {
            captain_claw.health+=10;
        }
    }
    return captain_claw;
}
player health_raise_2(player &captain_claw,const sf::Sprite medicine_potion_2[15], const sf::Sprite claw)
{
    for (int i = 0; i <= 14 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(medicine_potion_2[i].getGlobalBounds()))
        {
            captain_claw.health+=15;
        }
    }
    return captain_claw;
}
player health_raise_3(player &captain_claw,const sf::Sprite medicine_potion_3[15],const sf::Sprite claw)
{
    for (int i = 0; i <= 14 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(medicine_potion_3[i].getGlobalBounds()))
        {
            captain_claw.health+=25;
        }
    }
    return captain_claw;
}
player ammo(player &captain_claw)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt))
    {
        captain_claw.ammo_pistol--;
    }
    return captain_claw;
}
player ammo_raise_1(player &captain_claw,const sf::Sprite pistol_shot[15],const sf::Sprite claw)
{
    for (int i = 0; i <= 14 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(pistol_shot[i].getGlobalBounds()))
        {
            captain_claw.ammo_pistol+=5;
        }
    }
    return captain_claw;
}
player ammo_raise_2(player &captain_claw, const sf::Sprite pistol_shot_bag[10], const sf::Sprite claw)
{
    for (int i = 0; i <= 9 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(pistol_shot_bag[i].getGlobalBounds()))
        {
            captain_claw.ammo_pistol+=10;
        }
    }
    return captain_claw;
}
player ammo_raise_3(player &captain_claw,const sf::Sprite pistol_death_bag[5], const sf::Sprite claw)
{
    for (int i = 0; i <= 4 ; ++i)
    {
        if (claw.getGlobalBounds().intersects(pistol_death_bag[i].getGlobalBounds()))
        {
            captain_claw.ammo_pistol+=25;
        }
    }
    return captain_claw;
}
