#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "spritesheet_parser.h"

void ResUnloadAll();

bool ResFontLoadFromFile(const char* identifier);
bool ResImageLoadFromFile(const char* identifier);
bool ResTextureLoadFromFile(const char* identifier, const sf::IntRect& area = sf::IntRect());
bool ResTextureLoadFromImage(const sf::Image& image, const char* identifier, const sf::IntRect& area = sf::IntRect());
bool ResSoundBuffLoadFromFile(const char* identifier);
bool ResSpriteSheetLoadFromFile(const char* identifier);
bool ResTextureLoadFromSpriteSheet(const spriteSheet_t& spriteSheet);

void ResFontUnload(const char* identifier, bool erase = true);
void ResImageUnload(const char* identifier, bool erase = true);
void ResTextureUnload(const char* identifier, bool erase = true);
void ResSoundBuffUnload(const char* identifier, bool erase = true);
void ResSpriteSheetUnload(const char* identifier, bool erase = true);
void ResTextureSpriteSheetUnload(const spriteSheet_t& spriteSheet);

const sf::Font& ResFontGet(const char* identifier);
const sf::Image& ResImageGet(const char* identifier);
const sf::Texture& ResTextureGet(const char* identifier);
const sf::SoundBuffer& ResSoundBuffGet(const char* identifier);
const spriteSheet_t* ResSpriteSheetGet(const char* identifier);