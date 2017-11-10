#include "World.h"

//------------------------------------------------------------------|
//-------------------------------WORLD------------------------------|
//------------------------------------------------------------------|

const int World::LOADING_UPS = 1000;
const int World::LOADING_FPS = 60;
const int World::RUNNING_UPS = 45;
const int World::RUNNING_FPS = 250;

//Create WorldSeed
int createSeed()
{
	srand((unsigned int)time(NULL));
	return rand();
}
const int _worldSeed = createSeed();

World::World(GameEngine* gameEngine, GameState* gameState)
{
	//Set Game Engine and State
	_gameEngine = gameEngine;
	_gameState = gameState;

	//Set Window Size
	SDL_GetWindowSize(_gameEngine->getWindow(), &windowSize[0], &windowSize[1]);

	//View Camera
	int xy[2] = { -(windowSize[0] / 2), (windowSize[1] / 2) };
	camPtr = new Camera(xy, windowSize[0], windowSize[1]);

	//Primary Systems
	_inputManager = _gameEngine->getInputManager();
	_resourceManager = _gameEngine->getResManager();
	_menuManager = new InGameMenuManager();
	_worldChunkManager = new WorldChunkManager(_resourceManager, windowSize[0], windowSize[1], _worldSeed);
	_entityManager = new EntityManager(_resourceManager, _inputManager, _worldChunkManager, camPtr);

	//Menus
	_inventoryMenu = new InventoryMenu(gameState, (windowSize[0] / 2) - 600, (windowSize[1] / 2) - 250);
	_menuManager->addMenu(_inventoryMenu);
}

World::~World()
{
	delete _menuManager;

	delete _worldChunkManager; //TODO saving
	delete _entityManager;
	delete camPtr; //TODO deallocation
}

void World::closeAllInGameMenus()
{
	_menuManager->closeAll();
}

void World::handleEvents()
{
	//TEMP
	if (_inputManager->getMouse()->isLeftPressed())
	{
		Mouse* m = _inputManager->getMouse();
		int xy[2] = { 0, 0 };
		m->getMouseXY(xy);

		int viewPortXY[2] = { 0, 0 };
		camPtr->getViewXY(viewPortXY);

		xy[0] += viewPortXY[0];
		xy[1] = (-xy[1]) + viewPortXY[1];

		_entityManager->createItemEntity(xy[0], xy[1], BLOCK, Rock_def);
		_entityManager->createItemEntity(xy[0], xy[1], BLOCK, Dirt_def);
	}
	_menuManager->handleEvents();
	_entityManager->handleEvents();
}

int c = 0;
void World::update()
{ 
	if (_worldChunkManager->isInitialized())
	{
		//
		//GAME RUNNING
		//

		//Update Entity Manger
		_entityManager->update();
		if (c != _entityManager->getActiveEntityCount())
		{
			std::cout << _entityManager->getActiveEntityCount() << std::endl;
			
			c = _entityManager->getActiveEntityCount();
		}

		//Adjust World Chunks
		int viewXY[2] = { 0, 0 };
		camPtr->getViewXY(viewXY);

		viewXY[0] += (windowSize[0] / 2);
		viewXY[1] -= (windowSize[1] / 2);

		_worldChunkManager->adjustWorldChunksByXY(viewXY[0], viewXY[1]);
		_worldChunkManager->update();

		_menuManager->update();
	}
	else
	{
		//
		//LOADING
		//
		_worldChunkManager->initGenerationStep();

		//Set Running Loop Speeds
		if (_worldChunkManager->isInitialized())
		{
			_gameState->setUpdatesPerSecond(RUNNING_UPS);
			_gameState->setFramesPerSecond(RUNNING_FPS);

			_entityManager->initialize();

			//Initialize menus
			Entity* player = _entityManager->getPlayer();
			_inventoryMenu->init(player->getPlayer(), player->getItemContainer());
		}
	}
}

void World::draw(SDL_Renderer* renderer,float interpolation, bool paused)
{
	float _interpolation = interpolation;

	if (paused)
		_interpolation = 1;

	if (_worldChunkManager->isInitialized())
	{
		//GAME RUNNING
		_entityManager->updateCamera(_interpolation);

		//BACK GROUND
		_resourceManager->drawBackground();

		//DRAW CHUNKS
		_worldChunkManager->draw(renderer, _interpolation, camPtr);

		//Entity Draw
		_entityManager->draw(renderer, _interpolation);

		//Draw Menus
		_menuManager->draw();
	}
	else
	{
		//LOADING
		_worldChunkManager->draw(renderer, _interpolation, NULL);
	}
}
