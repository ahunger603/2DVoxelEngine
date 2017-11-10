#ifndef ESCAPE_STATE_H
#define ESCAPE_STATE_H

#include "GameState.h"
#include "EscapeMenu.h"

class EscapeMenu;

class EscapeMenuState : public GameState
{
private:
	EscapeMenu* escapeMenu;
public:
	EscapeMenuState(GameEngine* gameEngine);

	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw(float interpolation);
};

#endif