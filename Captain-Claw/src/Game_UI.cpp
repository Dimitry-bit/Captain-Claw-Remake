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
    ResFontGet("JetBrainsMono-Regular.ttf");
    Treasure_UI_Text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
    Treasure_UI_Text.setCharacterSize(20);
    Treasure_UI_Text.setFillColor(sf::Color::Yellow);
    Treasure_UI_Text.setStyle(sf::Text::Bold);
    Animation TreasureAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_TREASURECHEST));
    AnimAnimatorInit(&animator_Treasure, &Treasure);
    AnimPlay(&animator_Treasure, &TreasureAnimation);
    Treasure.setScale(2, 2);
}

void UI_Treasure_Update(ECS* ecs, unsigned long long player_id)
{
    c_inventory_t* inventory = (c_inventory_t*) ECSGet(ecs, player_id, C_INVENTORY);
    Treasure.setPosition(20, 30);
    Treasure_UI_Text.setString(std::to_string(inventory->gold_counter));
    Treasure_UI_Text.setPosition(Treasure.getPosition().x + 80, Treasure.getPosition().y + 10);
    rWindow->draw(Treasure);
    rWindow->draw(Treasure_UI_Text);
}

void UI_Health_init()
{
    ResFontGet("JetBrainsMono-Regular.ttf");
    Health_UI_Text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
    Health_UI_Text.setCharacterSize(20);
    Health_UI_Text.setFillColor(sf::Color::Black);
    Health_UI_Text.setStyle(sf::Text::Bold);
    Animation HealthAnimation = AnimAnimationCreate(&ResSpriteSheetGet(UI_HEALTHHEART));
    AnimAnimatorInit(&animator_health, &health);
    AnimPlay(&animator_health, &HealthAnimation);
    health.setScale(2, 2);
}

void UI_Health_Update(ECS* ecs, unsigned long long player_id)
{
    c_damageable_t* Health = (c_damageable_t*) ECSGet(ecs, player_id, C_DAMAGEABLE);
    health.setPosition(rWindow->getSize().x - 100, 30);
    Health_UI_Text.setString(std::to_string(Health->health));
    Health_UI_Text.setPosition(health.getPosition().x + (-40), health.getPosition().y + 10);
    rWindow->draw(health);
    rWindow->draw(Health_UI_Text);
}

void UI_ammo_init()
{
    ResFontGet("JetBrainsMono-Regular.ttf");
    Ammo_UI_Text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
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
    Ammo.setPosition(rWindow->getSize().x - 50, 115);
    Ammo_UI_Text.setString(std::to_string(ammo->ammo_pistol));
    Ammo_UI_Text.setPosition(Ammo.getPosition().x + (-95), Ammo.getPosition().y - 25);
    Ammo.setOrigin(Ammo.getTextureRect().width, Ammo.getTextureRect().height);
    rWindow->draw(Ammo);
    rWindow->draw(Ammo_UI_Text);
}

void UI_lives_init()
{
    ResFontGet("JetBrainsMono-Regular.ttf");
    Lives_UI_Text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
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
    Lives.setPosition(rWindow->getSize().x - 90, 130);
    Lives_UI_Text.setPosition(Lives.getPosition().x + (-30), Lives.getPosition().y + 10);
    rWindow->draw(Lives);
    rWindow->draw(Lives_UI_Text);
}