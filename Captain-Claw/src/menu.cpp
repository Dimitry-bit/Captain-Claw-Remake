//
// Created by Zeyad on 05/02/23.
//
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "menu.h"
#include "renderer.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "claw.h"
#include "sound_sys.h"

enum class MenuState { Main, Help, Endgame };
enum ItemState { BLANK, SELECTED };

const int ITEMS_STATES = 2;
const int NUM_MAIN_ITEMS = 3;
const int NUM_END_ITEMS = 2;

// Set the default selected item to the first item in the main menu
int selectedMainMenuItemIndex = 0;
int selectedEndMenuItemIndex = 0;

float pressTimer = 0.0f;
const float pressPeriod = 0.1f;

// Set the initial menu state to the main menu
MenuState menuState = MenuState::Main;

void MenuUpdate(float deltaTime)
{
    sf::Texture mainTexture[NUM_MAIN_ITEMS][ITEMS_STATES];
    sf::Texture endTexture[NUM_END_ITEMS + 1][ITEMS_STATES];

    sf::Sprite mainMenu[NUM_MAIN_ITEMS];
    sf::Sprite endMenu[NUM_END_ITEMS + 1];
    sf::Sprite helpSprite;
    sf::Sprite menuSprite;

    sf::Vector2u windowSize = rWindow->getSize();
    menuSprite.setTexture(ResTextureGet(UI_MENU));
    menuSprite.setScale((float) windowSize.x / menuSprite.getLocalBounds().width,
                        (float) windowSize.y / menuSprite.getLocalBounds().height);

    mainTexture[0][BLANK].loadFromFile(UI_RETURN_1);
    mainTexture[0][SELECTED].loadFromFile(UI_RETURN_2);

    mainTexture[1][BLANK].loadFromFile(UI_HELP_1);
    mainTexture[1][SELECTED].loadFromFile(UI_HELP_2);

    mainTexture[2][BLANK].loadFromFile(UI_END_1);
    mainTexture[2][SELECTED].loadFromFile(UI_END_2);

    for (int i = 0; i < NUM_MAIN_ITEMS; i++) {
        mainMenu[i].setTexture(mainTexture[i][BLANK]);
        mainMenu[i].setOrigin(mainMenu[i].getLocalBounds().width / 2, mainMenu[i].getLocalBounds().height / 2);
    }

    // Set the position of the main menu items
    mainMenu[0].setPosition(windowSize.x / 2, 300);
    mainMenu[1].setPosition(windowSize.x / 2, 400);
    mainMenu[2].setPosition(windowSize.x / 2, 500);

    mainMenu[selectedMainMenuItemIndex].setTexture(mainTexture[selectedMainMenuItemIndex][SELECTED]);

    // Create the menu items for the load game submenu
    helpSprite.setTexture(ResTextureGet(UI_HELP_0));
    helpSprite.setScale((float) windowSize.x / helpSprite.getLocalBounds().width,
                        (float) windowSize.y / helpSprite.getLocalBounds().height);

    endTexture[0][BLANK].loadFromFile(UI_YES_1);
    endTexture[0][SELECTED].loadFromFile(UI_YES_2);

    endTexture[1][BLANK].loadFromFile(UI_NO_1);
    endTexture[1][SELECTED].loadFromFile(UI_NO_2);

    endTexture[2][BLANK].loadFromFile(UI_AREYOUSURE);

    for (int i = 0; i < NUM_END_ITEMS + 1; i++) {
        endMenu[i].setTexture(endTexture[i][BLANK]);
        endMenu[i].setOrigin(endMenu[i].getLocalBounds().width / 2, endMenu[i].getLocalBounds().height / 2);
    }

    // Set the position of the main menu items
    endMenu[2].setPosition(windowSize.x / 2, 300);
    endMenu[0].setPosition(windowSize.x / 2, 400);
    endMenu[1].setPosition(windowSize.x / 2, 500);

    // Set the default selected item to the first item in the main menu
    endMenu[selectedEndMenuItemIndex].setTexture(mainTexture[selectedEndMenuItemIndex][SELECTED]);

    pressTimer += deltaTime;
    if (pressTimer >= pressPeriod) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            pressTimer = 0.0f;

            if (menuState == MenuState::Main) {
                // Move up in the main menu
                if (selectedMainMenuItemIndex > 0) {
                    SoundPlay(&ResSoundBuffGet(WAV_UI_CLICK));
                    mainMenu[selectedMainMenuItemIndex].setTexture(
                        mainTexture[selectedMainMenuItemIndex][BLANK]);
                    selectedMainMenuItemIndex--;
                    mainMenu[selectedMainMenuItemIndex].setTexture(
                        mainTexture[selectedMainMenuItemIndex][SELECTED]);
                }
            } else if (menuState == MenuState::Endgame) {
                if (selectedEndMenuItemIndex > 0) {
                    SoundPlay(&ResSoundBuffGet(WAV_UI_CLICK));
                    endMenu[selectedEndMenuItemIndex].setTexture(
                        endTexture[selectedEndMenuItemIndex][BLANK]);
                    selectedEndMenuItemIndex--;
                    endMenu[selectedEndMenuItemIndex].setTexture(
                        endTexture[selectedEndMenuItemIndex][SELECTED]);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            pressTimer = 0.0f;

            if (menuState == MenuState::Main) {
                // Move down in the main menu
                if (selectedMainMenuItemIndex < NUM_MAIN_ITEMS - 1) {
                    SoundPlay(&ResSoundBuffGet(WAV_UI_CLICK));
                    mainMenu[selectedMainMenuItemIndex].setTexture(
                        mainTexture[selectedMainMenuItemIndex][0]);
                    selectedMainMenuItemIndex++;
                    mainMenu[selectedMainMenuItemIndex].setTexture(
                        mainTexture[selectedMainMenuItemIndex][1]);
                }
            } else if (menuState == MenuState::Endgame) {
                if (selectedEndMenuItemIndex < NUM_END_ITEMS - 1) {
                    SoundPlay(&ResSoundBuffGet(WAV_UI_CLICK));
                    endMenu[selectedEndMenuItemIndex].setTexture(
                        endTexture[selectedEndMenuItemIndex][BLANK]);
                    selectedEndMenuItemIndex++;
                    endMenu[selectedEndMenuItemIndex].setTexture(
                        endTexture[selectedEndMenuItemIndex][SELECTED]);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            pressTimer = 0.0f;

            if (menuState == MenuState::Main) {
                SoundPlay(&ResSoundBuffGet(WAV_UI_SELECT));
                // Check which main menu item was selected
                switch (selectedMainMenuItemIndex) {
                    case 0:isGamePaused = false;
                        break;
                    case 1:
                        // Switch to the load game submenu
                        menuState = MenuState::Help;
                        mainMenu[selectedMainMenuItemIndex].setTexture(
                            mainTexture[selectedMainMenuItemIndex][BLANK]);
                        break;
                    case 2:menuState = MenuState::Endgame;
                        mainMenu[selectedMainMenuItemIndex].setTexture(
                            mainTexture[selectedMainMenuItemIndex][BLANK]);
                        selectedEndMenuItemIndex = 0;
                        endMenu[selectedEndMenuItemIndex].setTexture(endTexture[selectedEndMenuItemIndex][SELECTED]);
                        break;
                }
            } else if (menuState == MenuState::Endgame) {
                switch (selectedEndMenuItemIndex) {
                    case 0:rWindow->close();
                        break;
                    case 1:SoundPlay(&ResSoundBuffGet(WAV_UI_SELECT));
                        menuState = MenuState::Main;
                        endMenu[selectedEndMenuItemIndex].setTexture(endTexture[selectedEndMenuItemIndex][BLANK]);
                        selectedMainMenuItemIndex = 0;
                        mainMenu[selectedMainMenuItemIndex].setTexture(
                            mainTexture[selectedMainMenuItemIndex][SELECTED]);
                        break;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            pressTimer = 0.0f;

            if (menuState == MenuState::Main) {
                // Switch back to the main menu
                SoundPlay(&ResSoundBuffGet(WAV_UI_SELECT));
                menuState = MenuState::Endgame;
                mainMenu[selectedMainMenuItemIndex].setTexture(mainTexture[selectedMainMenuItemIndex][BLANK]);
                selectedEndMenuItemIndex = 0;
                endMenu[selectedEndMenuItemIndex].setTexture(
                    endTexture[selectedEndMenuItemIndex][SELECTED]);
            } else if (menuState == MenuState::Help) {
                // Switch back to the main menu
                SoundPlay(&ResSoundBuffGet(WAV_UI_SELECT));
                menuState = MenuState::Main;
                selectedMainMenuItemIndex = 0;
                mainMenu[selectedMainMenuItemIndex].setTexture(
                    mainTexture[selectedMainMenuItemIndex][SELECTED]);
            } else if (menuState == MenuState::Endgame) {
                // Switch back to the main menu
                SoundPlay(&ResSoundBuffGet(WAV_UI_SELECT));
                menuState = MenuState::Main;
                endMenu[selectedEndMenuItemIndex].setTexture(endTexture[selectedEndMenuItemIndex][BLANK]);
                selectedMainMenuItemIndex = 0;
                mainMenu[selectedMainMenuItemIndex].setTexture(
                    mainTexture[selectedMainMenuItemIndex][SELECTED]);
            }
        }
    }

    if (menuState != MenuState::Help) {
        rWindow->draw(menuSprite);
    } else {
        rWindow->draw(helpSprite);
    }

    if (menuState == MenuState::Main) {
        for (int i = 0; i < NUM_MAIN_ITEMS; i++) {
            rWindow->draw(mainMenu[i]);
        }

    } else if (menuState == MenuState::Endgame) {
        for (int i = 0; i < NUM_END_ITEMS + 1; i++) {
            rWindow->draw(endMenu[i]);
        }
    }
}