#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>

#include "SDL.h"
#include "Vector.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "WorldChunkManager.h"
#include "Entity.h"
#include "PhysicsEngine.h"

class ResourceManager;
class InputManager;
class Camera;
class WorldChunkManager;
class PhysicsEngine;
class Entity;
class ItemContainer;

struct Dynamic;
struct AABB;
struct Item;
struct Physics;

class EntityManager
{
private:
	int activeEntities;
	bool initialized;

	ResourceManager* _resourceManager;
	InputManager* _inputManager;
	WorldChunkManager* _worldChunkManager;
	Camera* _viewPort;

	//Advanced Systems
	PhysicsEngine* _phyEngine;

	//Player
	Entity* player;
	void createPlayer(float x, float y);
	void handlePlayerEvents();
	void updatePlayer();

	//Entity Map
	std::map<std::pair<int, int>, std::map<int, Entity*>*> entityMap;

	void addEntity(Entity* entity);
public:
	EntityManager(ResourceManager* resourceManager, InputManager* inputManager, WorldChunkManager* worldChunkManager, Camera* viewPort);
	~EntityManager();

	//Retrieval
	int getActiveEntityCount();
	Entity* getPlayer();

	//Entity Creation
	void createItemEntity(float x, float y, int primaryID, int secondaryID);

	//Player Functions
	bool isInventoryFullForItem(int primaryID, int secondaryID);
	void addItemToPlayerInventory(Entity* item);

	void initialize();
	void handleEvents();
	void update();
	void updateCamera(float interpolation);
	void draw(SDL_Renderer* renderer, float interpolation);
};

#endif