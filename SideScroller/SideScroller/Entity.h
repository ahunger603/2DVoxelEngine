#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#include "SDL.h"
#include "EntityComponents.h"
#include "EntityManager.h"

class EntityManager;
class ItemContainer;

struct Dynamic;
struct AABB;
struct Item;

class Entity
{
private:
	static int entityCount;

	bool _exists;

	 int _id;
	 uint64_t _mask;

	Vector _pos;

	Player* _player;
	Dynamic* _dynamic;
	Physics* _phys;
	AABB* _aabb;
	Item* _item;
	ItemContainer* _itemCont;
public:
	Entity(int mask, float x, float y, Player* player, Dynamic* dynamic, Physics* phys, AABB* aabb, Item* item, ItemContainer* itemCont);
	~Entity();

	bool exists();
	void destroy();

	int getID();
	uint64_t getMask();

	void setPos(Vector pos);

	Vector getPos();
	void getXY(float* dest);
	void getCenterXY(float* dest);
	void getInterpolatedXY(float interpolation, float* dest);

	Player* getPlayer();
	Dynamic* getDynamic();
	Physics* getPhysics();
	AABB* getAABB();
	Item* getItem();
	ItemContainer* getItemContainer();
};


#endif