#ifndef TEMPITEMHOLDER_H
#define TEMPITEMHOLDER_H

#include "Item.h"
#include "ItemContainer.h"

class ItemContainer;

class TemporaryItemHolder
{
private:
	ItemContainer* _container;
	int _xy[2];
public:
	TemporaryItemHolder();
	TemporaryItemHolder(ItemContainer* container, int x, int y);

	void holdItem(ItemContainer* container, int x, int y);
	void dropItem(Item& item_dest);
};

#endif
