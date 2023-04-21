#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "slider.h"
using namespace sf;
void SettingsSoundSlider(Texture slider_tex,Sprite slider,Music music){
    int volume=100;
    float currentSize;
    music.setVolume(volume);
    if (Keyboard::isKeyPressed(Keyboard::Left)){
        int x=1;
        float realSize=slider_tex.getSize().x;
        music.setVolume(volume-25);
        slider.setTextureRect(IntRect(0,0,realSize-(x*realSize*(25/100)),slider_tex.getSize().y));
        currentSize=realSize-(x*realSize*(25/100));
        x++;
        if (x>=4) {
            x = 4;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)){
        int x=1;
        float realSize=slider_tex.getSize().x;
        music.setVolume(volume+25);
        slider.setTextureRect(IntRect(0,0,currentSize+(x*realSize*(25/100)),slider_tex.getSize().y));
        x++;
        if (x>=4){
            x=4;
        }
    }

}

void SettingsSfxSlider(Texture sfxSlider_tex,Sprite sfxSlider,Sound sound) {

    int volume = 100;
    float currentSize;
    sound.setVolume(volume);
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        int x = 1;
        float realSize = sfxSlider_tex.getSize().x;
        sound.setVolume(volume - 25);
        sfxSlider.setTextureRect(IntRect(0, 0, realSize - (x * realSize * (25 / 100)), sfxSlider_tex.getSize().y));
        currentSize = realSize - (x * realSize * (25 / 100));
        x++;
        if (x >= 4) {
            x = 4;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        int x = 1;
        float realSize = sfxSlider_tex.getSize().x;
        sound.setVolume(volume + 25);
        sfxSlider.setTextureRect(IntRect(0, 0, currentSize + (x * realSize * (25 / 100)), sfxSlider_tex.getSize().y));
        x++;
        if (x >= 4) {
            x = 4;
        }


    }
}