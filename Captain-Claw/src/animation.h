//
// Created by Zeyad on 03/28/23.
//

#pragma once

#include "SFML/System.hpp"

#include "spritesheet_parser.h"

enum animator_states_t {
    ANIMATOR_STATE_STOPPED = 0,
    ANIMATOR_STATE_PLAYING,
    ANIMATOR_STATE_PAUSED,
};

struct Animation {
    const spriteSheet_t* spriteSheet;
    float speed;
    bool isLoop;
};

struct Animator {
    Animation anim;
    animator_states_t state;
    sf::Sprite* sprite;
    unsigned int currentFrame;
    float elapsedTime;
};

Animation AnimAnimationCreate(const spriteSheet_t* spriteSheet, bool isLoop = true, float speed = 1.0f);
void AnimAnimatorInit(Animator* animator, sf::Sprite* sprite);

void AnimPlay(Animator* animator, const Animation* animation);
void AnimPause(Animator* animator);
void AnimResume(Animator* animator);
void AnimStop(Animator* animator);
void AnimSystemUpdate(float deltaTime);