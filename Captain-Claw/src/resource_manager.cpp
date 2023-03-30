#include <iostream>
#include <cstring>
#include "SFML/Graphics.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include "resource_manager.h"

using std::string;

#define HASH_TABLE_CAPACITY 50

struct hash_entry_t {
	char* key = nullptr;
	void* data = nullptr;
	hash_entry_t* next = nullptr;
};

hash_entry_t* fontTBL[HASH_TABLE_CAPACITY];
hash_entry_t* imageTBL[HASH_TABLE_CAPACITY];
hash_entry_t* textureTBL[HASH_TABLE_CAPACITY];
hash_entry_t* soundTBL[HASH_TABLE_CAPACITY];

const sf::Font defaultFont = sf::Font();
const sf::Texture defaultTexture = sf::Texture();
const sf::Image defaultImage = sf::Image();
const sf::SoundBuffer defaultSoundBuffer = sf::SoundBuffer();

size_t Hash(const char* key);
hash_entry_t* HashTableAllocPair(const char* key, void* data);
void HashTableDeallocPair(hash_entry_t& entry);
void HashTableInsert(hash_entry_t** hashTBL, const char* key, void* data);
void* HashTableRemove(hash_entry_t** hashTBL, const char* key);
void* HashTableLookup(hash_entry_t** hashTBL, const char* key);

bool ResFontLoadFromFile(const char* identifier)
{
	sf::Font* font = new sf::Font();
	string path("../resources/fonts/");
	path.append(identifier);
	if (!font->loadFromFile(path)) {
		printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
		delete font;
		return false;
	}

	HashTableInsert(fontTBL, identifier, font);
	printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
	return true;
}

bool ResImageLoadFromFile(const char* identifier)
{
	sf::Image* image = new sf::Image();
	string path("../resources/images/");
	path.append(identifier);
	if (!image->loadFromFile(path)) {
		printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
		delete image;
		return false;
	}

	HashTableInsert(imageTBL, identifier, image);
	printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
	return true;

}

bool ResTextureLoadFromFile(const char* identifier, const sf::IntRect& area)
{
	sf::Texture* texture = new sf::Texture();
	string path("../resources/textures/");
	path.append(identifier);
	if (!texture->loadFromFile(path, area)) {
		printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
		delete texture;
		return false;
	}

	HashTableInsert(textureTBL, identifier, texture);
	printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
	return true;
}

bool ResSoundBuffLoadFromFile(const char* identifier)
{
	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	string path("../resources/sounds/");
	path.append(identifier);
	if (!soundBuffer->loadFromFile(path)) {
		printf("[ERROR][Resource]: Unable to load %s.\n", path.c_str());
		delete soundBuffer;
		return false;
	}

	HashTableInsert(soundTBL, identifier, soundBuffer);
	printf("[INFO][Resources]: %s loaded successfully.\n", identifier);
	return true;
}

const sf::Font& ResFontGet(const char* identifier)
{
	sf::Font* font = (sf::Font*) HashTableLookup(fontTBL, identifier);
	if (font == nullptr) {
		printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
		return defaultFont;
	}

	return *font;
}

const sf::Image& ResImageGet(const char* identifier)
{
	sf::Image* image = (sf::Image*) HashTableLookup(imageTBL, identifier);
	if (image == nullptr) {
		printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
		return defaultImage;
	}

	return *image;
}

const sf::Texture& ResTextureGet(const char* identifier)
{
	sf::Texture* texture = (sf::Texture*) HashTableLookup(textureTBL, identifier);
	if (texture == nullptr) {
		printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
		return defaultTexture;
	}

	return *texture;
}

const sf::SoundBuffer& ResSoundBuffGet(const char* identifier)
{
	sf::SoundBuffer* soundBuff = (sf::SoundBuffer*) HashTableLookup(soundTBL, identifier);
	if (soundBuff == nullptr) {
		printf("[ERROR][Resources]: %s is not loaded, returning default.\n", identifier);
		return defaultSoundBuffer;
	}

	return *soundBuff;
}

void ResFontUnload(const char* identifier)
{
	sf::Font* data = (sf::Font*) HashTableRemove(fontTBL, identifier);
	delete data;
	printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResImageUnload(const char* identifier)
{
	sf::Image* data = (sf::Image*) HashTableRemove(imageTBL, identifier);
	delete data;
	printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResTextureUnload(const char* identifier)
{
	sf::Texture* data = (sf::Texture*) HashTableRemove(textureTBL, identifier);
	delete data;
	printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

void ResSoundBuffUnload(const char* identifier)
{
	sf::SoundBuffer* data = (sf::SoundBuffer*) HashTableRemove(soundTBL, identifier);
	delete data;
	printf("[INFO][Resources]: %s unloaded successfully.\n", identifier);
}

size_t Hash(const char* key)
{
	size_t value = 0;
	size_t keyLen = strlen(key);

	for (size_t i = 0; i < keyLen; ++i) {
		value = value * 37 + key[i];
	}

	value %= HASH_TABLE_CAPACITY;
	return value;
}

void HashTableInsert(hash_entry_t** hashTBL, const char* key, void* data)
{
	size_t slot = Hash(key);
	hash_entry_t* entry = hashTBL[slot];

	if (entry == nullptr) {
		hashTBL[slot] = HashTableAllocPair(key, data);
		return;
	}

	hash_entry_t* prev;

	while (entry != nullptr) {
		if (strcmp(entry->key, key) == 0) {
			// TODO(Dimitry): Free old data
			entry->data = data;
		}

		prev = entry;
		entry = prev->next;
	}

	prev->next = HashTableAllocPair(key, data);
}

void* HashTableRemove(hash_entry_t** hashTBL, const char* key)
{
	size_t slot = Hash(key);
	hash_entry_t* entry = hashTBL[slot];
	void* data = nullptr;

	if (entry == nullptr) {
		return data;
	}

	hash_entry_t* prev;

	while (entry != nullptr) {
		if (strcmp(entry->key, key) == 0) {
			if (prev != nullptr) {
				prev->next = entry->next;
			}

			data = entry->data;
			entry->data = nullptr;
			delete entry->key;
			delete entry;
			return data;
		}

		prev = entry;
		entry = entry->next;
	}

	return data;
}

void* HashTableLookup(hash_entry_t** hashTBL, const char* key)
{
	size_t slot = Hash(key);
	hash_entry_t* entry = hashTBL[slot];

	if (entry == nullptr) {
		return nullptr;
	}

	while (entry != nullptr) {
		if (strcmp(entry->key, key) == 0) {
			return entry->data;
		}

		entry = entry->next;
	}

	return nullptr;
}

hash_entry_t* HashTableAllocPair(const char* key, void* data)
{
	hash_entry_t* entry = new hash_entry_t;
	size_t keySize = strlen(key) + 1;
	entry->key = new char[keySize];
	entry->data = data;
	strncpy(entry->key, key, keySize);
	return entry;
}