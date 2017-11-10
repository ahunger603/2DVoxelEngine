#include "GameState.h"

GameState::GameState(GameEngine* gameEngine,int ups, int fps)
{
	_gameEngine = gameEngine;

	_updatesPerSecond = ups;
	_framesPerSecond = fps;
}

GameEngine* GameState::getGameEngine()
{ 
	return _gameEngine; 
}

//Set
void GameState::setActivity(bool act)
{
	active = act;
}
void GameState::setUpdatesPerSecond(int ups)
{
	_updatesPerSecond = ups;
}
void GameState::setFramesPerSecond(int fps)
{
	_framesPerSecond = fps;
}
//Get
int GameState::getUpdatesPerSecond()
{
	return _updatesPerSecond;
}
int GameState::getFramesPerSecond()
{
	return _framesPerSecond;
}

bool GameState::isActive() 
{ 
	return active; 
}
