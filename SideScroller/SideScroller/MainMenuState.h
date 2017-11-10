#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "GameState.h"
#include "MainMenu.h"

class MainMenu;

class MainMenuState : public GameState
{
private:
	MainMenu* _mainMenu;
public:
	MainMenuState(GameEngine* gameEngine);

	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw(float interpolation);
};

#endif