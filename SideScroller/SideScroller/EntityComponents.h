#ifndef ENTITYCOMPONENTS
#define ENTITYCOMPONENTS

#include "Item.h"
#include "ItemContainer.h"
#include "Player.h"
#include "Vector.h"

struct Item;

class Vector;
class ItemContainer;

typedef enum Component
{
	COMPONENT_NONE = 0,
	COMPONENT_PLAYER = 1 << 0,
	COMPONENT_AABB = 1 << 1,
	COMPONENT_DYNAMIC = 1 << 2,
	COMPONENT_PHYSICS = 1 << 3,
	COMPONENT_SPRITE = 1 << 4,
	COMPONENT_PERSISTANCE = 1 << 5,
	COMPONENT_CONTAINER = 1 << 6,
	COMPONENT_LOOT = 1 << 7,
	COMPONENT_CHEST = 1 << 8,
	COMPONENT_DESTRUCTABLE = 1 << 9,
	COMPONENT_PERISHABLE = 1 << 10,
	COMPONENT_ITEM = 1 << 11
};

struct AABB
{
	float _w;
	float _h;

	AABB(float w, float h)
	{
		_w = w;
		_h = h;
	}
};

struct Dynamic
{
	Vector _velocity;

	Dynamic() : _velocity(0, 0){}
};

struct Physics
{
	float _gravityFactor;
	float _airResistFactor;

	float _invMass;
	float _Mass;
	float _elasticity;

	std::list<Vector>* forces;

	bool collisions[4];

	Physics(float gravityFactor, float airResistFactor, float mass, float elasticity)
	{
		_gravityFactor = gravityFactor;
		_airResistFactor = airResistFactor;

		_invMass = (mass > 0) ? (1 / mass) : (0);
		_Mass = mass;
		_elasticity = elasticity;

		for (int i = 0; i < 4; i++)
		{
			collisions[i] = false;
		}

		forces = new std::list<Vector>();
	}
};

#endif