#include "Tile.h"
#include <iostream>

//Constructors
Tile::Tile(int ch_gr_x, int ch_gr_y, tile_def def, Chunk* parent)
{
	_def = def;

	_parentChunk = parent;

	_ch_gr_x = ch_gr_x;
	_ch_gr_y = ch_gr_y;

	int parentChunkXY[2] = { 0, 0 };
	_parentChunk->getXY(parentChunkXY);

	InteractiveTile* interactiveTile = NULL;

	_x = parentChunkXY[0] + (TILE_He * _ch_gr_x);
	_y = parentChunkXY[1] - (TILE_Wi * _ch_gr_y);

	tileVariation = abs((int)(_x * _y * 0.7)) % 3;
}

void Tile::setType(tile_def def)
{
	_def = def;

	if (_parentChunk->isGenerated())
	{
		adjustSituation();
		for (int i = 0; i <= 7; i++)
		{
			Tile* adjT = getAdjacentTile(i);
			if (adjT != nullptr && adjT->notEmpty())
				adjT->adjustSituation();
		}
	}
}


bool Tile::isSolid()
{
	return TILE_SOLIDITY.at(_def);
}

bool Tile::inRect(int x, int y, int w, int h)
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	int X2 = XY[0] + TILE_Wi;
	int Y2 = XY[1] - TILE_He;
	int rectX2 = x + w;
	int rectY2 = y - h;

	if (x > X2 || y < Y2 || rectX2 < XY[0] || rectY2 > XY[1])
		return false;

	return true;
}

bool Tile::pointInTile(int x, int y)
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	if (x >= XY[0] && y <= XY[1] && x <= XY[0] + TILE_Wi && y >= XY[1] - TILE_He)
		return true;

	return false;
}

CacheTile* Tile::toCacheTile()
{
	return (new CacheTile(this));
}

tile_def Tile::getTileDef()
{
	return _def;
}

Tile* Tile::getAdjacentTile(int dir)
{
	Tile* tile = nullptr;

	int _dir = dir;
	if (dir > 7)
		dir %= 7;

	int t_gr_xy[2] = { _ch_gr_x, _ch_gr_y };
	switch (dir)
	{
	case 0:
		t_gr_xy[0]++;
		break;
	case 1:
		t_gr_xy[0]++;
		t_gr_xy[1]--;
		break;
	case 2:
		t_gr_xy[1]--;
		break;
	case 3:
		t_gr_xy[0]--;
		t_gr_xy[1]--;
		break;
	case 4:
		t_gr_xy[0]--;
		break;
	case 5:
		t_gr_xy[0]--;
		t_gr_xy[1]++;
		break;
	case 6:
		t_gr_xy[1]++;
		break;
	case 7:
		t_gr_xy[0]++;
		t_gr_xy[1]++;
		break;
	}
	if (t_gr_xy[0] >= 0 && t_gr_xy[1] >= 0 && t_gr_xy[0] < CHUNK_Wi && t_gr_xy[1] < CHUNK_He)
	{
		tile = _parentChunk->_chunk[t_gr_xy[1]][t_gr_xy[0]];
	}
	else
	{
		Chunk* adjChunk = nullptr;

		int rch_x = 0;
		int rch_y = 0;

		if (0 > t_gr_xy[0])
			rch_x--;
		if (0 > t_gr_xy[1])
			rch_y++;
		if (t_gr_xy[0] >= CHUNK_Wi)
			rch_x++;
		if (t_gr_xy[1] >= CHUNK_He)
			rch_y--;

		adjChunk = _parentChunk->getChunkByRelativeChXY(rch_x, rch_y);
		if (adjChunk != nullptr)
		{
			if (t_gr_xy[0] >= CHUNK_Wi)
				t_gr_xy[0] -= CHUNK_Wi;

			if (t_gr_xy[1] >= CHUNK_He)
				t_gr_xy[1] -= CHUNK_He;

			if (t_gr_xy[0] < 0)
				t_gr_xy[0] += CHUNK_Wi;

			if (t_gr_xy[1] < 0)
				t_gr_xy[1] += CHUNK_He;

			tile = adjChunk->getTileAt_ChGrXY(t_gr_xy[0], t_gr_xy[1]);
		}
	}

	return tile;
}

void Tile::adjustSituation()
{
	if (notEmpty())
	{
		uint64_t tileSituation = 0;
		for (int i = 0; i <= 3; i++)
		{
			int dir = i * 2;
			Tile* adjTile = getAdjacentTile(dir);
			if (adjTile != nullptr)
			{
				if (adjTile->notEmpty())
				{
					if ((adjTile->getTileDef() == _def) || ((_def == Dirt_def || _def == Grass_def) && (adjTile->getTileDef() == Grass_def || adjTile->getTileDef() == Dirt_def)))
					{
						switch (dir)
						{
						case 0:
							tileSituation = (tileSituation | TS_S_R);
							break;
						case 2:
							tileSituation = (tileSituation | TS_S_T);
							break;
						case 4:
							tileSituation = (tileSituation | TS_S_L);
							break;
						case 6:
							tileSituation = (tileSituation | TS_S_B);
							break;
						}
					}
					else
					{
						switch (dir)
						{
						case 0:
							tileSituation = (tileSituation | TS_D_R);
							break;
						case 2:
							tileSituation = (tileSituation | TS_D_T);
							break;
						case 4:
							tileSituation = (tileSituation | TS_D_L);
							break;
						case 6:
							tileSituation = (tileSituation | TS_D_B);
							break;
						}
					}
				}
				else
				{
					switch (dir)
					{
					case 0:
						tileSituation = (tileSituation | TS_E_R);
						break;
					case 2:
						tileSituation = (tileSituation | TS_E_T);
						break;
					case 4:
						tileSituation = (tileSituation | TS_E_L);
						break;
					case 6:
						tileSituation = (tileSituation | TS_E_B);
						break;
					}
				}
			}
		}

		_parentChunk->getResourceManager()->updateTileTexture(_def, tileVariation, tileSituation, texturePage, textureLocation);
	}
}

//Functions
bool Tile::notEmpty() { return(_def != Empty_def); }

void Tile::getCh_Gr_XY(int* dest)
{
	dest[0] = _ch_gr_x;
	dest[1] = _ch_gr_y;

	return;
}

void Tile::getCh_XY(int* dest)
{
	return (_parentChunk->getCh_XY(dest));
}

void Tile::getXY(int* dest)
{
	dest[0] = _x;
	dest[1] = _y;
}

void Tile::selected()
{
	if (interactiveTile == NULL)
		interactiveTile = new InteractiveTile(this);
}

void Tile::deselected()
{
	if (interactiveTile != NULL)
		delete interactiveTile;

	interactiveTile = NULL;
}

void Tile::breakTile()
{
	if (interactiveTile != NULL)
	{
		interactiveTile->breakTile();
	}
}

void Tile::update()
{
	if (notEmpty())
	{
		if (interactiveTile != NULL)
		{
			_parentChunk->setInteractive();
		}
	}
}

void Tile::draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr)
{
	if (notEmpty())
	{
		int tile_xy[2] = { _x, _y };

		tile = { tile_xy[0], tile_xy[1], TILE_Wi, TILE_He };

		//Visiblity
		if (camPtr->inView(&tile))
			_parentChunk->getResourceManager()->drawTile(camPtr, _x, _y, texturePage, &textureLocation);
	}
}


//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------InteractiveTile---------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|

InteractiveTile::InteractiveTile(Tile* parentTile)
{
	_parentTile = parentTile;

	breakHits = 0;
}

void InteractiveTile::breakTile()
{
	breakHits++;

	if (breakHits > 5)
	{
		_parentTile->setType(Empty_def);
	}
}

//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//---------------------------CacheTile-----------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|
//-----------------------------------------------------------------|//-----------------------------------------------------------------|

CacheTile::CacheTile()
{
	_def = Empty_def;

	_ch_gr_x = NULL;
	_ch_gr_y = NULL;
}

CacheTile::CacheTile(Tile* tile)
{
	_def = tile->_def;

	_ch_gr_x = tile->_ch_gr_x;
	_ch_gr_y = tile->_ch_gr_y;
}

CacheTile::CacheTile(int ch_gr_x, int ch_gr_y, tile_def def)
{
	_def = def;

	_ch_gr_x = ch_gr_x;
	_ch_gr_y = ch_gr_y;
}

Tile* CacheTile::toTile(Chunk* chunk)
{
	return (new Tile(_ch_gr_x, _ch_gr_y, _def, chunk));
}