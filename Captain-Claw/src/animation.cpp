//
// Created by Zeyad on 03/28/23.
//

#include <vector>
#include <cmath>
#include "SFML/Graphics.hpp"

#include "animation.h"

std::vector<Animator*> animators;
std::vector<Animator*> animatorsToRemove;

const float animatorUpdateRate = 1.0f / 10.0f;
const float originAlignmentThresholdX = 5.0f;
const float originAlignmentThresholdY = 5.0f;

void AnimUpdate(Animator* animator, float deltaTime);

// TODO(Zeyad): Validate pointer

Animation AnimAnimationCreate(const spriteSheet_t* spriteSheet, bool isLoop, float speed)
{
    Animation animation = {
        .spriteSheet = spriteSheet,
        .speed = speed,
        .isLoop = isLoop,
    };

    return animation;
}

void AnimAnimatorInit(Animator* animator, sf::Sprite* sprite)
{
    animator->currentFrame = 0;
    animator->elapsedTime = 0;
    animator->state = ANIMATOR_STATE_STOPPED;
    animator->sprite = sprite;
}

void AnimPlay(Animator* animator, const Animation* animation)
{
    AnimStop(animator);
    animator->state = ANIMATOR_STATE_PLAYING;
    animator->elapsedTime = -1.0f; // NOTE(Tony): force update texture and texture rect
    animator->anim = *animation;
    animator->currentFrame = 0;

    animators.push_back(animator);
}

void AnimPause(Animator* animator)
{
    animator->state = ANIMATOR_STATE_PAUSED;
}

void AnimResume(Animator* animator)
{
    animator->state = ANIMATOR_STATE_PLAYING;
}

void AnimStop(Animator* animator)
{
    animator->state = ANIMATOR_STATE_STOPPED;
    for (auto it = animators.begin(); it != animators.end(); ++it) {
        if ((*it)->state == ANIMATOR_STATE_STOPPED) {
            animators.erase(it);
            break;
        }
    }
}

void AnimUpdate(Animator* animator, float deltaTime)
{
    animator->elapsedTime += deltaTime;

    // TODO(Zeyad): Division by zero
    if (animator->elapsedTime >= animatorUpdateRate / animator->anim.speed || animator->elapsedTime <= 0.0f) {
        animator->elapsedTime = 0;

        const spriteSheet_t& spriteSheet = *animator->anim.spriteSheet;

        if (animator->anim.isLoop) {
            animator->currentFrame %= spriteSheet.frameCount;
        } else if (animator->currentFrame >= spriteSheet.frameCount) {
            animatorsToRemove.push_back(animator);
        }

        sf::Sprite& sprite = *animator->sprite;
        const sf::IntRect& area = spriteSheet.frames[animator->currentFrame].area;
        const sf::Vector2f& pivot = spriteSheet.frames[animator->currentFrame].pivot;

        sprite.setTexture(spriteSheet.texture);
        sprite.setTextureRect(spriteSheet.frames[animator->currentFrame].area);

        if (fabs(sprite.getOrigin().x - pivot.x * area.width) >= originAlignmentThresholdX) {
            sprite.move(pivot.x * area.width - sprite.getOrigin().x, 0);
        }
        if (fabs(sprite.getOrigin().y - pivot.y * area.height) >= originAlignmentThresholdY) {
            sprite.move(0, pivot.y * area.height - sprite.getOrigin().y);
        }
        sprite.setOrigin(pivot.x * area.width, pivot.y * area.height);

        animator->currentFrame++;
    }
}

float AnimGetNormalizedTime(const Animator* animator)
{
    return ((float) animator->currentFrame / (float) animator->anim.spriteSheet->frameCount);
}

string AnimGetRunningAnimName(const Animator* animator)
{
    if (animator->state == ANIMATOR_STATE_STOPPED) {
        return "";
    }

    return animator->anim.spriteSheet->fileName;
}

void AnimSystemUpdate(float deltaTime)
{
    for (auto& animator: animators) {
        AnimUpdate(animator, deltaTime);
    }

    for (auto& animator: animatorsToRemove) {
        for (auto it = animators.begin(); it != animators.end(); ++it) {
            if (*it == animator) {
                animators.erase(it);
                break;
            }
        }
    }
    animatorsToRemove.clear();
}