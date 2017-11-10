#ifndef WMM_H
#define WMM_H

#include <map>
#include <queue>
#include <list>
#include <iostream>

#include "SDL.h"
#include "ChunkCacheManager.h"
#include "ChunkGenerationQueue.h"
#include "ResourceManager.h"
#include "Tile.h"
#include "Chunk.h"

class Chunk;
class ChunkGenerationQueue;
class ResourceManager;
class Tile;
class Camera;
class ChunkCacheManager;

struct QueueNode;

const int worldDimW = 16;
const int worldDimH = 12;
const int LOAD_BUFFER = 1;

class SphereicalIndex
{
private:
	int _xindex;
	int _yindex;
	int _w;
	int _h;
public:
	SphereicalIndex(int width, int height);
	SphereicalIndex(SphereicalIndex &si);

	void IncX(unsigned int n);
	void DecX(unsigned int n);
	void IncY(unsigned int n);
	void DecY(unsigned int n);

	int getXIndex() { return _xindex; }
	int getYIndex() { return _yindex; }
	int getWidth() { return _w; }
	int getHeight() { return _h; }
};

class WorldChunkManager
{
private:
	ResourceManager* _resourceManager;
	int _worldSeed;

	//General Var
	int _windowSizeW;
	int _windowSizeH;

	//Init Var
	bool initialized;
	int initProg;

	//Chunk Map
	int key_coord[2];

	//WorldArray
	Chunk* _worldArray[worldDimH][worldDimW];
	SphereicalIndex _worldIndex;

	//Map Motion
	void shiftRight();
	void shiftLeft();
	void shiftUp();
	void shiftDown();

	//Chunk Generation Queue
	Chunk* chunkGenerating;
	ChunkGenerationQueue* chunkGenQueue;

	//Chunk Cache
	ChunkCacheManager* _chunkCacheManager;
public:
	WorldChunkManager(ResourceManager* resourceManager, int windowSizeW, int windowSizeH, int worldSeed);
	~WorldChunkManager();

	bool isInitialized();
	void initGenerationStep();

	//Main
	void draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr);
	void update();

	void getWindowSize(int* dest);

	//Retrieval--------------------------
	void getChXY(int x, int y, int* dest);
	void getCenterChXY(int* dest);

	//Chunk
	ChunkCacheManager* getChunkCacheManager();
	Chunk* getChunkAtChXY(int ch_x, int ch_y);
	Chunk* getChunkAtRelativeXY(unsigned int r_x, unsigned int r_y);
	Chunk* getChunkAtXY(int x, int y);

	//Tile
	Tile* getTileAtXY(int x, int y);
	void getTilesInRect(int x, int y, int w, int h, std::list<Tile*> &tileList);

	//Check
	bool inWorld(int ch_x, int ch_y);

	//Motion
	void adjustWorldChunksByXY(int x, int y);
};

//-----------------------------------------------------

#endif