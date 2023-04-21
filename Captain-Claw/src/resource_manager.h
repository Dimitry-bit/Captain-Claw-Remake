#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "spritesheet_parser.h"

enum asset_tags_t {
    ASSET_TAG_NONE = 0,
    ASSET_TAG_ANY = ~0,
    ASSET_TAG_TILE = 1,
    ASSET_TAG_OBJ = 1 << 1,
    ASSET_TAG_TREASURE = 1 << 2,
    ASSET_TAG_PICKUP = 1 << 3,
    ASSET_TAG_ANIMATION = 1 << 4,
    ASSET_TAG_EYECANDY = 1 << 5,
};

enum asset_types_t {
    ASSET_NONE,
    ASSET_FONT,
    ASSET_IMAGE,
    ASSET_TEXTURE,
    ASSET_SOUNDBUFFER,
    ASSET_SPRITESHEET,
};

struct asset_header_t {
    string fileName;
    string fileExtension;
    string id;
};

struct asset_slot_t {
    asset_header_t header;
    asset_types_t type;
    int assetTags;
    union {
        void* data;
        sf::Font* font;
        sf::Image* image;
        sf::Texture* texture;
        sf::SoundBuffer* soundBuffer;
        spriteSheet_t* spriteSheet;
    };
};

void AssetPopType();
void AssetPushType(asset_types_t type);

bool ResLoadFromFile(const char* identifier, int tags = 0);
bool ResTextureLoadFromSpriteSheet(const char* identifier, int tags = 0);
bool ResTextureLoadFromImage(const sf::Image& image, const char* identifier, const sf::IntRect& area = sf::IntRect());

void ResFontUnload(const char* identifier, bool erase = true);
void ResImageUnload(const char* identifier, bool erase = true);
void ResTextureUnload(const char* identifier, bool erase = true);
void ResSoundBufferUnload(const char* identifier, bool erase = true);
void ResSpriteSheetUnload(const char* identifier, bool erase = true);
void ResTextureSpriteSheetUnload(const spriteSheet_t& spriteSheet);

void ResUnloadAll();

const sf::Font& ResFontGet(const char* identifier);
const sf::Image& ResImageGet(const char* identifier);
const sf::Texture& ResTextureGet(const char* identifier);
const sf::SoundBuffer& ResSoundBuffGet(const char* identifier);
const spriteSheet_t& ResSpriteSheetGet(const char* identifier);