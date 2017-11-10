#include "Chunk.h"

int Chunk::chunkCnt = 0;

Chunk::Chunk(WorldChunkManager* worldChunkManager, ResourceManager* resourceManager, int ch_x, int ch_y)
{
	_worldChunkManager = worldChunkManager;
	_resourceManager = resourceManager;

	changed = false;

	id = (Chunk::chunkCnt++);

	_ch_x = ch_x;
	_ch_y = ch_y;

	_x = (CHUNK_Wi * TILE_Wi)*_ch_x;
	_y = (CHUNK_He * TILE_He)*_ch_y;

	genProg = 0;
	generated = false;

	interactive = false;
	deactivating = false;

	chunkArea = { _x, _y,(CHUNK_Wi * TILE_Wi),(CHUNK_He * TILE_He) };

	//Fill Chunk Space
	for (int gr_y = 0; gr_y < CHUNK_He; gr_y++)
	{
		for (int gr_x = 0; gr_x < CHUNK_Wi; gr_x++)
		{
			_chunk[gr_y][gr_x] = NULL;
		}
	}
}

Chunk::~Chunk()
{
	//Cache
	if (changed)
	{
		_worldChunkManager->getChunkCacheManager()->cacheChunk(this);
	}

	//Delete Eachtile
	for (int gr_y = 0; gr_y < CHUNK_He; gr_y++)
	{
		for (int gr_x = 0; gr_x < CHUNK_Wi; gr_x++)
		{
			Tile* tile = _chunk[gr_y][gr_x];

			if (tile != NULL)
			{
				_chunk[gr_y][gr_x] = NULL;

				delete tile;
			}
		}
	}
}


int Chunk::getID()
{
	return id;
}

void Chunk::updateEdgeTileSituations(int side)
{
	int _side = side;
	if (side > 3)
		_side %= 3;

	if (side == 0)
	{
		for (int gr_y = 0; gr_y < CHUNK_He; gr_y++)
		{
			_chunk[gr_y][CHUNK_Wi - 1]->adjustSituation();
		}
	}
	else
	{
		if (side == 1)
		{
			for (int gr_x = 0; gr_x < CHUNK_He; gr_x++)
			{
				_chunk[0][gr_x]->adjustSituation();
			}
		}
		else
		{
			if (side == 2)
			{
				for (int gr_y = 0; gr_y < CHUNK_He; gr_y++)
				{
					_chunk[gr_y][0]->adjustSituation();
				}
			}
			else
			{
				if (side == 3)
				{
					for (int gr_x = 0; gr_x < CHUNK_He; gr_x++)
					{
						_chunk[CHUNK_He - 1][gr_x]->adjustSituation();
					}
				}
			}
		}
	}
}

WorldChunkManager* Chunk::getWorldChunkManager()
{
	return _worldChunkManager;
}

ResourceManager* Chunk::getResourceManager()
{
	return _resourceManager;

}

//----------------------Position Functions
void Chunk::getXY(int* dest)
{
	dest[0] = _x;
	dest[1] = _y;

	return;
}

void Chunk::getCh_XY(int* dest)
{
	dest[0] = _ch_x;
	dest[1] = _ch_y;

	return;
}

void Chunk::getTileXY(int ch_gr_x, int ch_gr_y, int* dest)
{
	dest[0] = _x + (TILE_Wi * ch_gr_x);
	dest[1] = _y - (TILE_He * ch_gr_y);

	return;
}

bool Chunk::inView(Camera* camPtr)
{
	int dx = camPtr->getDrawX(_x);
	int dy = camPtr->getDrawY(_y);
	int dx2 = camPtr->getDrawX(_x + chunkArea.w);
	int dy2 = camPtr->getDrawY(_y - chunkArea.h);

	int windowSize[2] = { 0, 0 };
	_worldChunkManager->getWindowSize(windowSize);

	if (dx > windowSize[0] || dy > windowSize[1] || dx2 < 0 || dy2 < 0)
		return false;

	return true;
}

bool Chunk::inRect(int x, int y, int w, int h)
{
	int _x2 = _x + chunkArea.w;
	int _y2 = _y - chunkArea.h;
	int rectX2 = x + w;
	int rectY2 = y - h;

	if (x > _x2 || y < _y2 || rectX2 < _x || rectY2 > _y)
		return false;

	return true;
}

bool Chunk::isGenerated()
{
	return generated;
}

Chunk* Chunk::getChunkByRelativeChXY(int rch_x, int rch_y)
{
	if (!(rch_x == 0 && rch_y == 0))
	{
		return _worldChunkManager->getChunkAtChXY(_ch_x + rch_x, _ch_y + rch_y);
	}
	else
		return this;
}

//---------------------------------Tile Functions

void Chunk::getTilesInRect(int x, int y, int w, int h, std::list<Tile*> &tileList)
{
	int gr_xy1[2] = {((x - _x) / TILE_Wi), ((_y - y) / TILE_He)};
	int gr_xy2[2] = { gr_xy1[0] + (w / TILE_Wi)+1, gr_xy1[1] + (h / TILE_He)+1};

	for (int gr_y = gr_xy1[1]; gr_y <= gr_xy2[1] && gr_y < CHUNK_He; gr_y++)
	{
		for (int gr_x = gr_xy1[0]; gr_x <= gr_xy2[0] && gr_x < CHUNK_Wi; gr_x++)
		{
			if (gr_y >= 0 && gr_x >= 0)
			{
				Tile* tile = _chunk[gr_y][gr_x];
				if (tile->notEmpty() && tile->inRect(x, y, w, h))
					tileList.push_front(tile);
			}
		}
	}
}

Tile* Chunk::getTileAt_XY(int x, int y)
{
	int ch_gr_x = abs(x - _x) / TILE_Wi;
	int ch_gr_y = abs(y - _y) / TILE_He;

	return getTileAt_ChGrXY(ch_gr_x, ch_gr_y);
}

Tile* Chunk::getTileAt_ChGrXY(int ch_gr_x, int ch_gr_y)
{
	return _chunk[ch_gr_y][ch_gr_x];
}


//---------------------------------Generation Functions
void Chunk::generate(int _worldSeed, int steps)
{
	int step_cnt = 0;
	int ChunkArea = (CHUNK_Wi * CHUNK_He);
	int totalProg = genProg;
	while (step_cnt < steps && (totalProg) < ChunkArea)
	{
		int gr_y = (totalProg) / CHUNK_Wi;
		int gr_x = (totalProg) % CHUNK_Wi;

		generateTile(_worldSeed, gr_x, gr_y);

		step_cnt++;
		totalProg++;
	}

	genProg += steps;

	if (!(genProg < ChunkArea))
	{
		for (int gr_x = 0; gr_x < CHUNK_Wi; gr_x++)
		{
			for (int gr_y = 0; gr_y < CHUNK_He; gr_y++)
			{
				_chunk[gr_y][gr_x]->adjustSituation();
			}
		}
		for (int rch_y = -1; rch_y <= 1; rch_y++)
		{
			for (int rch_x = -1; rch_x <= 1; rch_x++)
			{
				Chunk* chunk = getChunkByRelativeChXY(rch_x, rch_y);
				if (chunk != nullptr)
				{
					for (int i = 0; i < 4; i++)
						chunk->updateEdgeTileSituations(i);
				}
			}
		}
		generated = true;
	}
}

void Chunk::generateTile(int _worldSeed, int gr_x, int gr_y)
{
	Tile* tile = new Tile(gr_x, gr_y, Empty_def, this);
	_chunk[gr_y][gr_x] = tile;

	int XY[2] = { 0, 0};
	tile->getXY(XY);

	int seedFactor1 = (_worldSeed % 13) + 1;
	int seedFactor2 = (_worldSeed % 32) + 1;

	if (XY[1] > scaled_octave_noise_2d(1, 0, 0.00075f, 200, -300, XY[0], seedFactor1*500) - 400)
	{
		//Above Ground

		//Grass Lands
		int GrassY = scaled_octave_noise_2d(1, 0, 0.001f, 450, -200, XY[0], seedFactor2 * 1000);
		if (XY[1] < GrassY)
		{
			if (XY[1] < (GrassY - TILE_He - 12))
				tile->setType(Dirt_def);
			else
				tile->setType(Grass_def);
		}
	}
	else
	{
		//Below Ground
		tile->setType(Rock_def);
	}
}

//--------------------------------------General

void Chunk::setInteractive()
{
	changed = true;
	interactive = true;
}

void Chunk::update()
{
	bool pastInteractive = interactive;

	interactive = false;
	for (int x = 0; x < CHUNK_Wi; x++)
	{
		for (int y = 0; y < CHUNK_He; y++)
		{
			if (_chunk[y][x] != NULL && _chunk[y][x]->notEmpty())
				_chunk[y][x]->update();
		}
	}

	//Interactive
	if (!pastInteractive && interactive)
	{
		deactivating = false;
	}

	//Non-interactive
	if (pastInteractive && !interactive)
	{
		deactivating = true;
		ticks_sinceInteractive = SDL_GetTicks();
	}

	//Deactivation
	if (deactivating)
	{
		if (SDL_GetTicks() - ticks_sinceInteractive > TICKS_TO_DEACTIVATE)
		{
			deactivating = false;
		}
	}
}

void Chunk::draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr)
{
	//Manually Draw All tiles
	if (generated && camPtr->inView(&chunkArea))
	{
		for (int x = 0; x < CHUNK_Wi; x++)
		{
			for (int y = 0; y < CHUNK_He; y++)
			{
				if (_chunk[y][x]->notEmpty())
					_chunk[y][x]->draw(renderer, interpolation, camPtr);
			}
		}
	}
	/*
	else
	{
		//Draw Texture
		if (generated && camPtr->inView(&chunkArea))
		{
			//Draw Chunk Texture
			int xy[2] = { 0, 0 };
			getXY(xy);

			xy[0] = camPtr->getDrawX(xy[0]);
			xy[1] = camPtr->getDrawY(xy[1]);

			SDL_Rect chunkRect = { xy[0], xy[1], chunkArea.w, chunkArea.h };
			SDL_RenderCopy(renderer, _resourceManager->getChunkTexture(id), NULL, &chunkRect);
		}
	}
	*/
}
