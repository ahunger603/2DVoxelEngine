#include <iostream>
#include <list>
#include <string>
#include <Windows.h>
#include <ctime>
#include <stdlib.h>   
#include "GameEngine.h"

GameEngine::GameEngine(SDL_Window* window, SDL_Renderer* renderer, InputManager* inputManager, ResourceManager* resourceManager)
{
	//Primary Engine Components
	_window = window;
	_renderer = renderer;

	_resourceManager = resourceManager;
	_inputManager = inputManager;
	_inputManager->setGameEngine(this);

	//Loop Regulating Variable
	start_fcount_tick = SDL_GetTicks();
	next_fps = SDL_GetTicks();
	next_update_tick = SDL_GetTicks();
	next_draw_tick = SDL_GetTicks();
	updates = 0;
	interpolation = 0;

	fps = 0;
	avg_fps = 0;
	frames = 0;
	total_frames = 0;
	frames_index = 0;
	for (int i = 0; i < FPS_ACCURACY; i++)
		frames_array[i] = 0;

	title = "";
	titleRect.x = titleRect.y = 0;

	//Other Var
	debugMode = true;

	int windowSize[2] = { 0, 0 };
	SDL_GetWindowSize(_window, &windowSize[0], &windowSize[1]);
	windowRect = { 0, 0, windowSize[0], windowSize[1] };
}

SDL_Window* GameEngine::getWindow()
{
	return _window;
}
SDL_Renderer* GameEngine::getRenderer()
{
	return _renderer;
}
InputManager* GameEngine::getInputManager()
{
	return _inputManager;
}
ResourceManager* GameEngine::getResManager()
{
	return _resourceManager;
}

void GameEngine::SetUpdatesPerSec(int ups)
{
	UPDATES_PER_SECOND = ups;
	UPDATE_SKIP_TICKS = 1000 / ups;

	next_update_tick = SDL_GetTicks();
}

void GameEngine::SetFramesPerSec(int _fps)
{
	MAX_FRAMES_PER_SECOND = _fps;
	FRAME_SKIP_TICKS = 1000 / _fps;

	next_draw_tick = SDL_GetTicks();
	next_fps = SDL_GetTicks();
	start_fcount_tick = SDL_GetTicks();

	fps = 0;
	avg_fps = 0;
	frames = 0;
	total_frames = 0;
	frames_index = 0;
	for (int i = 0; i < FPS_ACCURACY; i++)
		frames_array[i] = 0;
}

void GameEngine::Init()
{
	running = true;
}

void GameEngine::Cleanup()
{
	if (gameStates.size() > 0)
	{
		auto gameStatesEnd = gameStates.end();
		for (auto it = gameStates.begin(); it != gameStatesEnd; it++)
			(*it)->Cleanup();
	}
}

void GameEngine::ResumeFront()
{
	if (gameStates.size() > 0)
	{
		GameState* gs = *gameStates.begin();
		gs->Resume();
	}
}

void GameEngine::PushState(GameState* state)
{
	gameStates.push_front(state);
	state->Init();
}
void GameEngine::removeState(std::list<GameState*>::iterator it)
{
	GameState* gs = (*it);

	gameStates.erase(it);

	gs->Cleanup();

	delete gs;
}
void GameEngine::PopFrontState()
{
	GameState* gs = gameStates.front();
	gameStates.pop_front();

	gs->Cleanup();
	
	delete gs;
}
void GameEngine::PopBackState()
{
	GameState* gs = gameStates.back();
	gameStates.pop_back();

	gs->Cleanup();

	delete gs;
}

void GameEngine::HandleEvents()
{
	//Update Inputs
	_inputManager->update();

	//Handle Inputs
	if (gameStates.size() > 0)
	{
		(*gameStates.begin())->HandleEvents();
	}
}
void GameEngine::Update()
{
	//Update Loop Regulation
	if (gameStates.size() > 0)
	{
		GameState* gs = (*gameStates.begin());

		//Set Updates per sec
		int ups = gs->getUpdatesPerSecond();
		if (ups != UPDATES_PER_SECOND)
		{
			SetUpdatesPerSec(ups);
		}

		//Set Frames per sec
		int fps = gs->getFramesPerSecond();
		if (gs->getFramesPerSecond() != MAX_FRAMES_PER_SECOND)
		{
			SetFramesPerSec(fps);
		}
	}

	//State Update Loop
	updates = 0;
	while (SDL_GetTicks() > next_update_tick && updates < MAX_UPDATES)
	{
		if (gameStates.size() > 0)
		{
			//Update States
			auto gameStatesEnd = gameStates.end();
			for (auto it = gameStates.begin(); it != gameStates.end(); it++)
			{
				GameState* gs = (*it);
				if (gs->isActive())
					gs->Update();
				else
					removeState(it++);
			}
		}

		//Set Update Regulating
		next_update_tick += UPDATE_SKIP_TICKS;
		updates++;
	}
}
void GameEngine::Draw()
{
	if (SDL_GetTicks() > next_draw_tick)
	{
		//Clear
		SDL_RenderClear(_renderer);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(_renderer, &windowRect);


		//Draw States
		interpolation = float(SDL_GetTicks() + UPDATE_SKIP_TICKS - next_update_tick) / float(UPDATE_SKIP_TICKS);
		if (gameStates.size() > 0)
		{
			for (auto it = gameStates.rbegin(); it != gameStates.rend(); it++)
				(*it)->Draw(interpolation);
		}

		//Draw FPS
		if (debugMode)
		{
			//SDL_Surface* titleSurf = TTF_RenderText_Solid(_resourceManager->getFont(1, 20), title.c_str(), SDL_COLORS.at(WHITE));
			//SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(_renderer, titleSurf);
			//SDL_QueryTexture(titleTexture, NULL, NULL, &titleRect.w, &titleRect.h);

			//SDL_RenderCopy(_renderer, titleTexture, NULL, &titleRect);

			//SDL_FreeSurface(titleSurf);
			//SDL_DestroyTexture(titleTexture);

			char *a = new char[title.size() + 1];
			a[title.size()] = 0;
			memcpy(a, title.c_str(), title.size());
			_resourceManager->drawText(font_ALPHABETA_20_W, a, 0, 0);
		}

		//Display
		SDL_RenderPresent(_renderer);

		//Set Draw Regulating
		next_draw_tick += FRAME_SKIP_TICKS;
		frames++;
		total_frames++;
	}

	//Calculate FPS--------------------------------------------
	if (debugMode)
	{
		if (SDL_GetTicks() > next_fps)
		{
			fps -= frames_array[frames_index];
			frames_array[frames_index] = frames;
			fps += frames;

			avg_fps = total_frames / float((SDL_GetTicks() - start_fcount_tick) / 1000.0);
			title = "Current FPS: " + std::to_string(fps) + " | Avg FPS: " + std::to_string(avg_fps);

			frames_index = (frames_index + 1) % FPS_ACCURACY;

			frames = 0;
			next_fps += FPS_SKIP_TICKS;
		}
	}
}