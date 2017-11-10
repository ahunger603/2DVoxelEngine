#ifndef CHUNKGENQUEUE_H
#define CHUNKGENQUEUE_H

#include "Chunk.h"
#include "WorldChunkManager.h"

class Chunk;
class WorldChunkManager;

struct QueueNode
{
	Chunk* _data;
	int _manhattanDist;

	QueueNode(Chunk* chunk, int manhattanDist)
	{
		_data = chunk;
		_manhattanDist = manhattanDist;
	}
};

const int QUEUE_SIZE = 180;
class ChunkGenerationQueue
{
private:
	WorldChunkManager* _worldChunkManager;
	int _worldCenterChXY[2];

	int size;
	QueueNode* queue[QUEUE_SIZE];
	
	//Private Functions
	void merge(int left, int mid, int right);
	void mergeSort(int left, int right);
	void sort();
	void addQueueNode(QueueNode* node);

	int getManhattanDistance(Chunk* chunk);
	int getManhattanDistance(int ch_x, int ch_y);
public:
	ChunkGenerationQueue(WorldChunkManager* worldChunkManager);

	//Interface
	void update();

	void removeChunkNode(int chunkID);
	void addChunk(Chunk* chunk);

	QueueNode** getQueue();
	int getSize();
};


#endif