#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

bool ResFontLoadFromFile(const char* identifier);
bool ResImageLoadFromFile(const char* identifier);
bool ResTextureLoadFromFile(const char* identifier, const sf::IntRect& area = sf::IntRect());
bool ResSoundBuffLoadFromFile(const char* identifier);

void ResFontUnload(const char* identifier);
void ResImageUnload(const char* identifier);
void ResTextureUnload(const char* identifier);
void ResSoundBuffUnload(const char* identifier);

const sf::Font& ResFontGet(const char* identifier);
const sf::Image& ResImageGet(const char* identifier);
const sf::Texture& ResTextureGet(const char* identifier);
const sf::SoundBuffer& ResSoundBuffGet(const char* identifier);