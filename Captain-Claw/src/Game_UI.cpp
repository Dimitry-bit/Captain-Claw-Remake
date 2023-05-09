#include <iostream>
#include "renderer.h"
#include "SFML/Graphics.hpp"
#include "Game_UI.h"
#include "resource_manager.h"
#include "animation.h"
#include  "asset_constants.h"
#include "ecs.h"
#include"entity_components.h"

Animator animator_Treasure;
Animator animator_health;
Animator animator_ammo;
Animator animator_lives;
sf::Sprite Treasure;
sf::Sprite health;
sf::Sprite Ammo;
sf::Sprite Lives;
sf::Text Treasure_UI_Text;
sf::Text Health_UI_Text;
sf::Text Ammo_UI_Text;
sf::Text Lives_UI_Text;

void UI_Treasure_init()
{
    Treasure_UI_Text.setFont(ResFontGet(FONT_MAGICDEC));
    Treasure_UI_Text.setCharacterSize(30);
    Treasure_UI_Text.setFillColor(sf::Color::Yellow);
    Treasure_UI_Text.setStyle(sf::Text::Bold);
    Treasure_UI_Text.setOutlineThickness(1.0f);
    Treasure_UI_Text.setOutlineColor(sf::Color::Black);
    Animation TreasureAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_TREASURECHEST));
    AnimAnimatorInit(&animator_Treasure, &Treasure);
    AnimPlay(&animator_Treasure, &TreasureAnimation);
    Treasure.setScale(1.8f, 1.8f);
}

void UI_Treasure_Update(ECS* ecs, unsigned long long player_id)
{
    c_inventory_t* inventory = (c_inventory_t*) ECSGet(ecs, player_id, C_INVENTORY);
    Treasure.setPosition(10, 10);
    Treasure_UI_Text.setString(std::to_string(inventory->gold_counter));
    Treasure_UI_Text.setPosition(Treasure.getPosition().x + 60, Treasure.getPosition().y + 00);
    rWindow->draw(Treasure);
    rWindow->draw(Treasure_UI_Text);
}

void UI_Health_init()
{
    Health_UI_Text.setFont(ResFontGet(FONT_MAGICDEC));
    Health_UI_Text.setCharacterSize(40);
    Health_UI_Text.setFillColor(sf::Color::Red);
    Health_UI_Text.setOutlineThickness(2.0f);
    Health_UI_Text.setOutlineColor(sf::Color::Black);
    Health_UI_Text.setStyle(sf::Text::Bold);
    Animation HealthAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_HEALTHHEART));
    AnimAnimatorInit(&animator_health, &health);
    AnimPlay(&animator_health, &HealthAnimation);
    health.setScale(2, 2);
}

void UI_Health_Update(ECS* ecs, unsigned long long player_id)
{
    c_damageable_t* Health = (c_damageable_t*) ECSGet(ecs, player_id, C_DAMAGEABLE);
    health.setPosition(rWindow->getSize().x - health.getTextureRect().width, 10);
    Health_UI_Text.setString(std::to_string(Health->health));
    Health_UI_Text.setPosition(health.getPosition().x + (-55), health.getPosition().y - 5.0f);
    rWindow->draw(health);
    rWindow->draw(Health_UI_Text);
}

void UI_ammo_init()
{
    Ammo_UI_Text.setFont(ResFontGet(FONT_MAGICDEC));
    Ammo_UI_Text.setCharacterSize(15);
    Ammo_UI_Text.setFillColor(sf::Color::Black);
    Ammo_UI_Text.setStyle(sf::Text::Bold);
    Animation ammoAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_PISTOL));
    AnimAnimatorInit(&animator_ammo, &Ammo);
    animator_ammo.adjustOrigin = false;
    AnimPlay(&animator_ammo, &ammoAnimation);
    Ammo.setScale(2, 2);
}

void UI_ammo_Update(ECS* ecs, unsigned long long player_id)
{
    c_inventory_t* ammo = (c_inventory_t*) ECSGet(ecs, player_id, C_INVENTORY);
    Ammo.setPosition(rWindow->getSize().x, 95);
    Ammo_UI_Text.setString(std::to_string(ammo->ammo_pistol));
    Ammo_UI_Text.setPosition(Ammo.getPosition().x + (-95), Ammo.getPosition().y - 25);
    Ammo.setOrigin(Ammo.getTextureRect().width, Ammo.getTextureRect().height);
    rWindow->draw(Ammo);
    rWindow->draw(Ammo_UI_Text);
}

void UI_lives_init()
{
    Lives_UI_Text.setFont(ResFontGet(FONT_MAGICDEC));
    Lives_UI_Text.setCharacterSize(20);
    Lives_UI_Text.setFillColor(sf::Color::Black);
    Lives_UI_Text.setStyle(sf::Text::Bold);
    Animation LivesAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_LIVESHEAD));
    AnimAnimatorInit(&animator_lives, &Lives);
    AnimPlay(&animator_lives, &LivesAnimation);
    Lives.setScale(2, 2);
}

void UI_lives_Update(ECS* ecs, unsigned long long player_id)
{
    c_damageable_t* lives = (c_damageable_t*) ECSGet(ecs, player_id, C_DAMAGEABLE);
    Lives_UI_Text.setString(std::to_string(lives->lives));
    Lives.setPosition(rWindow->getSize().x - Lives.getTextureRect().width, 110);
    Lives_UI_Text.setPosition(Lives.getPosition().x + (-30), Lives.getPosition().y + 5);
    rWindow->draw(Lives);
    rWindow->draw(Lives_UI_Text);
}