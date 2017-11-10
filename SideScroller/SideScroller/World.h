#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <unordered_map>
#include <ctime>
#include <map>

#include "SDL.h"
#include "GameEngine.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "WorldChunkManager.h"
#include "EntityManager.h"
#include "InventoryMenu.h"
#include "InGameMenuManager.h"

class GameEngine;
class GameState;
class ResourceManager;
class InputManager;
class Chunk;
class WorldChunkManager;
class EntityManager;
class Character;
class InGameMenuManager;

class World
{
private:
	GameEngine* _gameEngine;
	GameState* _gameState;

	Camera* camPtr;

	//Primary Systems
	InputManager* _inputManager;
	ResourceManager* _resourceManager;
	InGameMenuManager* _menuManager;
	WorldChunkManager* _worldChunkManager;
	EntityManager* _entityManager;

	//Core Menus
	InventoryMenu* _inventoryMenu;
public:
	static const int LOADING_UPS;
	static const int LOADING_FPS;
	static const int RUNNING_UPS;
	static const int RUNNING_FPS;

	int windowSize[2];

	//Construct/Destructor
	World(GameEngine* gameEngine, GameState* gameState);
	~World();

	void closeAllInGameMenus();

	//Core Events
	void handleEvents();
	void update();
	void draw(SDL_Renderer* renderer, float interpolation, bool paused);
};

#endif
