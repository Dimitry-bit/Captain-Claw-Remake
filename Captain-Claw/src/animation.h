//
// Created by Zeyad on 03/28/23.
//
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// A structure holding the information about an animator (like a modified sprite)
struct Animator {
    sf::Sprite* lol; //Pointer to the sprite to be animated
    int row; //Row indicating the desired animation from the spritesheet
    bool pause = false; //Flag indicating whether the animation is paused or not (default is not paused)
};


// A structure holding the information about an animation
struct Animation {

    sf::Texture* texture;
    int frames; // Total number of frames in the animation
    int rows; // Number of rows (animations) in the animation spritesheet
    float frameTime; // Duration of each frame
    bool loop; // Flag to indicate if the animation should loop
    int currentFrame; // Index of the current frame being displayed
    float elapsedTime;
    std::vector<Animator> sprites; // Vector of animators for this animation
};
// Function to initialize an animation
void AnimationInit(Animation* anim, sf::Texture* texture, int frames, int rows, float frameTime, bool loop = true);

// Function to initialize an animator
void AnimatorInit(Animator& animtr, sf::Sprite& sprite);

// Function to start playing an animation (a request to play, i.e add to the vector of animators)
void play(Animation* anim, Animator& animtr, int row);

// Function to update an animation (handles the current requests, i.e the vector of animators)
void update(Animation* anim, float deltaTime);

// Function to stop an animation (a request to terminate, i.e remove from the vector of animators)
void stop(Animation* anim, Animator& animtr);

// Function to pause an animation (sets the flag to pause)
void pause(Animation* anim, Animator& animtr);

// Function to resume an animation (sets the flag to resume)
void resume(Animation* anim, Animator& animtr);

// Function to inverse the state of an animation (if pause then resume and vice versa)
void inverseState(Animation* anim, Animator& animtr);