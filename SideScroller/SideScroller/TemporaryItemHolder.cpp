#include "TemporaryItemHolder.h"

TemporaryItemHolder::TemporaryItemHolder()
{
	_container = nullptr;
	_xy[0] = -1;
	_xy[1] = -1;
}

TemporaryItemHolder::TemporaryItemHolder(ItemContainer* container, int x, int y)
{
	_container = container;
	_xy[0] = x;
	_xy[1] = y;
}

void TemporaryItemHolder::holdItem(ItemContainer* container, int x, int y)
{
	_container = container;
	_xy[0] = x;
	_xy[1] = y;
}

void TemporaryItemHolder::dropItem(Item& item_dest)
{
	if (_container != nullptr && _xy[0] != -1 && _xy[1] != -1)
	{
		_container->removeItemByXY(_xy[0], _xy[1], item_dest);
	}
}