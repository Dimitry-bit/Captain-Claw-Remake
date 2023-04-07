//
// Created by Zeyad on 03/28/23.
//
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


struct Animation {

    sf::Texture texture;
    int frames;
    int rows;
    float frameTime;
    bool loop;
    int currentFrame;
    float elapsedTime;
    std::vector<std::pair<sf::Sprite*, int>> sprites;
};

void AnimationInit(Animation* anim, sf::Texture& texture, int frames, int rows, float frameTime, bool loop = true);

void play(Animation* anim, sf::Sprite& sprite, int row);
void update(Animation* anim, float deltaTime);
void stop(Animation* anim, sf::Sprite& sprite);