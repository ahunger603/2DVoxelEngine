#include "Player.h"

Player::Player()
{
	speed = 132;
	jumpPower = 800;

	_selectedTile = nullptr;

	//Intents
	_intentSelectTile = nullptr;
	_intentMoveLeft = false;
	_intentMoveRight = false;
	_intentJump = false;
}

//Intent
void Player::intentSelectTile(Tile* tile)
{
	_intentSelectTile = tile;
}

void Player::selectTile()
{
	if (_intentSelectTile != NULL && _intentSelectTile->notEmpty())
	{
		//New Tile
		if (_intentSelectTile != _selectedTile)
		{
			//std::cout << "Changed Tile" << std::endl;

			if (_selectedTile != NULL)
				_selectedTile->deselected();

			_selectedTile = _intentSelectTile;

			_selectedTile->selected();
		}
		else
		{
			//SameTile
			//std::cout << "Breaking Tile" << std::endl;

			if (_selectedTile != NULL)
			{
				_selectedTile->breakTile();
			}
		}
	}
	else
	{
		//Empty Tile
		if (_selectedTile != NULL)
		{
			//Clearing Selection
			//std::cout << "Clearing Selection" << std::endl;

			_selectedTile->deselected();

			_selectedTile = NULL;
		}
	}
	_intentSelectTile = NULL;
}