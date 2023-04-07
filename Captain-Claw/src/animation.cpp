//
// Created by Zeyad on 03/28/23.
//
#include "animation.h"
#include "SFML/Graphics.hpp"

void AnimationInit(Animation* anim, sf::Texture& texture, int frames, int rows, float frameTime, bool loop) {
    anim->texture = texture;
    anim->frames = frames;
    anim->rows = rows;
    anim->frameTime = frameTime;
    anim->loop = loop;
    anim->currentFrame = 0;
    anim->elapsedTime = 0.0f;
}

void play(Animation* anim, sf::Sprite& sprite, int row) {
    anim->sprites.emplace_back(&sprite, row);
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
    int frameWidth = anim->texture.getSize().x /anim->frames;
    int frameHeight = anim->texture.getSize().y / anim->rows;
    for (auto sprite : anim->sprites) {
        sprite.first->setTexture(anim->texture);
        sprite.first->setTextureRect(sf::IntRect(anim->currentFrame * frameWidth, sprite.second * frameHeight, frameWidth, frameHeight));
    }
}

void stop(Animation* anim, sf::Sprite& sprite) {

    for (auto it = anim->sprites.begin(); it != anim->sprites.end(); it++) {
        if (it->first == &sprite)
        {
            anim->sprites.erase(it);
        }
    }

}