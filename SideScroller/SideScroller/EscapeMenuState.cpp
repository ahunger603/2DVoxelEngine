#include "EscapeMenuState.h"

EscapeMenuState::EscapeMenuState(GameEngine* gameEngine) : GameState(gameEngine, 100, 60)
{
	init = false;
	paused = false;
	active = true;
}

void EscapeMenuState::Init()
{
	int windowSize[2] = { 0, 0 };
	SDL_GetWindowSize(_gameEngine->getWindow(), &windowSize[0], &windowSize[1]);

	escapeMenu = new EscapeMenu(this, (windowSize[0] / 2) - 104,( windowSize[1] / 2) - 150, 208, 300);

	init = true;
}
void EscapeMenuState::Cleanup()
{
	_gameEngine->ResumeFront(); 
}



void EscapeMenuState::Pause()
{
	paused = true;
}
void EscapeMenuState::Resume()
{
	paused = false;
}



void EscapeMenuState::HandleEvents()
{
	InputManager* inputManager = _gameEngine->getInputManager();

	if (inputManager->isKeyReleased(KEY_ESC))
	{
		active = false;
	}

	escapeMenu->handleEvents();
}
void EscapeMenuState::Update()
{
	if (init && !paused)
	{
		escapeMenu->update();
	}
}
void EscapeMenuState::Draw(float interpolation)
{
	if (init && !paused)
	{
		escapeMenu->draw();
	}
}