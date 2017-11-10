#include "Entity.h"

int Entity::entityCount = 0;

Entity::Entity(int mask, float x, float y, Player* player, Dynamic* dynamic, Physics* phys, AABB* aabb, Item* item, ItemContainer* itemCont) : _pos(x, y)
{
	_exists = true;

	_id = (++entityCount);
	_mask = mask;

	_player = player;
	_dynamic = dynamic;
	_phys = phys;
	_aabb = aabb;
	_item = item;
	_itemCont = itemCont;
}

Entity::~Entity()
{
	if (_player != nullptr)
		delete _player;
	if (_dynamic != nullptr)
		delete _dynamic;
	if (_aabb != nullptr)
		delete _aabb;
	if (_item != nullptr)
		delete _item;
}

bool Entity::exists()
{
	return _exists;
}

void Entity::destroy()
{
	_exists = false;
}

void Entity::setPos(Vector pos)
{
	_pos = pos;
}

int Entity::getID()
{
	return _id;
}

uint64_t Entity::getMask()
{
	return _mask;
}

Vector Entity::getPos()
{
	return _pos;
}

void Entity::getXY(float* dest)
{
	dest[0] = _pos.getX();
	dest[1] = _pos.getY();
}

void Entity::getCenterXY(float* dest)
{
	if ((COMPONENT_AABB & _mask) == COMPONENT_AABB)
	{
		dest[0] = _pos.getX() + (_aabb->_w / 2);
		dest[1] = _pos.getY() - (_aabb->_h / 2);
	}
	else
	{
		getXY(dest);
	}
}

void Entity::getInterpolatedXY(float interpolation, float* dest)
{
	if ((COMPONENT_DYNAMIC & _mask) == COMPONENT_DYNAMIC)
	{
		Vector interPos = _pos + (_dynamic->_velocity * interpolation);
			
		dest[0] = interPos.getX();
		dest[1] = interPos.getY();
	}
	else
	{
		getXY(dest);
	}
}

Player* Entity::getPlayer()
{
	return _player;
}

Dynamic* Entity::getDynamic()
{
	return _dynamic;
}

Physics* Entity::getPhysics()
{
	return _phys;
}

AABB* Entity::getAABB()
{
	return _aabb;
}

Item* Entity::getItem()
{
	return _item;
}

ItemContainer* Entity::getItemContainer()
{
	return _itemCont;
}