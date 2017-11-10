#include "EntityManager.h"

using namespace std;

EntityManager::EntityManager(ResourceManager* resourceManager, InputManager* inputManager, WorldChunkManager* worldChunkManager, Camera* viewPort)
{
	activeEntities = 0;
	initialized = false;

	_resourceManager = resourceManager;
	_inputManager = inputManager;
	_worldChunkManager = worldChunkManager;

	_phyEngine = new PhysicsEngine(this, _worldChunkManager);

	_viewPort = viewPort;

	player = NULL;
}

EntityManager::~EntityManager()
{
	delete _phyEngine;

	for (auto it = entityMap.begin(); it != entityMap.end(); it++)
	{
		map<int, Entity*>* chunkEntityMap = it->second;

		for (auto itt = chunkEntityMap->begin(); itt != chunkEntityMap->end(); itt++)
		{
			Entity* entity = itt->second;
			if (entity != nullptr)
			{
					delete entity;
			}
			chunkEntityMap->at(itt->first) = nullptr;
		}

		entityMap.erase(it++);

		delete chunkEntityMap;
	}

}

void EntityManager::addEntity(Entity* entity)
{
	float xy[2] = { 0, 0 };
	entity->getXY(xy);

	int ch_xy[2] = { 0, 0 };
	_worldChunkManager->getChXY((int)xy[0], (int)xy[1], ch_xy);

	pair<int, int> ch_xy_pair(ch_xy[0], ch_xy[1]);
	try
	{
		//Found Chunk Entity Map
		map<int, Entity*>* chunkEntityMap = entityMap.at(ch_xy_pair);
		
		pair<int, Entity*> entityEntry(entity->getID(), entity);

		chunkEntityMap->insert(entityEntry);
	}
	catch (out_of_range)
	{
		//Creating Chunk Entity Map
		map<int, Entity*>* chunkEntityMap = new map<int, Entity*>;

		//Add Entity Entry
		pair<int, Entity*> entityEntry(entity->getID(), entity);

		chunkEntityMap->insert(entityEntry);

		//Add Chunk Entity Map
		pair<pair<int, int>, map<int, Entity*>*> chunkEntityMapEntry(ch_xy_pair, chunkEntityMap);

		entityMap.insert(chunkEntityMapEntry);
	}
	activeEntities++;
}

void EntityManager::createPlayer(float x, float y)
{
	int mask = (COMPONENT_PLAYER | COMPONENT_DYNAMIC | COMPONENT_PHYSICS | COMPONENT_AABB | COMPONENT_SPRITE | COMPONENT_PERSISTANCE | COMPONENT_CONTAINER);

	Player* player_ = new Player();
	Dynamic* dynamic = new Dynamic();
	Physics* phys = new Physics(1, 1, 50, 0);
	AABB* aabb = new AABB(12, 16);
	ItemContainer* inv = new ItemContainer(10, 5);

	player = new Entity(mask, x, y, player_, dynamic, phys, aabb, nullptr, inv);

	addEntity(player);
}

int EntityManager::getActiveEntityCount()
{
	return activeEntities;
}

Entity* EntityManager::getPlayer()
{
	return player;
}

void EntityManager::updatePlayer()
{
	Player* playerComponent = player->getPlayer();
	Physics* physComponent = player->getPhysics();

	//Action
	playerComponent->selectTile();

	//Movement
	if (physComponent->collisions[0])
	{
		if (playerComponent->_intentMoveLeft)
		{
			physComponent->forces->push_back(Vector(-playerComponent->speed, 0));

			playerComponent->_intentMoveLeft = false;
		}
		if (playerComponent->_intentMoveRight)
		{
			physComponent->forces->push_back(Vector(playerComponent->speed, 0));

			playerComponent->_intentMoveRight = false;
		}
		if (playerComponent->_intentJump)
		{
			physComponent->forces->push_back(Vector(0, playerComponent->jumpPower));

			playerComponent->_intentJump = false;
		}
	}
}

void EntityManager::createItemEntity(float x, float y, int primaryID, int secondaryID)
{
	int mask = (COMPONENT_AABB | COMPONENT_DYNAMIC | COMPONENT_PHYSICS |  COMPONENT_SPRITE | COMPONENT_ITEM);

	Dynamic* dynamic = new Dynamic();
	Physics* phys = new Physics(1, 1, 5, 0);
	AABB* aabb = new AABB(6, 6);
	Item* item = new Item(primaryID, secondaryID, -1);

	addEntity(new Entity(mask, x, y, nullptr, dynamic, phys, aabb, item, nullptr));
}

bool EntityManager::isInventoryFullForItem(int primaryID, int secondaryID)
{
	return (player->getItemContainer()->isFullForItem(primaryID, secondaryID));
}

void EntityManager::addItemToPlayerInventory(Entity* item)
{
	if ((COMPONENT_ITEM & item->getMask()) == COMPONENT_ITEM)
	{
		if (player->getItemContainer()->addItem(Item(*item->getItem()))) //Add
			item->destroy();
	}
}

void EntityManager::initialize()
{
	createPlayer(0, 300);
	initialized = true;
}

void EntityManager::handleEvents()
{
	//Player----------------------------------
	if (initialized)
	{
		if (_inputManager->isKeyReleased(KEY_S))
		{
			player->getItemContainer()->printContainer();
		}
		handlePlayerEvents();
	}
}

void EntityManager::handlePlayerEvents()
{
	//Keyboard
	if (player->getPhysics()->collisions[0])
	{
		//W
		if (_inputManager->getKeyState(KEY_W))
		{
			player->getPlayer()->_intentJump = true;
		}

		//A
		if (_inputManager->getKeyState(KEY_A))
		{
			player->getPlayer()->_intentMoveLeft = true;
		}
		else
		{
			//D
			if (_inputManager->getKeyState(KEY_D))
			{
				player->getPlayer()->_intentMoveRight = true;
			}
		}
	}

	//Mouse
	Mouse* mouse = _inputManager->getMouse();
	if (!mouse->isUsed())
	{
		//Pressed
		if (mouse->isLeftPressed())
		{

		}

		if (mouse->isRightPressed())
		{

		}

		//Hold
		if (mouse->isLeftDown())
		{
			int mouseXY[2] = { 0, 0 };
			mouse->getMouseXY(mouseXY);

			int viewPortXY[2] = { 0, 0 };
			_viewPort->getViewXY(viewPortXY);

			mouseXY[0] += viewPortXY[0];
			mouseXY[1] = (-mouseXY[1]) + viewPortXY[1];

			Tile* tile = _worldChunkManager->getTileAtXY(mouseXY[0], mouseXY[1]);

			player->getPlayer()->intentSelectTile(tile);
		}
		else
		{
			if (mouse->isRightDown())
			{

			}
		}

		mouse->Used();
	}
}

void EntityManager::update()
{
	//Update Player
	updatePlayer();

	//Update Entities
	for (auto entityMapIt = entityMap.begin(); entityMapIt != entityMap.end(); entityMapIt++)
	{
		pair<int, int> mapChXY = (entityMapIt->first);
		std::map<int, Entity*>* entityChunk = (entityMapIt->second);
		if (_worldChunkManager->inWorld(mapChXY.first, mapChXY.second))
		{
			if (entityChunk->size() != 0)
			{
				//Update Entity Chunk
				for (auto entityChIt = entityChunk->begin(); entityChIt != entityChunk->end(); entityChIt++)
				{
					Entity* entity = entityChIt->second;

					//Update Physics
					_phyEngine->updateEntity(entity);

					//Update Entity Map Pos
					if (entity->exists())
					{
						float entityXY[2] = { 0, 0 };
						entity->getXY(entityXY);

						int entityChXY[2] = { 0, 0 };
						_worldChunkManager->getChXY(entityXY[0], entityXY[1], entityChXY);

						int wcChXY[2] = { 0, 0 };
						_worldChunkManager->getCenterChXY(wcChXY);
						if (mapChXY.first != entityChXY[0] || mapChXY.second != entityChXY[1])
						{
							entityChunk->erase(entityChIt++);
							addEntity(entity);

							activeEntities--;
						}
					}
					else
					{
						//Destroy Non-existant
						entityChunk->erase(entityChIt++);
						delete entity;
						
						activeEntities--;
					}
				}
			}
		}
		else
		{
			//Destroy Entity Chunk
			if (entityChunk->size() != 0)
			{
				for (auto entityChIt = entityChunk->begin(); entityChIt != entityChunk->end(); entityChIt++)
				{
					Entity* entity = (entityChIt->second);
					//Destroy Non-Persistant Entites
					if ((COMPONENT_PERSISTANCE & entity->getMask()) != COMPONENT_PERSISTANCE)
					{
						delete entityChIt->second;
						activeEntities--;
					}
					else
					{
						//TODO Save Persistant
					}
				}
			}

			delete entityChunk;

			//Remove From Map
			entityMap.erase(entityMapIt++);
		}
	}

	//Set Camera
	float playerCenterXY[2] = { 0, 0 };
	player->getCenterXY(playerCenterXY);

	int center[2] = { (int)playerCenterXY[0], (int)playerCenterXY[1] };
	_viewPort->setPosByCenter(center);
}

void EntityManager::updateCamera(float interpolation)
{
	float playerInterPos[2] = { 0, 0 };
	player->getInterpolatedXY(interpolation, playerInterPos);
	int playerCenterXY[2] = { playerInterPos[0], playerInterPos[1] };

	_viewPort->setPosByCenter(playerCenterXY);
}

void EntityManager::draw(SDL_Renderer* renderer, float interpolation)
{
	//Draw Entities
	for (auto it = entityMap.begin(); it != entityMap.end(); it++)
	{
		std::pair<int, int> chunkXY = it->first;
		for (auto itt = (it->second)->begin(); itt != (it->second)->end(); itt++)
		{
			Entity* entity = itt->second;
			//Draw Entity
			if (((COMPONENT_AABB | COMPONENT_SPRITE) & entity->getMask()) == (COMPONENT_AABB | COMPONENT_SPRITE))
			{
				float interXY[2] = { 0, 0 };
				entity->getInterpolatedXY(interpolation, interXY);

				int xx = _viewPort->getDrawX(interXY[0]);
				int yy = _viewPort->getDrawY(interXY[1]);

				SDL_Rect er = { xx, yy, entity->getAABB()->_w, entity->getAABB()->_h };

				if (((COMPONENT_ITEM)& entity->getMask()) == (COMPONENT_ITEM))
				{
					const int* color = TILE_RGBA_COLORS.at((tile_def)(entity->getItem()->_secondaryID));
					SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]); //WHITE
					SDL_RenderFillRect(renderer, &er);
				}
				else
				{
					SDL_SetRenderDrawColor(renderer, 137, 137, 137, 255); //WHITE
					SDL_RenderFillRect(renderer, &er);
				}
			}
		}
	}
}