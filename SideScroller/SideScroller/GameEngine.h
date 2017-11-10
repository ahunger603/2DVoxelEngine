#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "SDL.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "GameState.h"
#include <list>
#include <string>
#include <Windows.h>
#include <ctime>
#include <stdlib.h>    

class GameState;
class InputManager;
class ResourceManager;

const int FPS_ACCURACY = 8;
const int FPS_SKIP_TICKS = 1000 / FPS_ACCURACY;

class GameEngine
{
	friend GameState; 

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	InputManager* _inputManager;
	ResourceManager* _resourceManager;

	std::list<GameState*> gameStates;

	bool running;

	//Default Engine Regulating Values
	int UPDATES_PER_SECOND = 45;
	int UPDATE_SKIP_TICKS = 1000 / UPDATES_PER_SECOND;
	int MAX_UPDATES = 5;

	int MAX_FRAMES_PER_SECOND = 250;
	int FRAME_SKIP_TICKS = 1000 / MAX_FRAMES_PER_SECOND;

	//Loop Regulating Variables
	uint32_t start_fcount_tick;
	uint32_t next_fps;
	uint32_t next_update_tick;
	uint32_t next_draw_tick;
	int updates;
	float interpolation;

	int fps;
	float avg_fps;
	int frames;
	int total_frames;
	int frames_index;
	int frames_array[FPS_ACCURACY];

	std::string title = "";
	SDL_Rect titleRect;

	//Other Var
	bool debugMode;
	SDL_Rect windowRect;

public:
	GameEngine(SDL_Window* window, SDL_Renderer* renderer, InputManager* inputManager, ResourceManager* resourceManager);

	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
	InputManager* getInputManager();
	ResourceManager* getResManager();

	void SetUpdatesPerSec(int ups);
	void SetFramesPerSec(int fps);

	void Init();
	void Cleanup();

	void ResumeFront();

	void PushState(GameState* state);
	void removeState(std::list<GameState*>::iterator it);
	void PopFrontState();
	void PopBackState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }
	void Quit() { running = false; }
};


#endif