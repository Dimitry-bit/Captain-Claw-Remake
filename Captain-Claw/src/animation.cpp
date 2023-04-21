//
// Created by Zeyad on 03/28/23.
//
#include "animation.h"
#include "SFML/Graphics.hpp"


void AnimationInit(Animation* anim, const spriteSheet_t* spriteSheet, float frameTime, bool loop){
    anim->spriteSheet = spriteSheet;
    anim->frameTime = frameTime;
    anim->loop = loop;
    anim->currentFrame = 0;
    anim->elapsedTime = 0.0f;
}


// Initialize the animator (modified sprite)
void AnimatorInit(Animator& animtr, sf::Sprite& sprite) {
    animtr.lol = &sprite;
}

// Add the modified sprite to the animation to be played
void play(Animation* anim, Animator& animtr) {
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
        if (anim->currentFrame >= anim->spriteSheet->frameCount) {
            if (anim->loop) {
                anim->currentFrame = 0;
            } else {
                anim->currentFrame = anim->spriteSheet->frameCount - 1;
            }
        }
    }
    sf::Vector2f origin;
    for (auto sprite : anim->sprites) {

        origin.x = anim->spriteSheet->frames[anim->currentFrame].pivot.x * anim->spriteSheet->frames[anim->currentFrame].area.width;
        origin.y = anim->spriteSheet->frames[anim->currentFrame].pivot.y * anim->spriteSheet->frames[anim->currentFrame].area.width;
        sprite.lol->setOrigin(origin);
        if (!sprite.pause){
        sprite.lol->setTexture(anim->spriteSheet->texture);
        sprite.lol->setTextureRect(anim->spriteSheet->frames[anim->currentFrame].area);


        printf("Current frame: %i\n", anim->currentFrame);
        printf("Texture rect: %i, %i, %i, %i\n", sprite.lol->getTextureRect().left ,sprite.lol->getTextureRect().top, sprite.lol->getTextureRect().width, sprite.lol->getTextureRect().height);
    }
    }

}

