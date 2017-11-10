#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "GameEngine.h"
#include "MainMenuState.h"
#include "InputManager.h"
#include "ResourceManager.h"

//Constants
//const int screenW = 1920, screenH = 1080;
const int screenW = 1280, screenH = 720;

//Global Var
static bool running;

int main(int argc, char* argv[])
{
	//Window init
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	//SDL INIT
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	//Core Components
	window = window = SDL_CreateWindow("SideScroller", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, NULL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	InputManager* inputManager = new InputManager();
	ResourceManager* resourceManger = new ResourceManager(renderer, window);
	resourceManger->loadDefaultTextures();

	GameEngine gameEngine = GameEngine(window, renderer, inputManager, resourceManger);
	gameEngine.Init();

	//Enter game
	GameState* menuState = new MainMenuState(&gameEngine);
	gameEngine.PushState(menuState);

	//Main Game Loop
	running = true;
	while (running)
	{
		//Handle Events---------------------------------------
		gameEngine.HandleEvents();

		//Update Game-----------------------------------------
		gameEngine.Update();

		//Draw Game-------------------------------------------
		gameEngine.Draw();
		
		//Quit Check------------------------------------------
		if (!gameEngine.Running())
			running = false;
	}

	//Game Deallocation
	gameEngine.Cleanup();
	delete inputManager;
	delete resourceManger;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
	return 0;
}