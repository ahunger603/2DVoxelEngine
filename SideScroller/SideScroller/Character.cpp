#include "Character.h"

Character::Character(WorldChunkManager* worldChunkManager, PhysicsEngine* physicsEngine, int xx, int yy) : RigidEntity(worldChunkManager->getChunkAtXY(xx, yy), physicsEngine, (float)xx, (float)yy, BODY_W, BODY_H, 50, 0, 1, 1, true, true)
{
	_worldChunkManager = worldChunkManager;

	speed = 132;
	jumpPower = 800;

	_selectedTile = NULL;

	//Intent
	_intentSelectTile = false;
	_intentMoveLeft = false;
	_intentMoveRight = false;
	_intentJump = false;
}

//Position Functions
void Character::getXY(int* dest)
{
	dest[0] = (int)getX();
	dest[1] = (int)getY() + (CHAR_H - BODY_H);

	return;
}

void Character::getChXY(int* dest)
{
	return _worldChunkManager->getChXY((int)getX(), (int)getY() - (CHAR_H - BODY_H), dest);
}

void Character::getCenterXY(int* dest)
{
	dest[0] = (int)getX() + (CHAR_W_HALF);
	dest[1] = (int)(getY() + (CHAR_H - BODY_H)) - (CHAR_H_HALF);

	return;
}

Vector Character::getInterpolatedXY(float interpolation)
{
	Vector InterPos = RigidEntity::getInterpolatedXY(interpolation);
	InterPos.setY(InterPos.getY() + (CHAR_H - BODY_H));

	return InterPos;
}

//Intent
void Character::intentSelectTile(Tile* tile)
{
	_intentSelectTile = tile;
}

void Character::intentMoveLeft() { _intentMoveLeft = true ;}

void Character::intentMoveRight() { _intentMoveRight = true; }

void Character::intentJump() { _intentJump = true; }

//Actions
void Character::selectTile()
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

//Movement
void Character::move(int dir)
{
	if (isFloor())
	{
		if (dir == 0)
			addForce(Vector(-speed, 0));
		else
			addForce(Vector(speed, 0));
	}
	_intentMoveLeft = false;
	_intentMoveRight = false;
}

void Character::jump()
{
	if (isFloor())
	{
		addForce(Vector(0, jumpPower));
	}
	_intentJump = false;
}

//Update
void Character::update()
{
	//Action
	selectTile();

	//Movement
	if (_intentMoveLeft)
		move(0);
	if (_intentMoveRight)
		move(1);
	if (_intentJump)
		jump();
}

bool Character::isFloor()
{
	return getCollision(0);
}

//Draw
void Character::draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr)
{
	Vector playerInterPos = getInterpolatedXY(interpolation);
	int xx = camPtr->getDrawX(playerInterPos.getX());
	int yy = camPtr->getDrawY(playerInterPos.getY());

	SDL_Rect p = {xx, yy, CHAR_W, CHAR_H};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
	SDL_RenderFillRect(renderer, &p);
}