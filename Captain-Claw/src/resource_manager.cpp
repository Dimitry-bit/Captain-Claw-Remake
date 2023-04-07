#include "SFML/Graphics.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include "resource_manager.h"

using std::string;

const string parentDir("../resources/");
const string fontDir(parentDir + "fonts/");
const string imageDir(parentDir + "images/");
const string textureDir(parentDir + "textures/");
const string soundDir(parentDir + "sounds/");

const sf::Font defaultFont = sf::Font();
const sf::Texture defaultTexture = sf::Texture();
const sf::Image defaultImage = sf::Image();
const sf::SoundBuffer defaultSoundBuffer = sf::SoundBuffer();

std::map<std::string, sf::Font*> fontTBL;
std::map<std::string, sf::Image*> imageTBL;
std::map<std::string, sf::Texture*> textureTBL;
std::map<std::string, sf::SoundBuffer*> soundTBL;

bool ResFontLoadFromFile(const char* identifier)
{
    sf::Font* font = new sf::Font();
    const string path(fontDir + identifier);
    if (!font->loadFromFile(path)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete font;
        return false;
    }

    delete fontTBL[identifier];
    fontTBL[identifier] = font;
    printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
    return true;
}

bool ResImageLoadFromFile(const char* identifier)
{
    sf::Image* image = new sf::Image();
    const string path(imageDir + identifier);
    if (!image->loadFromFile(path)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete image;
        return false;
    }

    delete imageTBL[identifier];
    imageTBL[identifier] = image;
    printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
    return true;

}

bool ResTextureLoadFromFile(const char* identifier, const sf::IntRect& area)
{
    sf::Texture* texture = new sf::Texture();
    const string path(textureDir + identifier);
    if (!texture->loadFromFile(path, area)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete texture;
        return false;
    }

    delete textureTBL[identifier];
    textureTBL[identifier] = texture;
    printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
    return true;
}

bool ResSoundBuffLoadFromFile(const char* identifier)
{
    sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
    const string path(soundDir + identifier);
    if (!soundBuffer->loadFromFile(path)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete soundBuffer;
        return false;
    }

    delete soundTBL[identifier];
    soundTBL[identifier] = soundBuffer;
    printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
    return true;
}

const sf::Font& ResFontGet(const char* identifier)
{
    if (!fontTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultFont;
    }

    sf::Font* font = (sf::Font*) fontTBL.at(identifier);
    if (font == nullptr) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultFont;
    }

    return *font;
}

const sf::Image& ResImageGet(const char* identifier)
{
    if (!imageTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultImage;
    }

    sf::Image* image = (sf::Image*) imageTBL.at(identifier);
    if (image == nullptr) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultImage;
    }

    return *image;
}

const sf::Texture& ResTextureGet(const char* identifier)
{
    if (!textureTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultTexture;
    }

    sf::Texture* texture = (sf::Texture*) textureTBL.at(identifier);
    if (texture == nullptr) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultTexture;
    }

    return *texture;
}

const sf::SoundBuffer& ResSoundBuffGet(const char* identifier)
{
    if (!soundTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultSoundBuffer;
    }

    sf::SoundBuffer* soundBuff = (sf::SoundBuffer*) soundTBL.at(identifier);
    if (soundBuff == nullptr) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return defaultSoundBuffer;
    }

    return *soundBuff;
}

void ResFontUnload(const char* identifier, bool erase)
{
    if (!fontTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, unloading failed.", identifier);
        return;
    }

    delete fontTBL.at(identifier);
    if (erase) {
        fontTBL.erase(identifier);
    }

    printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResImageUnload(const char* identifier, bool erase)
{
    if (!imageTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, unloading failed.", identifier);
        return;
    }

    delete imageTBL.at(identifier);
    if (erase) {
        imageTBL.erase(identifier);
    }

    printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResTextureUnload(const char* identifier, bool erase)
{
    if (!textureTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, unloading failed.", identifier);
        return;
    }

    delete textureTBL.at(identifier);
    if (erase) {
        textureTBL.erase(identifier);
    }

    printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResSoundBuffUnload(const char* identifier, bool erase)
{
    if (!soundTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, unloading failed.", identifier);
        return;
    }

    delete soundTBL.at(identifier);
    if (erase) {
        soundTBL.erase(identifier);
    }

    printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResUnloadAll()
{
    for (auto& asset: fontTBL) {
        ResFontUnload(asset.first.c_str(), false);
    }
    fontTBL.clear();

    for (auto& asset: imageTBL) {
        ResImageUnload(asset.first.c_str(), false);
    }
    imageTBL.clear();

    for (auto& asset: textureTBL) {
        ResTextureUnload(asset.first.c_str(), false);
    }
    textureTBL.clear();

    for (auto& asset: soundTBL) {
        ResSoundBuffUnload(asset.first.c_str(), false);
    }
    soundTBL.clear();
}