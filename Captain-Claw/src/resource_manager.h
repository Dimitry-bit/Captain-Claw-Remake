#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

void ResUnloadAll();

bool ResFontLoadFromFile(const char* identifier);
bool ResImageLoadFromFile(const char* identifier);
bool ResTextureLoadFromFile(const char* identifier, const sf::IntRect& area = sf::IntRect());
bool ResSoundBuffLoadFromFile(const char* identifier);

void ResFontUnload(const char* identifier, bool erase = true);
void ResImageUnload(const char* identifier, bool erase = true);
void ResTextureUnload(const char* identifier, bool erase = true);
void ResSoundBuffUnload(const char* identifier, bool erase = true);

const sf::Font& ResFontGet(const char* identifier);
const sf::Image& ResImageGet(const char* identifier);
const sf::Texture& ResTextureGet(const char* identifier);
const sf::SoundBuffer& ResSoundBuffGet(const char* identifier);