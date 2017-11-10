#include <ctime>
#include <iostream>
#include "PlayState.h"
#include "EscapeMenuState.h"

PlayState::PlayState(GameEngine* gameEngine) : GameState(gameEngine, World::LOADING_UPS, World::LOADING_FPS)
{
	init = false;
	paused = false;
	active = true;
}

void PlayState::Init()
{
	_world = new World(_gameEngine, this);

	init = true;
}
void PlayState::Cleanup()
{
	delete _world;
}

void PlayState::Pause()
{
	paused = true;

	int sdim[2] = { 0, 0 };
	SDL_GetWindowSize(_gameEngine->getWindow(), &sdim[0], &sdim[1]);
	pauseOverlay = _gameEngine->getResManager()->CreateColorTexture(sdim[0], sdim[1], 0xAA010101);
}
void PlayState::Resume()
{
	paused = false;
	SDL_DestroyTexture(pauseOverlay);
}

void PlayState::HandleEvents()
{
	InputManager* inputManager = _gameEngine->getInputManager();

	if (inputManager->isKeyReleased(KEY_ESC))
	{
		_world->closeAllInGameMenus();

		Pause();
		_gameEngine->PushState(new EscapeMenuState(_gameEngine));
	}

	if (init && !paused)
	{
		_world->handleEvents();
	}
}
void PlayState::Update()
{
	if (init && !paused)
	{
		_world->update();
	}
}

void PlayState::Draw(float interpolation)
{
	if (init)
	{
		if (!paused)
			_world->draw(_gameEngine->getRenderer(), interpolation, paused);
		else
		{
			_world->draw(_gameEngine->getRenderer(), interpolation, paused);
			SDL_RenderCopy(_gameEngine->getRenderer(), pauseOverlay, NULL, NULL);
		}
	}
}