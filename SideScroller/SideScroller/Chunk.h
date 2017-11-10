#ifndef CHUNK_H
#define CHUNK_H

#include <list>
#include <cmath>
#include <iostream>

#include "Noise.h"
#include "Tile.h"
#include "WorldChunkManager.h"
#include "ResourceManager.h"
#include "ChunkCacheManager.h"
#include "Camera.h"

class Tile;
class Camera;
class WorldChunkManager;
class ChunkCacheManager;
class ResourceManager;

const int CHUNK_Wi = 16;
const int CHUNK_He = 16;

class Chunk
{
	friend Tile;
	friend ChunkCacheManager;
private:
	WorldChunkManager* _worldChunkManager;
	ResourceManager* _resourceManager;

	//bool unique;
	bool changed;
	int id;
	int _ch_x;
	int _ch_y;
	int _x;
	int _y;

	int genProg;
	bool generated;

	//Interactive / Deactivation Var
	bool interactive;
	bool deactivating;
	void setInteractive();
	uint32_t ticks_sinceInteractive;

	SDL_Rect chunkArea;
	Tile* _chunk[CHUNK_Wi][CHUNK_He];
public:
	Chunk(WorldChunkManager* worldChunkManager, ResourceManager* resourceManager, int ch_x, int ch_y);
	~Chunk();

	static int chunkCnt;

	WorldChunkManager* getWorldChunkManager();
	ResourceManager* getResourceManager();

	int getID();

	void updateEdgeTileSituations(int side);//0 - right, 1 - top, 2 - left, 3 - bottom

	//Position Functions
	void getXY(int* dest);
	void getCh_XY(int* dest);
	void getTileXY(int ch_gr_x, int ch_gr_y, int* dest);

	bool inView(Camera* camPtr);
	bool inRect(int x, int y, int w, int h);
	bool isGenerated();
	
	//Chunk Functions
	Chunk* getChunkByRelativeChXY(int rch_x, int rch_y);
	
	//Tile Functions
	void getTilesInRect(int x, int y, int w, int h, std::list<Tile*> &tileList);
	Tile* getTileAt_XY(int x, int y);
	Tile* getTileAt_ChGrXY(int ch_gr_x, int ch_gr_y);

	//Generation Functions
	void generate(int worldSeed, int steps);
	void generateTile(int worldSeed, int gr_x, int gr_y);

	//General Functions
	void update();
	void draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr);
};

#endif