#include "WorldChunkManager.h"

//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------WORLD MEMORY MANAGER----------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|

SphereicalIndex::SphereicalIndex(int width, int height)
{
	_xindex = 0;
	_yindex = 0;
	_w = width;
	_h = height;
}

SphereicalIndex::SphereicalIndex(SphereicalIndex &si)
{
	_xindex = si.getXIndex();
	_yindex = si.getYIndex();
	_w = si.getWidth();
	_h = si.getHeight();
}

void SphereicalIndex::IncX(unsigned int n)
{
	_xindex = (_xindex + n) % _w;
}
void SphereicalIndex::DecX(unsigned int n)
{
	_xindex -= n;
	while(_xindex < 0)
		_xindex += _w;
}
void SphereicalIndex::IncY(unsigned int n)
{
	_yindex = (_yindex + n) % _h;
}
void SphereicalIndex::DecY(unsigned int n)
{
	_yindex -= n;
	while(_yindex < 0)
		_yindex += _h;
}

WorldChunkManager::WorldChunkManager(ResourceManager* resourceManager, int windowSizeW, int windowSizeH, int worldSeed) : _worldIndex(worldDimW, worldDimH)
{
	_resourceManager = resourceManager;
	_worldSeed = worldSeed;

	//General Var
	_windowSizeW = windowSizeW;
	_windowSizeH = windowSizeH;

	//Init Var
	initialized = false;
	initProg = 0;

	//WorldArray
	for (int ry = 0; ry < worldDimH; ry++)
	{
		for (int rx = 0; rx < worldDimW; rx++)
		{
			_worldArray[ry][rx] = nullptr;
		}
	}

	//Chunk Map 
	key_coord[0] = (-worldDimW / 2);
	key_coord[1] = (worldDimH / 2);

	//Chunk Generation
	chunkGenerating = NULL;
	chunkGenQueue = new ChunkGenerationQueue(this);

	//Chunk Cache
	_chunkCacheManager = new ChunkCacheManager();

}
WorldChunkManager::~WorldChunkManager()
{
	//TODO
	delete chunkGenQueue;
	delete _chunkCacheManager;
}

bool WorldChunkManager::isInitialized()
{
	return initialized;
}

//MAIN FUNCTIONS
void WorldChunkManager::initGenerationStep()
{
	if (initProg < (worldDimW*worldDimH))
	{
		int ry = initProg / worldDimW;
		int rx = initProg % worldDimW;
		
		Chunk* chunk = new Chunk(this, _resourceManager, key_coord[0] + rx, key_coord[1] - ry);

		//Chunk Generation
		if (!(chunk->isGenerated()))
		{
			chunk->generate(_worldSeed, 256);
		}

		if (chunk->isGenerated())
			initProg++;

		_worldArray[ry][rx] = chunk;
	}
	else
		initialized = true;
}

void WorldChunkManager::draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr)
{
	if (initialized)
	{
		//GAME RUNNING

		for (int ry = 0; ry < worldDimH; ry++)
		{
			for (int rx = 0; rx < worldDimW; rx++)
			{
				Chunk* chunk = _worldArray[ry][rx];
				if (chunk != nullptr && chunk->inView(camPtr))
				{
					chunk->draw(renderer, interpolation, camPtr);
				}
			}
		}
	}
	else
	{
		//LOADING
		int centerXY[2] = { _windowSizeW / 2, _windowSizeH / 2 };

		//Loading bar background

		SDL_Rect back = { centerXY[0] - _windowSizeW / 4, centerXY[1] - 50, centerXY[0], 100 };

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
		SDL_RenderFillRect(renderer, &back);

		//Loading bar
		SDL_Rect bar = { centerXY[0] - (_windowSizeW / 4) + 5, centerXY[1] - 45, (int)((centerXY[0] - 10)*(initProg / (double)(worldDimH*worldDimW))), 90 };

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //WHITE
		SDL_RenderFillRect(renderer, &bar);
	}
}

void WorldChunkManager::update()
{
	//Update Queue
	chunkGenQueue->update();

	//Chunk Generation
	if (chunkGenerating == nullptr)
	{
		//Retrive Next Chunk
		if (chunkGenQueue->getSize() != 0)
		{
			QueueNode* nextNode = chunkGenQueue->getQueue()[chunkGenQueue->getSize() - 1];
			if (nextNode != nullptr)
			{
				Chunk* chunk = nextNode->_data;

				int ch_xy[2] = { 0, 0 };
				chunk->getCh_XY(ch_xy);

				//Checking Chunk Cache
				int chunkCachePos = _chunkCacheManager->getChunkCachePos(ch_xy[0], ch_xy[1]);
				if (chunkCachePos == -1)
				{
					//Generating Chunk
					chunkGenerating = chunk;
				}
				else
				{
					//Reading Chunk From Cache
					if (_chunkCacheManager->readChunkCache(chunk, chunkCachePos) == 0)
						chunkGenQueue->removeChunkNode(chunk->getID());
				}
			}
		}
	}

	if (chunkGenerating != nullptr)
	{
		//Generate
		if (!(chunkGenerating->isGenerated()))
		{
			chunkGenerating->generate(_worldSeed, 128);
		}

		//Generation Finished
		if (chunkGenerating->isGenerated())
		{
			chunkGenQueue->removeChunkNode(chunkGenerating->getID());
			chunkGenerating = nullptr;
		}
	}


	//Update Tiles
	for (int r_y = 0; r_y < worldDimH; r_y++)
	{
		for (int r_x = 0; r_x < worldDimW; r_x++)
		{
			Chunk* chunk = getChunkAtRelativeXY(r_x, r_y);
			if (chunk != nullptr && chunk->isGenerated())
				chunk->update();
		}
	}
}

void WorldChunkManager::getWindowSize(int* dest)
{
	dest[0] = _windowSizeW;
	dest[1] = _windowSizeH;
}

//RETRIEVAL FUNCTIONS
void WorldChunkManager::getChXY(int x, int y, int* dest)
{
	dest[0] = (int)floor(x / (float)(CHUNK_Wi * TILE_Wi));
	dest[1] = (int)ceil(y / (float)(CHUNK_He * TILE_He));
}

void WorldChunkManager::getCenterChXY(int* dest)
{
	dest[0] = key_coord[0] + (worldDimW / 2);
	dest[1] = key_coord[1] - (worldDimH / 2);
}

ChunkCacheManager* WorldChunkManager::getChunkCacheManager()
{
	return _chunkCacheManager;
}

Chunk* WorldChunkManager::getChunkAtChXY(int ch_x, int ch_y)
{
	if (inWorld(ch_x, ch_y))
	{
		return getChunkAtRelativeXY(ch_x - key_coord[0], key_coord[1] - ch_y);
	}

	return nullptr;
}

Chunk* WorldChunkManager::getChunkAtRelativeXY(unsigned int r_x, unsigned int r_y)
{
	if (r_x >= 0 && r_x < worldDimW && r_y >= 0 && r_y < worldDimH)
	{
		SphereicalIndex si(_worldIndex);
		si.IncX(r_x);
		si.IncY(r_y);
		Chunk* chunk = _worldArray[si.getYIndex()][si.getXIndex()];

		if (chunk != nullptr && chunk->isGenerated())
			return chunk;
	}

	return nullptr;
}

Chunk* WorldChunkManager::getChunkAtXY(int x, int y)
{
	int Ch_XY[2] = { 0, 0 };
	getChXY(x, y, Ch_XY);
	return getChunkAtChXY(Ch_XY[0], Ch_XY[1]);
}

Tile* WorldChunkManager::getTileAtXY(int x, int y)
{
	Chunk* chunk = getChunkAtXY(x, y);
	Tile* tile = chunk->getTileAt_XY(x, y);

	return tile;
}

void WorldChunkManager::getTilesInRect(int x, int y, int w, int h, std::list<Tile*> &tileList)
{
	int ch_xy1[2] = { 0, 0 };
	getChXY(x, y, ch_xy1);

	int ch_xy2[2] = { 0, 0 };
	getChXY(x + w, y - h, ch_xy2);

	for (int ch_y = ch_xy1[1]; ch_y >= ch_xy2[1]; ch_y--)
	{
		for (int ch_x = ch_xy1[0]; ch_x <= ch_xy2[0]; ch_x++)
		{
			Chunk* chunk = getChunkAtChXY(ch_x, ch_y);
			if (chunk != nullptr)
			{
				chunk->getTilesInRect(x, y, w, h, tileList);
			}
		}
	}
}

//Check Functions
bool WorldChunkManager::inWorld(int ch_x, int ch_y)
{
	return ((ch_x >= key_coord[0] && ch_x < (key_coord[0] + worldDimW)) && (ch_y <= key_coord[1] && ch_y >(key_coord[1] - worldDimH)));
}

//MOTION FUNCTIONS
void WorldChunkManager::adjustWorldChunksByXY(int x, int y)
{
	bool shifted = false;
	//Get Chunk Pos
	int Ch_XY[2] = { 0, 0 };
	getChXY(x, y, Ch_XY);

	int worldCh_XY[2] = { 0, 0 };
	getCenterChXY(worldCh_XY);

	//Shift Right Cond.
	if ((!shifted) && (Ch_XY[0] > worldCh_XY[0] + LOAD_BUFFER))
	{
		shiftRight();
		shifted = true;
	}

	//Shift Left Cond.
	if ((!shifted) && (Ch_XY[0] < worldCh_XY[0] - LOAD_BUFFER))
	{
		shiftLeft();
		shifted = true;
	}

	//Shift Up Cond.
	if ((!shifted) && (Ch_XY[1] > worldCh_XY[1] + LOAD_BUFFER))
	{
		shiftUp();
		shifted = true;
	}

	//Shift D Cond.
	if ((!shifted) && (Ch_XY[1] < worldCh_XY[1] - LOAD_BUFFER))
	{
		shiftDown();
		shifted = true;
	}
}

void WorldChunkManager::shiftRight()
{
	key_coord[0]++;

	SphereicalIndex si(_worldIndex);
	for (int ry = 0; ry < worldDimH; ry++)
	{
		Chunk* chunk = _worldArray[si.getYIndex()][si.getXIndex()];

		//Remove From Gen Queue
		if (!chunk->isGenerated())
			chunkGenQueue->removeChunkNode(chunk->getID());

		//Delete
		delete chunk;
		chunk = nullptr;
		_worldArray[si.getYIndex()][si.getXIndex()] = nullptr;

		//Add New
		chunk = new Chunk(this, _resourceManager, key_coord[0] + (worldDimW - 1), key_coord[1] - ry);
		_worldArray[si.getYIndex()][si.getXIndex()] = chunk;
		chunkGenQueue->addChunk(chunk);

		si.IncY(1);
	}

	_worldIndex.IncX(1);
}
void WorldChunkManager::shiftLeft()
{
	key_coord[0]--;

	SphereicalIndex si(_worldIndex);
	si.IncX(worldDimW - 1);
	for (int ry = 0; ry < worldDimH; ry++)
	{
		Chunk* chunk = _worldArray[si.getYIndex()][si.getXIndex()];

		//Remove From Gen Queue
		if (!chunk->isGenerated())
			chunkGenQueue->removeChunkNode(chunk->getID());

		//Delete
		delete chunk;
		chunk = nullptr;
		_worldArray[si.getYIndex()][si.getXIndex()] = nullptr;

		//Add New
		chunk = new Chunk(this, _resourceManager, key_coord[0], key_coord[1] - ry);
		_worldArray[si.getYIndex()][si.getXIndex()] = chunk;
		chunkGenQueue->addChunk(chunk);

		si.IncY(1);
	}

	_worldIndex.DecX(1);
}
void WorldChunkManager::shiftUp()
{
	key_coord[1]++;

	SphereicalIndex si(_worldIndex);
	si.IncY(worldDimH - 1);
	for (int rx = 0; rx < worldDimH; rx++)
	{
		Chunk* chunk = _worldArray[si.getYIndex()][si.getXIndex()];

		//Remove From Gen Queue
		if (!chunk->isGenerated())
			chunkGenQueue->removeChunkNode(chunk->getID());

		//Delete
		delete chunk;
		chunk = nullptr;
		_worldArray[si.getYIndex()][si.getXIndex()] = nullptr;

		//Add New
		chunk = new Chunk(this, _resourceManager, key_coord[0] + rx, key_coord[1]);
		_worldArray[si.getYIndex()][si.getXIndex()] = chunk;
		chunkGenQueue->addChunk(chunk);

		si.IncX(1);
	}
	_worldIndex.DecY(1);
}
void WorldChunkManager::shiftDown()
{
	key_coord[1]--;

	SphereicalIndex si(_worldIndex);
	for (int rx = 0; rx < worldDimH; rx++)
	{
		Chunk* chunk = _worldArray[si.getYIndex()][si.getXIndex()];

		//Remove From Gen Queue
		if (!chunk->isGenerated())
			chunkGenQueue->removeChunkNode(chunk->getID());

		//Delete
		delete chunk;
		chunk = nullptr;
		_worldArray[si.getYIndex()][si.getXIndex()] = nullptr;

		//Add New
		chunk = new Chunk(this, _resourceManager, key_coord[0] + rx, key_coord[1] - (worldDimH - 1));
		_worldArray[si.getYIndex()][si.getXIndex()] = chunk;
		chunkGenQueue->addChunk(chunk);

		si.IncX(1);
	}
	_worldIndex.IncY(1);
}


