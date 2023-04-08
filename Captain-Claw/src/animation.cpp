//
// Created by Zeyad on 03/28/23.
//
#include "animation.h"
#include "SFML/Graphics.hpp"

void AnimationInit(Animation* anim, sf::Texture* texture, int frames, int rows, float frameTime, bool loop) {
    anim->texture = texture;
    anim->frames = frames;
    anim->rows = rows;
    anim->frameTime = frameTime;
    anim->loop = loop;
    anim->currentFrame = 0;
    anim->elapsedTime = 0.0f;
}


// Initialize the animator (modified sprite)
void AnimatorInit(Animator& animtr, sf::Sprite& sprite)
{
    animtr.lol = &sprite;
}

// Add the modified sprite to the animation to be played
void play(Animation* anim, Animator& animtr, int row) {
    animtr.row = row;
    anim->sprites.push_back(animtr);

}

// Remove the modified sprite from the animation
void stop(Animation* anim, Animator& animtr) {

    for (auto it = anim->sprites.begin(); it != anim->sprites.end(); it++) {
        if (it->lol == animtr.lol)
        {
            anim->sprites.erase(it);
            break;
        }
    }

}

void pause(Animation* anim, Animator& animtr)
{
    for (auto it = anim->sprites.begin(); it != anim->sprites.end(); it++) {
        if (it->lol == animtr.lol) {
           it->pause = true;
           break;
        }
    }

}

void resume(Animation* anim, Animator& animtr)
{
    for (auto it = anim->sprites.begin(); it != anim->sprites.end(); it++) {
        if (it->lol == animtr.lol) {
            it->pause = false;
            break;
        }
    }

}
void inverseState(Animation* anim, Animator& animtr){
    for (auto it = anim->sprites.begin(); it != anim->sprites.end(); it++) {
        if (it->lol == animtr.lol) {
            it->pause = !(it->pause);
            break;
        }
    }
}



void update(Animation* anim, float deltaTime) {
    anim->elapsedTime += deltaTime;
    if (anim->elapsedTime >= anim->frameTime) {
        anim->elapsedTime -= anim->frameTime;
        anim->currentFrame++;
        if (anim->currentFrame >= anim->frames) {
            if (anim->loop) {
                anim->currentFrame = 0;
            } else {
                anim->currentFrame = anim->frames - 1;
            }
        }
    }
    int frameWidth = anim->texture->getSize().x /anim->frames;
    int frameHeight = anim->texture->getSize().y / anim->rows;
    for (auto sprite : anim->sprites) {
        if (!sprite.pause){
        sprite.lol->setTexture(*(anim->texture));
        sprite.lol->setTextureRect(sf::IntRect(anim->currentFrame * frameWidth, sprite.row * frameHeight, frameWidth, frameHeight));
    }
    }
}

