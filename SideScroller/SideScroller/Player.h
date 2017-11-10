#ifndef PLAYER_H
#define PLAYER_H

#include "Tile.h"

class Tile;

class Player
{
private:
	Tile* _selectedTile;
	Tile* _intentSelectTile;

public:
	Player();

	double speed;
	double jumpPower;

	bool _intentMoveLeft;
	bool _intentMoveRight;
	bool _intentJump;

	//Intent
	void intentSelectTile(Tile* tile);

	//Actions
	void selectTile();
};


#endif