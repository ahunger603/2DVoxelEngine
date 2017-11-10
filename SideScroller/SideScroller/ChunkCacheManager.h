#ifndef CHUNK_CACHE_MNGER
#define CHUNK_CACHE_MNGER

#include <map>
#include <iostream>
#include <fstream>

#include "Tile.h"
#include "Chunk.h"

class WorldChunkManager;
class Chunk;
class Tile;
class CacheTile;

const int TICKS_TO_DEACTIVATE = 1000;

class ChunkCacheManager
{
private:
	//Chunk Record
	int cacheSize;
	std::map< std::pair<int, int>, std::map< std::pair<int, int>, int>* > _chunkRecords;

	//Chunk Cache
	std::fstream* _chunkCache;

	//Interal Cache Functinos
	int writeChunk(Chunk* chunk, int cachePos);
public:
	ChunkCacheManager();
	~ChunkCacheManager();

	int cacheChunk(Chunk* chunk);

	int getChunkCachePos(int ch_x, int ch_y);
	int readChunkCache(Chunk* chunk, int cachePos);
};

#endif