//
// Created by dell on 26/04/2023.
//
#pragma once
#include "iostream"
#include "SFML/Graphics.hpp"
#include "pickup_system.h"
#include "resource_manager.h"
void UI_Treasure_init();
void UI_Treasure_Update(ECS*ecs,unsigned long long player_id);
void UI_Health_init();
void UI_Health_Update(ECS*ecs,unsigned long long player_id);
void UI_ammo_init();
void UI_ammo_Update(ECS*ecs,unsigned long long player_id);
void UI_lives_init();
void UI_lives_Update(ECS*ecs,unsigned long long player_id);





