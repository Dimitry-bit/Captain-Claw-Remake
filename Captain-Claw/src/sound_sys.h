#pragma once

#include "SFML/Audio.hpp"

unsigned int SoundPlay(const sf::SoundBuffer* sound, bool isLoop = false);
void SoundPause(unsigned int handle);
void SoundResume(unsigned int handle);
void SoundStop(unsigned int handle);
void SoundRestart(unsigned int handle);

void SoundSetPosition(unsigned int handle, float x, float y, float z);
void SoundSetPosition(unsigned int handle, const sf::Vector3f& pos);

sf::Sound* SoundGetSoundObj(unsigned int handle);
sf::Sound::Status SoundGetStatus(unsigned int handle);

void SoundSystemUpdate();