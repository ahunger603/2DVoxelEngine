#include "ChunkGenerationQueue.h"
#include <iostream>

ChunkGenerationQueue::ChunkGenerationQueue(WorldChunkManager* worldChunkManager)
{
	_worldChunkManager = worldChunkManager;
	_worldChunkManager->getCenterChXY(_worldCenterChXY);

	size = 0;
	for (int i = 0; i < QUEUE_SIZE; i++)
	{
		queue[i] = nullptr;
	}
}

//Private Functions
void ChunkGenerationQueue::merge(int left, int mid, int right)
{
	int i, left_end, num_elements, tmp_pos;

	left_end = (mid - 1);
	tmp_pos = left;
	num_elements = (right - left + 1);
	QueueNode** temp = new QueueNode*[size];

	while ((left <= left_end) && (mid <= right))
	{
		if (queue[left]->_manhattanDist > queue[mid]->_manhattanDist)
			temp[tmp_pos++] = queue[left++];
		else
			temp[tmp_pos++] = queue[mid++];
	}

	while (left <= left_end)
		temp[tmp_pos++] = queue[left++];

	while (mid <= right)
		temp[tmp_pos++] = queue[mid++];

	for (i = 0; i < num_elements; i++)
		queue[right--] = temp[right];

	delete temp;
}

void ChunkGenerationQueue::mergeSort(int left, int right)
{
	if (right > left)
	{
		int mid = (right + left) / 2;
		mergeSort(left, mid);
		mergeSort((mid + 1), right);

		merge(left, (mid + 1), right);
	}
}

void ChunkGenerationQueue::sort()
{
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			queue[i]->_manhattanDist = getManhattanDistance(queue[i]->_data);
		}
		mergeSort(0, size - 1);
	}
}

void ChunkGenerationQueue::addQueueNode(QueueNode* node)
{
	if (size < QUEUE_SIZE)
	{
		queue[size] = node;
		size++;
	}
	else
		std::cout << "QUEUE OVERLOAD" << std::endl;
}

int ChunkGenerationQueue::getManhattanDistance(int ch_x, int ch_y)
{
	return (abs(ch_x - _worldCenterChXY[0]) + abs(ch_y - _worldCenterChXY[1]));
}


int ChunkGenerationQueue::getManhattanDistance(Chunk* chunk)
{
	int ch_xy[2] = { 0, 0 };
	chunk->getCh_XY(ch_xy);

	return getManhattanDistance(ch_xy[0], ch_xy[1]);
}

//Interface
void ChunkGenerationQueue::update()
{
	int worldCenterChXY[2];
	_worldChunkManager->getCenterChXY(worldCenterChXY);

	if (worldCenterChXY[0] != _worldCenterChXY[0] || worldCenterChXY[1] != worldCenterChXY[1])
	{
		//Resort
		sort();

		//Set Center
		_worldCenterChXY[0] = worldCenterChXY[0];
		_worldCenterChXY[1] = worldCenterChXY[1];
	}
}

int ChunkGenerationQueue::getSize()
{
	return size;
}

void ChunkGenerationQueue::removeChunkNode(int chunkID)
{
	for (int i = 0; i < size; i++)
	{
		QueueNode* node = queue[i];
		if (node->_data->getID() == chunkID)
		{
			queue[i] = nullptr;
			for (int k = i; k < size && k < (QUEUE_SIZE - 1); k++)
			{
				queue[k] = queue[k + 1];
			}
			queue[size - 1] = nullptr;

			delete node;

			size--;
		}
	}
}

void ChunkGenerationQueue::addChunk(Chunk* chunk)
{
	int manhattanDist = getManhattanDistance(chunk);
	QueueNode* node = new QueueNode(chunk, manhattanDist);

	addQueueNode(node);
}

QueueNode** ChunkGenerationQueue::getQueue()
{
	return queue;
}