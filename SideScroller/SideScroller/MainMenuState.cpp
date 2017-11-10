#include <iostream>
#include "MainMenuState.h"

MainMenuState::MainMenuState(GameEngine* gameEngine) : GameState(gameEngine, 100, 60)
{
	init = false;
	paused = false;
	active = true;
}

void MainMenuState::Init()
{
	int windowSize[2] = { 0, 0 };
	SDL_GetWindowSize(_gameEngine->getWindow(), &windowSize[0], &windowSize[1]);

	_mainMenu = new MainMenu(this, windowSize[0], windowSize[1]); 

	init = true;
}
void MainMenuState::Cleanup()
{
	delete _mainMenu;
}



void MainMenuState::Pause()
{
	paused = true;
}
void MainMenuState::Resume()
{
	paused = false;
}



void MainMenuState::HandleEvents()
{
	if (init && !paused)
	{
		_mainMenu->handleEvents();
	}
}
void MainMenuState::Update()
{
	if (init && !paused)
	{
		_mainMenu->update();
	}
}
void MainMenuState::Draw(float interpolation)
{
	if (init && !paused)
	{
		_mainMenu->draw();
	}
}