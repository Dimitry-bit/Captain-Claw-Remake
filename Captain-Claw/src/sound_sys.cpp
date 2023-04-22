#include <unordered_map>

#include "sound_sys.h"

struct sound_component_t {
    unsigned int handle;
    bool isLoop;
    sf::Sound self;
};

std::unordered_map<unsigned int, sound_component_t> soundObjects;
std::vector<unsigned int> soundObjectsToRemove;
unsigned int soundHandleIndex = 1;

bool IsValidSoundObj(unsigned int handle)
{
    if (!soundObjects.count(handle)) {
        printf("[ERROR][SoundSys]: Sound object ID(%d) is not a live.\n", handle);
        return false;
    }

    return true;
}

unsigned int SoundPlay(const sf::SoundBuffer* sound, bool isLoop)
{
    if (!sound) {
        return 0;
    }

    // TODO(Tony): Should wrap ?
    unsigned int handle = soundHandleIndex++;
    soundObjects[handle] = {
        .handle = handle,
        .isLoop = isLoop,
    };

    sound_component_t& soundComponent = soundObjects.at(handle);
    soundComponent.self.setBuffer(*sound);
    soundComponent.self.play();

    return handle;
}

void SoundPause(unsigned int handle)
{
    soundObjects.at(handle).self.pause();
}

void SoundResume(unsigned int handle)
{
    if (!IsValidSoundObj(handle)) {
        return;
    }

    soundObjects.at(handle).self.play();
}

void SoundStop(unsigned int handle)
{
    if (!IsValidSoundObj(handle)) {
        return;
    }

    soundObjects.at(handle).self.stop();
    soundObjectsToRemove.push_back(handle);
}

void SoundRestart(unsigned int handle)
{
    if (!IsValidSoundObj(handle)) {
        return;
    }

    soundObjects.at(handle).self.setPlayingOffset(sf::Time::Zero);
    soundObjects.at(handle).self.play();
}

void SoundSetPosition(unsigned int handle, float x, float y, float z)
{
    if (!IsValidSoundObj(handle)) {
        return;
    }

    soundObjects.at(handle).self.setPosition(x, y, z);
}

void SoundSetPosition(unsigned int handle, const sf::Vector3f& pos)
{
    SoundSetPosition(handle, pos.x, pos.y, pos.z);
}

sf::Sound::Status SoundGetStatus(unsigned int handle)
{
    if (!IsValidSoundObj(handle)) {
        return sf::Sound::Stopped;
    }

    return soundObjects.at(handle).self.getStatus();
}

sf::Sound* SoundGetSoundObj(unsigned int handle)
{
    if (!IsValidSoundObj(handle)) {
        return nullptr;
    }

    return &soundObjects.at(handle).self;
}

void SoundSystemUpdate()
{
    for (auto& soundObj: soundObjects) {
        if (soundObj.second.self.getStatus() != sf::SoundSource::Stopped)
            continue;

        if (soundObj.second.isLoop) {
            soundObj.second.self.play();
        } else {
            soundObjectsToRemove.push_back(soundObj.first);
        }
    }

    for (auto& soundHandle: soundObjectsToRemove) {
        soundObjects.erase(soundHandle);
    }
    soundObjectsToRemove.clear();
}