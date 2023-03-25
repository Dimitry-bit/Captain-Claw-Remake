# Readme

# Conventions

## Coding Convention

Code element | Convention | Example
--- | :---: | ---
Defines | ALL_CAPS | `#define PLATFORM_DESKTOP`
Variables | lowerCase | `int screenWidth = 0;`, `float targetFrameTime = 0.016f;`
Local variables | lowerCase | `Vector2 playerPosition = { 0 };`
Global variables | lowerCase | `bool windowReady = false;`
Constants | lowerCase | `const int maxValue = 8;`
Pointers | snake_case_t *pointer | `texture2D_t *array = NULL;`
float values | always x.xf | `float gravity = 10.0f`
Operators | value1*value2 | `int product = value*6;`
Operators | value1/value2 | `int division = value/4;`
Operators | value1 + value2 | `int sum = value + 10;`
Operators | value1 - value2 | `int res = value - 5;`
Enum | snake_case_t | `enum texture_format_t`
Enum members | ALL_CAPS | `PIXELFORMAT_UNCOMPRESSED_R8G8B8`
Struct | snake_case_t | `struct texture2D_t`, `struct material_t`
Struct members | lowerCase | `texture.width`, `color.r`
Functions | TitleCase | `InitWindow()`, `LoadImageFromMemory()`
Functions params | lowerCase | `width`, `height`
Ternary Operator | (condition) ? result1 : result2 | `printf("Value is 0: %s", (value == 0) ? "yes" : "no");`

_NOTE: Avoid ~~`namespace std`~~_
```cpp 
using namespace std;
```

- Some other conventions to follow:
	 - **ALWAYS** initialize all defined variables.
	 - **Do not use TABS**, use 4 spaces instead.
	 - Avoid trailing spaces, please, avoid them
	 - Control flow statements always are followed **by a space**:

```cpp
if (condition) 
	value = 0;

while (!WindowShouldClose()) {

}

for (int i = 0; i < NUM_VALUES; i++) 
	cout << i;

// Be careful with the switch formatting!
switch (value) {
    case 0: {

    } break;
    case 2: break;
    default: break;
}
```

 - All conditions checks are **always between parenthesis** but not boolean values:
```cpp
if ((value > 1) && (value < 50) && valueActive) {

}
```

 - When dealing with braces or curly brackets, open-close them in aligned mode (Functions):
```cpp
void SomeFunction()
{
   // TODO: Do something here!
}
```

* Naming Convention
	- Directories will be named using `snake_case`: `resources/models`, `resources/fonts`
	- Files will be named using `snake_case`: `main_title.png`, `cubicmap.png`, `sound.wav`

_NOTE: Avoid any space or special character in the files/dir naming!_ 

## Games/Examples Directories Organization Conventions
  
 - Data files should be organized by context and usage in the game, think about the loading requirements for data and put all the resources that need to be loaded at the same time together.
 - Use descriptive names for the files, it would be perfect if just reading the name of the file, it was possible to know what is that file and where fits in the game.
 - Here it is an example, note that some resources require to be loaded all at once while other require to be loaded only at initialization (gui, font).
  
```
resources/audio/fx/long_jump.wav
resources/audio/music/main_theme.ogg
resources/screens/logo/logo.png
resources/screens/title/title.png
resources/screens/gameplay/background.png
resources/characters/player.png
resources/characters/enemy_slime.png
resources/common/font_arial.ttf
resources/common/gui.png
```

---

# Game Engine:

- Animation
  ---
	- Play Animation
	- Stop Animation
	- Restart Animation
	- Pause Animation

	---

- Logging (`printf()` )
  ---
	- Log Types:
		- Info
		- Warning
		- Error
		- Fatal
	- Syntax: `[LOG_TYPE][SCRIPT]: MESSAGE.`
		- `[INFO][Game]: Initialization finished successfully.`
	  
	---

- Drawing
  ---
	- Layers
		- Background (bg)
		- Middle
		- Frontground (fg)
	- Window resizing (optional)

	---

- Resource Management (optional)
  ---
	- Easy Loading/Unloading to resources
	- Easy Access to resources

	---

- Graphical User Interface
	- Button
		- Add
		- Update
		- Remove
	- Text (sf::Text)
		- Add
		- Edit
		- Remove

---

# Game

- Loading resources
  ---
	- Texture
	- Sound
	- Font
 
	---

- Level Design
  ---
	- Smooth level transitions

	---
  
- Game GUI
  ---
	- Main Menu
		- Select Level
		- Settings
			- Music Slider
			- Ambient Slider
			- SFX Slider
		- Keymap

		---

	- Pause Menu
		- Exit Game
		- Exit To Main Menu
		- Restart Level
		- Restart To Nearest Checkpoint
		- Settings
		- Keymap
  
		---

	- Current weapon
		- Melee
			- Sword
		- Ranged
			- Pistole
			- Magic
	- Current weapon ammo

		---

	- Gold counter
	- Health
	- Lives

	---

- Game Mechanics
  ---
	- Moving platforms
	- Hidden platforms
	- Pickups
		- Gold
		- Health potion
		- Ranged weapons ammo
	- Check points

	---
  
- Player mechanics
  ---
	- Move right/left
	- Move Up/Down (Ladders) 
	- Smooth Jumping
		- Time calculated jump (NO RANDOM FORCE)
	- Attack
		- Melee
		- Ranged
	- Animation
		- Attack
		- Reload (if ranged)
		- Death
		- Move
			- right/left
			- up/down
		- Damage
	- Lives and live pickups
	- Health
	- Damage
		- Red glow when player take damage
  
	---

- Enemy AI
  ---
	- Move right/left
	- Shooting
		- Melee
		- Ranged
	- Animation
		- Attack
		- Reload (if ranged)
		- Death
		- Move
		- Damage
	- Health
	- Drops
		- Gold
		- Ammo
		- Health
---