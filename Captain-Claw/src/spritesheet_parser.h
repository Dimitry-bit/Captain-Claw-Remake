#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

struct frameMetadata_t {
    string id;
    sf::IntRect area;
    sf::Vector2f pivot;
};

struct spriteSheet_t {
    string fileName;
    int frameCount = 0;
    sf::Texture texture;
    std::vector<frameMetadata_t> frames;
};

bool SpriteSheetLoadFromFile(const char* identifier, spriteSheet_t& spriteSheet);