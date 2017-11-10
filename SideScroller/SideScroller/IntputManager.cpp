#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
	exit = false;

	mouse = new Mouse();

	//Create Keys
	for (auto it = KEY_MAP.begin(); it != KEY_MAP.end(); it++)
	{
		bool* states = new bool[4];

		for (int i = 0; i < 4; i++)
			states[i] = false;

		std::pair<KEY, bool*> keystates(it->second, states);

		keyStateTable.insert(keystates);
	}

	//Targets
	gameEngine = NULL;
}

InputManager::~InputManager()
{
	delete mouse;
	for (auto it = keyStateTable.begin(); it != keyStateTable.end(); it++)
	{
		delete it->second;
	}
}

void InputManager::update()
{
	//Reset Values
	mouse->used = false;
	mouse->left_pressed = false;
	mouse->right_pressed = false;
	mouse->left_released = false;
	mouse->right_released = false;

	//Store Previous values
	bool pastLeft_down = mouse->left_down;
	bool pastRight_down = mouse->right_down;

	while (SDL_PollEvent(&ev))
	{
		int rawKey = (int)ev.button.button;
		switch (ev.type)
		{
		case SDL_QUIT:
			gameEngine->Quit();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (rawKey == 1)
				mouse->left_down = true;
			if (rawKey == 3)
				mouse->right_down = true;
			mouse->mx = ev.motion.x;
			mouse->my = ev.motion.y;
			break;
		case SDL_MOUSEBUTTONUP:
			if (rawKey == 1)
				mouse->left_down = false;
			if (rawKey == 3)
				mouse->right_down = false;
			mouse->dragging = false;
			mouse->mx = ev.motion.x;
			mouse->my = ev.motion.y;
			break;
		case SDL_MOUSEMOTION:
			//Dragging
			if (mouse->left_down || mouse->right_down)
			{
				mouse->dragging = true;
				mouse->mx2 = ev.motion.x;
				mouse->my2 = ev.motion.y;
			}
			else
			{
				mouse->dragging = false;
				mouse->mx = ev.motion.x;
				mouse->my = ev.motion.y;
				mouse->mx2 = ev.motion.x;
				mouse->my2 = ev.motion.y;
			}
			break;
		case SDL_KEYDOWN:
			setKeyState(rawKey, true);
			break;
		case SDL_KEYUP:
			setKeyState(rawKey, false);
			break;
		}
	}

	//Process Keys
	for (auto it = keyStateTable.begin(); it != keyStateTable.end(); it++)
	{
		bool* states = it->second;
		states[2] = false;
		states[3] = false;

		if (states[0] && !states[1])
			states[2] = true;
		if (!states[0] && states[1])
			states[3] = true;

		states[1] = states[0];
	}

	//Proccessing Mouse Raw
	//Left
	if (!pastLeft_down && mouse->left_down)
		mouse->left_pressed = true;

	if (pastLeft_down && !mouse->left_down)
		mouse->left_released = true;

	//Right
	if (!pastRight_down && mouse->right_down)
		mouse->right_pressed = true;

	if (pastRight_down && !mouse->right_down)
		mouse->right_released = true;
}

void InputManager::setKeyState(int key, bool state)
{
	try
	{
		if (printKeys)
			std::cout << key << std::endl;

		keyStateTable.at(KEY_MAP.at(key))[0] = state;
	}
	catch (...){}
}

bool InputManager::getKeyState(KEY key)
{
	if (keyStateTable.find(key) != keyStateTable.end())
		return keyStateTable.at(key)[0];

	return false;
}

bool InputManager::isKeyPressed(KEY key)
{
	if (keyStateTable.find(key) != keyStateTable.end())
		return keyStateTable.at(key)[2];

	return false;
}

bool InputManager::isKeyReleased(KEY key)
{
	if (keyStateTable.find(key) != keyStateTable.end())
		return keyStateTable.at(key)[3];

	return false;
}

Mouse* InputManager::getMouse()
{
	return mouse;
}

void InputManager::getMouseXY(int* xy)
{
	xy[0] = mouse->mx;
	xy[1] = mouse->my;
}

//Target Input
void InputManager::setGameEngine(GameEngine* _gameEngine)
{
	gameEngine = _gameEngine;
}


//Mouse -----------------------------------------------------

Mouse::Mouse()
{
	mx = 0;
	my = 0;
	mx2 = 0;
	my2 = 0;
	left_pressed = false;
	left_down = false;
	left_released = false;

	right_pressed = false;
	right_down = false;
	right_released = false;

	dragging = false;

	used = false;
}

void Mouse::getMouseXY(int* xy)
{
	if (!dragging)
	{
		xy[0] = mx;
		xy[1] = my;
	}
	else
	{
		xy[0] = mx2;
		xy[1] = my2;
	}
}
void Mouse::getMouseStartXY(int* xy)
{
	xy[0] = mx;
	xy[1] = my;
}

bool Mouse::isLeftPressed() { return left_pressed; }
bool Mouse::isLeftDown() { return left_down; }
bool Mouse::isLeftReleased() { return left_released; }

bool Mouse::isRightPressed(){ return right_pressed; }
bool Mouse::isRightDown(){ return right_down; }
bool Mouse::isRightReleased() { return right_released; }

bool Mouse::isDragging() { return dragging; }

bool Mouse::isUsed() { return used; }
void Mouse::Used() { used = true; }

