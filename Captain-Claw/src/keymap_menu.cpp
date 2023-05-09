#include <SFML/Graphics.hpp>
#include "keymap_menu.h"
#include "resource_manager.h"

void keymap_functionality()
{
    font_of_texts= ResFontGet("JetBrainsMono-Regular.ttf");
    for (int i=0;i<=6 ;i++)
    {
        arr[i].setFont(font_of_texts);
        arr[i].setCharacterSize(25);
        arr[i] .setFillColor(sf::Color::Yellow);
        arr[i].setStyle(sf::Text::Bold);
        arr[i].setPosition(400,400+(i*100));
    }
    arr[0].setString("arr[0]:S : Move down");
    arr[1].setString("arr[1]:D: Move Right");
    arr[2].setString("arr[2]:A : Move left");
    arr[3].setString("arr[3]: Space : rise up ");
    arr[4].setString("arr[4]: Num1 : Pistol " );
    arr[5].setString("arr[5]: Num2 : Magic " );
    arr[6].setString("arr[6]: Num3: Bomb " );
    arr[7].setString("arr[7]: Ctrl : sword " );
}
