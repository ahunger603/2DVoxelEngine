#ifndef TILE_H
#define TILE_H

#include <unordered_map>

#include "Camera.h"
#include "Chunk.h"

class Chunk;
class Camera;
class InteractiveTile;
class CacheTile;

typedef enum tile_def
{
	Empty_def = 0,
	Dirt_def = 1,
	Rock_def = 2,
	Grass_def = 3
};

//Constants
const int emptyColor[4] = { NULL, NULL, NULL, NULL };
const int dirtColor[4] = { 139, 90, 43, 255 };
const int rockColor[4] = { 139, 137, 137, 255 };
const int grassColor[4] = { 63, 255, 66, 255 };
static const std::unordered_map<tile_def, const int*> TILE_RGBA_COLORS =
{
	{ Empty_def, emptyColor },
	{ Dirt_def,  dirtColor},
	{ Rock_def,  rockColor},
	{ Grass_def, grassColor}
};

static const std::unordered_map<tile_def, Uint32> TILE_HEX_COLORS =
{
	{ Empty_def, 0x000000 },
	{ Dirt_def, 0x8B5A2B },
	{ Rock_def, 0x8B8989 },
	{ Grass_def, 0x3FFF42 }
};

static const std::unordered_map<tile_def, bool> TILE_SOLIDITY =
{
	{ Empty_def, false },
	{ Dirt_def, true },
	{ Rock_def, true },
	{ Grass_def, true }
};



//Dim
const int TILE_Wi = 24;
const int TILE_He = TILE_Wi;

class Tile
{
	friend Chunk;
	friend InteractiveTile;
	friend CacheTile;
private:
	int tileVariation;
	int texturePage;
	SDL_Rect textureLocation;

	//Chunk Relative Coord.
	int _ch_gr_x;
	int _ch_gr_y;

	int _x;
	int _y;

	SDL_Rect tile;

	//Properties
	Chunk* _parentChunk;
	tile_def _def;

	InteractiveTile* interactiveTile;

	void setType(tile_def def);

public:
	Tile(int ch_gr_x, int ch_gr_y, tile_def def, Chunk* parent);

	//Other Functions
	bool isSolid();
	bool notEmpty();
	bool inRect(int x, int y, int w, int h);
	bool pointInTile(int x, int y);

	CacheTile* toCacheTile();

	//Get
	tile_def getTileDef();
	Tile* getAdjacentTile(int dir); //0 - 7

	void adjustSituation();

	//Position Functions
	void getXY(int* dest);
	void getCh_XY(int* dest);
	void getCh_Gr_XY(int* dest);

	//Interactive Functions
	void selected();
	void deselected();
	void breakTile();

	//Core Fuctions
	void update();
	void draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr);
};

//-------------------------------------

class InteractiveTile
{
private:
	Tile* _parentTile;

	int breakHits;
public:
	InteractiveTile(Tile* parentTile);

	void breakTile();
};

//-------------------------------------

class CacheTile
{
	tile_def _def;
	int _ch_gr_x;
	int _ch_gr_y;
private:
public:
	CacheTile();
	CacheTile(Tile* tile);
	CacheTile(int ch_gr_x, int ch_gr_y, tile_def def);

	Tile* toTile(Chunk* chunk);
};

#endif