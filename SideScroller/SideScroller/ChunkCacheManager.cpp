#include "ChunkCacheManager.h"

using namespace std;

ChunkCacheManager::ChunkCacheManager()
{
	cacheSize = 0;

	//Chunk Cache
	_chunkCache = new fstream("chunkCache.bin", ios::binary | ios::in | ios::out | ios::trunc);
}

ChunkCacheManager::~ChunkCacheManager()
{
	//Chunk Records
	for (auto it = _chunkRecords.begin(); it != _chunkRecords.end(); it++)
	{
		map< pair<int, int>, int>* chunkRecord = it->second;

		delete chunkRecord;
	}

	//Chunk Cache
	if (_chunkCache->is_open())
		_chunkCache->close();

	delete _chunkCache;
}

int ChunkCacheManager::writeChunk(Chunk* chunk, int cachePos)
{
	if (cachePos <= cacheSize)
	{
		try
		{
			if (!_chunkCache->is_open())
			{
				cout << "Error: Chunk Cache Is Not Open" << endl;

				return -1;
			}
			else
			{
				_chunkCache->seekp((sizeof(CacheTile)*(CHUNK_He*CHUNK_Wi))*cachePos);

				for (int ch_gr_y = 0; ch_gr_y < CHUNK_He; ch_gr_y++)
				{
					for (int ch_gr_x = 0; ch_gr_x < CHUNK_Wi; ch_gr_x++)
					{
						Tile* tile = chunk->_chunk[ch_gr_y][ch_gr_x];
						CacheTile* cacheTile = NULL;
						if (tile != NULL)
							cacheTile = tile->toCacheTile();
						else
							cacheTile = new CacheTile(ch_gr_x, ch_gr_y, Empty_def);

						if (cacheTile != NULL)
						{
							if (!_chunkCache->write((char*)cacheTile, sizeof(CacheTile)))
							{
								cout << "Error: Failed To Write" << endl;
								return -1;
							}
						}
						else
						{
							cout << "Error: Could Not Create Cache Tile" << endl;
							return -1;
						}

						delete cacheTile;
					}
				}
			}

			//Increase Cache Size if not rewrite
			if (cachePos == cacheSize)
				cacheSize++;
		}
		catch (...)
		{ 
			cout << "Error: Could Not Cache Chunk" << endl; 
			return -1; 
		}
	}
	else
	{
		cout << "Error: Attempting To Write Chunk Out of Cache Range" << endl;
		return -1;
	}
	return 0;
}

int ChunkCacheManager::cacheChunk(Chunk* chunk)
{
	int ch_xy[2] = { 0, 0 };
	chunk->getCh_XY(ch_xy);

	pair<int, int> ch_xy10sPair(ch_xy[0] / 10, ch_xy[1] / 10);
	pair<int, int> ch_xyMod10Pair(ch_xy[0] % 10, ch_xy[1] % 10);

	try
	{
		//Chunk Record Found
		map< pair<int, int>, int>* chunkRecord = _chunkRecords.at(ch_xy10sPair);

		try
		{
			//Rewrite Existing Chunk Entry
			int chunkCachePos = chunkRecord->at(ch_xyMod10Pair);
			
			//Write Chunk
			if (writeChunk(chunk, chunkCachePos) == -1)
			{
				cout << "Error: Unsuccessful Chunk Cache" << endl;
				return -1;
			}
		}
		catch (std::out_of_range)
		{
			//Add Chunk Entry to exisiting Chunk Record
			pair< pair<int, int>, int> chunkEntry(ch_xyMod10Pair, cacheSize);
			chunkRecord->insert(chunkEntry);

			//Write Chunk
			if (writeChunk(chunk, cacheSize) == -1)
			{
				cout << "Error: Unsuccessful Chunk Cache" << endl;
				return -1;
			}
		}
	}
	catch (std::out_of_range)
	{
		//No Matching Chunk Record

		//New Chunk Record-----------------
		map< pair<int, int>, int>* chunkRecord = new map< pair<int, int>, int>;

		//Add Chunk Entry to New Chunk Record
		pair< pair<int, int>, int> entry(ch_xyMod10Pair, cacheSize);
		chunkRecord->insert(entry);

		//Adding Chunk Record------------------
		pair<pair<int, int>, map< pair<int, int>, int>*> chunkRecordPair(ch_xy10sPair, chunkRecord);
		_chunkRecords.insert(chunkRecordPair);

		//Write Chunk
		if (writeChunk(chunk, cacheSize) == -1)
		{
			cout << "Error: Unsuccessful Chunk Cache" << endl;
			return -1;
		}
	}
	return 0;
}

int ChunkCacheManager::getChunkCachePos(int ch_x, int ch_y)
{
	pair<int, int> ch_xy10sPair(ch_x / 10, ch_y / 10);
	pair<int, int> ch_xyMod10Pair(ch_x % 10, ch_y % 10);

	try
	{
		//Chunk Record Found
		map< pair<int, int>, int>* chunkRecord = _chunkRecords.at(ch_xy10sPair);

		try
		{
			//Chunk Cache Pos Found
			return chunkRecord->at(ch_xyMod10Pair);
		}
		catch (std::out_of_range)
		{
			return -1;
		}
	}
	catch (std::out_of_range)
	{
		return -1;
	}

	return -1;
}

int ChunkCacheManager::readChunkCache(Chunk* chunk, int cachePos)
{
	int cacheInitPosInBytes = (sizeof(CacheTile)*CHUNK_He*CHUNK_Wi*cachePos);
	try
	{
		for (int ch_gr_y = 0; ch_gr_y < CHUNK_He; ch_gr_y++)
		{
			for (int ch_gr_x = 0; ch_gr_x < CHUNK_Wi; ch_gr_x++)
			{
				_chunkCache->seekg(cacheInitPosInBytes + (sizeof(CacheTile)*ch_gr_y*CHUNK_Wi) + sizeof(CacheTile)*ch_gr_x);
				CacheTile cacheTile;

				_chunkCache->read((char*)&cacheTile, sizeof(CacheTile));

				chunk->_chunk[ch_gr_y][ch_gr_x] = cacheTile.toTile(chunk);
			}
		}

		chunk->genProg = CHUNK_He*CHUNK_Wi;
		chunk->generated = true;

		chunk->generate(NULL, NULL);
	}
	catch (...)
	{
		cout << "Error: Could Not Read Chunk" << endl;
		return -1;
	}
	return 0;
}