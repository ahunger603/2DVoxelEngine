#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameEngine.h"

class GameEngine;

static enum GameState_def
{
	playState_def,
	MainMenuState_def,
};

class GameState
{
protected:
	GameEngine* _gameEngine;

	int _updatesPerSecond;
	int _framesPerSecond;

	bool init;
	bool paused;
	bool active;
public:
	GameState(GameEngine* gameEngine, int ups, int fps);

	GameEngine* getGameEngine();

	void setActivity(bool act);
	void setUpdatesPerSecond(int updatesPerSecond);
	void setFramesPerSecond(int fps);
	int getUpdatesPerSecond();
	int getFramesPerSecond();

	bool isActive();

	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Draw(float interpolation) = 0;
};

#endif