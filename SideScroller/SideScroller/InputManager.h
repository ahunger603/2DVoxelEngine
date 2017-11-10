#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "SDL.h"
#include "GameEngine.h"
#include <unordered_map>

class GameEngine;
class Mouse;

const bool printKeys = false;

static enum KEY
{
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_I,
	KEY_ESC,
};

static std::unordered_map<int, KEY> KEY_MAP =
{
	{ 26, KEY_W },
	{ 22, KEY_S },
	{ 4, KEY_A },
	{ 7, KEY_D },
	{ 12, KEY_I},
	{ 41, KEY_ESC}
};

class InputManager
{
private:
	SDL_Event ev;
	//Other
	bool exit;

	//Keyboard
	std::unordered_map<KEY, bool*> keyStateTable;

	//Targets
	GameEngine* gameEngine;

	void setKeyState(int key, bool state);
public:
	//Raw Mouse
	Mouse* mouse;

	//Constructor
	InputManager();
	~InputManager();

	//Functions
	void update();
	bool getKeyState(KEY key);
	bool isKeyPressed(KEY key);
	bool isKeyReleased(KEY key);

	Mouse* getMouse();
	void getMouseXY(int* xy);

	//Target Input
	void setGameEngine(GameEngine* _gameEngine);
};

//Mouse
class Mouse
{
	friend InputManager;
private:
	int mx;
	int my;
	int mx2;
	int my2;
	bool left_pressed;
	bool left_down;
	bool left_released;

	bool right_pressed;
	bool right_down;
	bool right_released;

	bool dragging;

	bool used;
public:
	Mouse();

	void getMouseXY(int* xy);
	void getMouseStartXY(int* xy);

	bool isLeftPressed();
	bool isLeftDown();
	bool isLeftReleased();

	bool isRightPressed();
	bool isRightDown();
	bool isRightReleased();

	bool isDragging();

	bool isUsed();
	void Used();
};

#endif