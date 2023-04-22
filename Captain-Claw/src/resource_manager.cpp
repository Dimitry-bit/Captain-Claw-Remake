#include <regex>
#include "SFML/Graphics.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include "resource_manager.h"

using std::string;

struct asset_context_t {
    asset_types_t pushType;

    std::map<std::string, asset_slot_t> fontTBL;
    std::map<std::string, asset_slot_t> imageTBL;
    std::map<std::string, asset_slot_t> textureTBL;
    std::map<std::string, asset_slot_t> soundTBL;
    std::map<std::string, asset_slot_t> spriteSheetTBL;
};

const string parentDir("../resources/");
const string fontDir(parentDir + "fonts/");
const string imageDir(parentDir + "images/");
const string textureDir(parentDir + "textures/");
const string soundDir(parentDir + "sounds/");

const sf::Font defaultFont = sf::Font();
const sf::Texture defaultTexture = sf::Texture();
const sf::Image defaultImage = sf::Image();
const sf::SoundBuffer defaultSoundBuffer = sf::SoundBuffer();

static asset_context_t assetContext;

template <typename Type>
bool LoadFromFile(asset_slot_t& slot, const char* dir, const char* identifier);

template <typename Type>
void AssetUnload(std::map<std::string, asset_slot_t>& table, const char* identifier, bool erase);

asset_slot_t* AssetGet(std::map<std::string, asset_slot_t>& table, const char* identifier);

void AssetPopType()
{
    assetContext.pushType = ASSET_NONE;
}

void AssetPushType(asset_types_t type)
{
    assetContext.pushType = type;
}

template <typename Type>
bool LoadFromFile(asset_slot_t& slot, const char* dir, const char* identifier)
{
    Type* asset = new Type;
    string path(dir);
    path.append(identifier);
    if (!asset->loadFromFile(path)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete asset;
        return false;
    }

    slot.data = asset;
    slot.header.fileName = path;
    slot.header.fileExtension = path.substr(path.find_last_of('.'), path.length());
    slot.header.id = identifier;
    printf("[INFO][Resources]: %s loaded successfully.\n", path.c_str());
    return true;
}

bool ResSpriteSheetLoadFromFile(asset_slot_t& slot, const char* dir, const char* identifier)
{
    spriteSheet_t* spriteSheet = new spriteSheet_t;
    string path(dir);
    path.append(identifier);
    if (!SpriteSheetLoadFromFile(path.c_str(), *spriteSheet)) {
        printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
        delete spriteSheet;
        return false;
    }

    slot.spriteSheet = spriteSheet;
    slot.header.fileName = identifier;
    slot.header.fileExtension = path.substr(path.find_last_of('.'), path.length());
    slot.header.id = identifier;
    printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
    return true;
}

bool ResLoadFromFile(const char* identifier, int tags)
{
    switch (assetContext.pushType) {
        case ASSET_NONE: {
            return false;
        }
            break;
        case ASSET_FONT: {
            asset_slot_t& slot = assetContext.fontTBL[identifier];
            slot.assetTags = tags;
            slot.type = ASSET_FONT;
            return LoadFromFile<sf::Font>(slot, fontDir.c_str(), identifier);
        }
            break;
        case ASSET_IMAGE: {
            asset_slot_t& slot = assetContext.imageTBL[identifier];
            slot.assetTags = tags;
            slot.type = ASSET_IMAGE;
            return LoadFromFile<sf::Image>(slot, imageDir.c_str(), identifier);
        }
            break;
        case ASSET_TEXTURE: {
            asset_slot_t& slot = assetContext.textureTBL[identifier];
            slot.assetTags = tags;
            slot.type = ASSET_TEXTURE;
            return LoadFromFile<sf::Texture>(slot, textureDir.c_str(), identifier);
        }
            break;
        case ASSET_SOUNDBUFFER: {
            asset_slot_t& slot = assetContext.soundTBL[identifier];
            slot.assetTags = tags;
            slot.type = ASSET_SOUNDBUFFER;
            return LoadFromFile<sf::SoundBuffer>(slot, soundDir.c_str(), identifier);
        }
            break;
        case ASSET_SPRITESHEET: {
            asset_slot_t& slot = assetContext.spriteSheetTBL[identifier];
            slot.assetTags = tags;
            slot.type = ASSET_SPRITESHEET;
            return ResSpriteSheetLoadFromFile(slot, textureDir.c_str(), identifier);
        }
            break;
    }
    return false;
}

bool ResTextureLoadFromSpriteSheet(const char* identifier, int tags)
{
    if (!assetContext.spriteSheetTBL.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded.\n", identifier);
        return false;
    }

    bool status = true;

    const asset_slot_t* spriteSheetAsset = &assetContext.spriteSheetTBL.at(identifier);
    sf::Image image = spriteSheetAsset->spriteSheet->texture.copyToImage();

    for (auto& frame: spriteSheetAsset->spriteSheet->frames) {
        sf::Texture* tex = new sf::Texture;
        if (!tex->loadFromImage(image, frame.area)) {
            delete tex;
            status = false;
            continue;
        }

        std::string frameIdentifier(identifier);
        frameIdentifier = frameIdentifier.substr(0, frameIdentifier.find_last_of('.'));
        frameIdentifier.push_back('/');
        frameIdentifier.append(frame.id);

        asset_slot_t& slot = assetContext.textureTBL[frameIdentifier];

        slot.texture = tex;
        slot.header.fileName = spriteSheetAsset->header.fileName;
        slot.header.fileExtension = spriteSheetAsset->header.fileExtension;
        slot.header.id = frameIdentifier;
        frame.id = frameIdentifier;
        slot.type = ASSET_TEXTURE;
        slot.assetTags = spriteSheetAsset->assetTags | tags;
    }

    return status;
}

asset_slot_t* AssetGet(std::map<std::string, asset_slot_t>& table, const char* identifier)
{
    if (!table.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
        return nullptr;
    }

    return &table.at(identifier);
}

asset_slot_t* ResGetAssetSlot(asset_types_t type, const char* identifier)
{
    switch (type) {
        case ASSET_NONE: {
            return nullptr;
        }
            break;
        case ASSET_FONT: {
            return AssetGet(assetContext.fontTBL, identifier);
        }
            break;
        case ASSET_IMAGE: {
            return AssetGet(assetContext.imageTBL, identifier);
        }
            break;
        case ASSET_TEXTURE: {
            return AssetGet(assetContext.textureTBL, identifier);
        }
            break;
        case ASSET_SOUNDBUFFER: {
            return AssetGet(assetContext.soundTBL, identifier);
        }
            break;
        case ASSET_SPRITESHEET: {
            return AssetGet(assetContext.spriteSheetTBL, identifier);
        }
            break;
    }
    return nullptr;
}

const sf::Font& ResFontGet(const char* identifier)
{
    asset_slot_t* slot = AssetGet(assetContext.fontTBL, identifier);

    if (!slot || !slot->font) {
        return defaultFont;
    }

    return *slot->font;
}

const sf::Image& ResImageGet(const char* identifier)
{
    asset_slot_t* slot = AssetGet(assetContext.imageTBL, identifier);

    if (!slot || !slot->image) {
        return defaultImage;
    }

    return *slot->image;
}

const sf::Texture& ResTextureGet(const char* identifier)
{
    asset_slot_t* slot = AssetGet(assetContext.textureTBL, identifier);

    if (!slot || !slot->texture) {
        return defaultTexture;
    }

    return *slot->texture;
}

const sf::SoundBuffer& ResSoundBuffGet(const char* identifier)
{
    asset_slot_t* slot = AssetGet(assetContext.soundTBL, identifier);

    if (!slot || !slot->soundBuffer) {
        return defaultSoundBuffer;
    }

    return *slot->soundBuffer;
}

const spriteSheet_t& ResSpriteSheetGet(const char* identifier)
{
    asset_slot_t* slot = AssetGet(assetContext.spriteSheetTBL, identifier);
    return *slot->spriteSheet;
}

template <typename Type>
void AssetUnload(std::map<std::string, asset_slot_t>& table, const char* identifier, bool erase)
{
    if (!table.count(identifier)) {
        printf("[ERROR][Resources]: %s is not loaded, unloading failed.\n", identifier);
        return;
    }

    delete (Type*) table.at(identifier).data; // NOTE(Tony): Union
    if (erase) {
        table.erase(identifier);
    }

    printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResFontUnload(const char* identifier, bool erase)
{
    AssetUnload<sf::Font>(assetContext.fontTBL, identifier, erase);
}

void ResImageUnload(const char* identifier, bool erase)
{
    AssetUnload<sf::Image>(assetContext.imageTBL, identifier, erase);
}

void ResTextureUnload(const char* identifier, bool erase)
{
    AssetUnload<sf::Texture>(assetContext.textureTBL, identifier, erase);
}

void ResSoundBuffUnload(const char* identifier, bool erase)
{
    AssetUnload<sf::SoundBuffer>(assetContext.soundTBL, identifier, erase);
}

void ResSpriteSheetUnload(const char* identifier, bool erase)
{
    AssetUnload<spriteSheet_t>(assetContext.spriteSheetTBL, identifier, erase);
}

void ResTextureSpriteSheetUnload(const spriteSheet_t& spriteSheet)
{
    for (auto& frame: spriteSheet.frames) {
        ResTextureUnload(frame.id.c_str());
    }
}

void ResUnloadAll()
{
    for (auto& asset: assetContext.fontTBL) {
        ResFontUnload(asset.first.c_str(), false);
    }
    assetContext.fontTBL.clear();

    for (auto& asset: assetContext.imageTBL) {
        ResImageUnload(asset.first.c_str(), false);
    }
    assetContext.imageTBL.clear();

    for (auto& asset: assetContext.textureTBL) {
        ResTextureUnload(asset.first.c_str(), false);
    }
    assetContext.textureTBL.clear();

    for (auto& asset: assetContext.soundTBL) {
        ResSoundBuffUnload(asset.first.c_str(), false);
    }
    assetContext.soundTBL.clear();

    for (auto& asset: assetContext.spriteSheetTBL) {
        ResSpriteSheetUnload(asset.first.c_str(), false);
    }
    assetContext.spriteSheetTBL.clear();
}