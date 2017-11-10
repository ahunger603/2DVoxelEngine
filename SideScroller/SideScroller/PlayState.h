#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "SDL.h"
#include "World.h"
#include "GameState.h"
#include "GameEngine.h"

class PlayState : public GameState
{
private:
	World* _world;

	SDL_Texture* pauseOverlay;
public:
	PlayState(GameEngine* gameEngine);

	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw(float interpolation);
};

#endif