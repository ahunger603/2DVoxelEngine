#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <cmath>
#include <list>

#include "SDL.h"
#include "Tile.h"
#include "Camera.h"
#include "Vector.h"
#include "WorldChunkManager.h"
#include "RigidEntity.h"
#include "PhysicsEngine.h"

class Tile;
class Camera;
class RigidBody;
class WorldChunkManager;
class PhysicsEngine;

//Dimentions
const int CHAR_H = 64;
const int CHAR_W = 12;
const int CHAR_H_HALF = CHAR_H/2;
const int CHAR_W_HALF = CHAR_W/2;
const int BODY_H = 16;
const int BODY_W = 12;


class Character : public RigidEntity
{
private:
	WorldChunkManager* _worldChunkManager;

	double speed;
	double jumpPower;

	Tile* _selectedTile;

	//Intents
	Tile* _intentSelectTile;
	bool _intentMoveLeft;
	bool _intentMoveRight;
	bool _intentJump;

	//Actions
	void selectTile();

	//Movement
	void move(int dir); //0 = left | 1 = right
	void jump();
public:
	Character(WorldChunkManager* worldChunkManager, PhysicsEngine* physicsEngine, int x, int y);

	//Position Functions
	void getXY(int* dest);
	void getChXY(int* dest);
	void getCenterXY(int* dest);
	Vector getInterpolatedXY(float interpolation);

	//Intent
	void intentSelectTile(Tile* tile);
	void intentMoveLeft();
	void intentMoveRight();
	void intentJump();

	//Update
	void update();
	bool isFloor();

	//Draw
	void draw(SDL_Renderer* renderer, float interpolation, Camera* camPtr);
};

#endif