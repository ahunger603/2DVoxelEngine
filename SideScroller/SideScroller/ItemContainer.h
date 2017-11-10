#ifndef ITEMCONT_H
#define ITEMCONT_H

#include <iostream>
#include "Item.h"
#include "ResourceManager.h"

const int SLOT_SIZE = 44;
const int SPACER = 2;

struct Item;
class ResourceManager;

class ItemContainer
{
private:
	int _w;
	int _h;

	ItemStack*** container;
public:
	ItemContainer(int w, int h);
	~ItemContainer();

	int getWidth();
	int getHeight();

	bool getItem(int x, int y, Item& item, int* dstCount);
	int getItemCount(int x, int y);

	bool isFullForItem(int primaryID, int secondaryID);
	bool inBounds(int x, int y);

	bool addItem(int x, int y, Item& item);
	bool addItem(Item& item);

	bool swapItemSlots(int x, int y, int x2, int y2);

	bool removeItemByXY(int x, int y, Item& item);
	bool removeItemByID(int primaryID, int secondaryID, Item& item);

	void drawItem(int gr_x, int gr_y, int x, int y, ResourceManager* resManager);
	void drawItemSlot(int gr_x, int gr_y, int x, int y, ResourceManager* resManager);

	void printContainer();
};

#endif