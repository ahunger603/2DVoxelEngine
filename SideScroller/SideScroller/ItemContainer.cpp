#include "ItemContainer.h"

ItemContainer::ItemContainer(int w, int h)
{
	_w = w;
	_h = h;

	container = new ItemStack**[_h];
	for (int i = 0; i < _h; i++)
	{
		ItemStack** row = new ItemStack*[_w];

		for (int j = 0; j < _w; j++)
			row[j] = nullptr;

		container[i] = row;
	}
}

ItemContainer::~ItemContainer()
{
	for (int i = 0; i < _h; i++)
	{
		ItemStack** row = container[i];

		for (int j = 0; j < _w; j++)
		{
			if (row[j] != nullptr)
				delete row[j];

			row[j] = nullptr;
		}

		delete[] row;

		row = nullptr;
	}
	delete[] container;

	container = nullptr;
}

int ItemContainer::getWidth()
{
	return _w;
}

int ItemContainer::getHeight()
{
	return _h;
}

bool ItemContainer::getItem(int x, int y, Item& item, int* dstCount)
{
	ItemStack* containerItem = container[y][x];

	if (containerItem != nullptr)
	{
		item.setTo(containerItem->_item);
		*dstCount = containerItem->_size;
		return true;
	}
	else
		return false;
}

int ItemContainer::getItemCount(int x, int y)
{
	ItemStack* containerItem = container[y][x];

	if (containerItem != nullptr)
	{
		return containerItem->_size;
	}
	else
		return 0;
}

bool ItemContainer::isFullForItem(int primaryID, int secondaryID)
{
	bool ret = true;
	for (int y = 0; y < _h && ret; y++)
	{
		for (int x = 0; x < _w && ret; x++)
		{
			ItemStack* itemStack = container[y][x];
			if (itemStack != nullptr)
			{
				if (itemStack->_item._primaryID == primaryID && itemStack->_item._secondaryID == secondaryID && itemStack->_size < itemStack->_sizeLimit)
					ret = false;
			}
			else
				ret = false;
		}
	}

	return ret;
}

bool ItemContainer::inBounds(int x, int y)
{
	return (x > 0 && x < _w) && (y > 0 && y < _h);
}

bool ItemContainer::addItem(int x, int y, Item& item)
{
	if (inBounds(x, y))
	{
		ItemStack* stack = container[y][x];
		if (stack == nullptr)
		{
			container[y][x] = new ItemStack(item);

			return true;
		}
		else
		{
			if (stack->_sizeLimit < stack->_size && stack->_item.equalTo(item))
			{
				stack->_size++;

				return true;
			}
			else
				return addItem(item);
		}
	}
	else
		return false;
}

bool ItemContainer::addItem(Item& item)
{
	for (int y = 0; y < _h; y++)
	{
		for (int x = 0; x < _w; x++)
		{
			ItemStack* stack = container[y][x];
			if (stack == nullptr)
			{
				container[y][x] = new ItemStack(item);
				return true;
			}
			else
			{
				if (stack->_size < stack->_sizeLimit && stack->_item.equalTo(item))
				{
					stack->_size++;
					return true;
				}
			}
		}
	}

	return false;
}

bool ItemContainer::swapItemSlots(int x, int y, int x2, int y2)
{
	if (x >= 0 && x < _w && y >= 0 && y < _h && x2 >= 0 && x2 < _w && y2 >= 0 && y2 < _h)
	{
		ItemStack* stack1 = container[y][x];
		ItemStack* stack2 = container[y2][x2];

		container[y][x] = stack2;
		container[y2][x2] = stack1;

		return true;
	}
	else
		return false;
}

bool ItemContainer::removeItemByXY(int x, int y, Item& item)
{
	if (inBounds(x, y))
	{
		ItemStack* stack = container[y][x];
		if (stack != nullptr)
		{
			stack->_size--;

			item.setTo(stack->_item);

			if (stack->_size == 0)
			{
				delete stack;

				container[y][x] = nullptr;
			}
			return true;
		}
		else
			return nullptr;

	}
	else
		return nullptr;
}

bool ItemContainer::removeItemByID(int primaryID, int secondaryID, Item& item)
{
	//TODO
	return nullptr;
}

void ItemContainer::drawItem(int gr_x, int gr_y, int x, int y, ResourceManager* resManager)
{
	if (gr_x >= 0 && gr_x < _w && gr_y >= 0 && gr_y < _w)
	{
		ItemStack* itemStack = container[gr_y][gr_x];
		if (itemStack != nullptr)
		{
			resManager->drawItemStack(itemStack, x, y);
		}
	}
}

void ItemContainer::drawItemSlot(int gr_x, int gr_y, int x, int y, ResourceManager* resManager)
{
	if (gr_x >= 0 && gr_x < _w && gr_y >= 0 && gr_y < _w)
	{
		ItemStack* itemStack = container[gr_y][gr_x];
		if (itemStack != nullptr)
		{
			resManager->drawItemContainerSlot(x, y, itemStack);
		}
		else
		{
			resManager->drawItemContainerSlot(x, y, nullptr);
		}
	}
}

void ItemContainer::printContainer()
{
	for (int y = 0; y < _h; y++)
	{
		std::cout << ">> ";
		for (int x = 0; x < _w; x++)
		{
			ItemStack* stack = container[y][x];
			if (stack != nullptr)
				std::cout << "ItemStack: " << stack->_size << " | ";
		}
		std::cout << std::endl;
	}

}