#ifndef PHYENGINE_H
#define PHYENGINE_H

#include <cmath>
#include <list>
#include <unordered_map>
#include <iostream>

#include "Vector.h"
#include "WorldChunkManager.h"
#include "EntityComponents.h"
#include "Entity.h"
#include "EntityManager.h"

struct AABB;
struct Dynamic;
struct Physics;

class WorldChunkManager;
class Entity;
class EntityManager;

class PhysicsEngine
{
private:
	EntityManager* _entityManager;
	WorldChunkManager* _worldChunkManager;


	//Variable Preinstatiation----------------------
	Dynamic* dyn;
	Vector targetPos;
	float friction_coeff;
	Vector current_v;

	//Item only 
	Entity* player;
	float playerXY[2];
	int x_diff;
	int y_diff;
	float manhattenDist;
	Vector dir;

	//Tile Collision
	AABB* aabb;
	Physics* phys;
	float exy[2];
	int ew;
	int eh;
	Vector current_a;
	Vector velocity_dir;
	Vector velocity_dirFract;
	bool collisions[4];

	//Get Origin Point
	int originPoint[2];

	//Tile Cords
	int XY[2];
	int lb[2];
	int rt[2];

	float t1;
	float t2;
	float t3;
	float t4;

	float tmin;
	float tmax;

	float tt;
public:
	PhysicsEngine(EntityManager* entityManager, WorldChunkManager* worldChunkManager); 

	void updateEntity(Entity* entity);
};

#endif