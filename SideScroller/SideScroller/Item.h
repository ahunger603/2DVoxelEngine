#ifndef ITEM_H
#define ITEM_H

#include <unordered_map>

typedef enum PrimaryTags
{
	MELEE_WEAPON = 1,
	HELM = 2,
	CHEST = 3,
	BOOTS = 4,
	GLOVES = 5,
	TOOL = 6,
	INGREDIENT = 7,
	FOOD = 8,
	BLOCK = 9,
	ENHANCEMENT_GEM = 10
};

typedef enum BlockTags
{
	DIRT = 1,
	ROCK = 2,
	GRASS = 3
};

static const std::unordered_map<int, int> BlockIcons
{
	{ DIRT, 0 },
	{ ROCK, 1 }
};

static const std::unordered_map<PrimaryTags, std::unordered_map<int, int>> ItemIconMap
{
	{ BLOCK, BlockIcons }
};

const int ENHANCEMNT_CAP = 8;
struct Item
{
	int _primaryID;
	int _secondaryID;
	int _durability;

	Item(int primaryID, int secondaryID, int durability)
	{
		_primaryID = primaryID;
		_secondaryID = secondaryID;
		_durability = durability;
	}

	Item(Item& item)
	{
		_primaryID = item._primaryID;
		_secondaryID = item._secondaryID;
		_durability = item._durability;
	}

	bool equalTo(Item& item)
	{
		return (_primaryID == item._primaryID && _secondaryID == item._secondaryID);
	}

	void setTo(Item& item)
	{
		_primaryID = item._primaryID;
		_secondaryID = item._secondaryID;
		_durability = item._durability;
	}
};

struct ItemStack
{
	int _sizeLimit;
	int _size;
	Item _item;

	ItemStack(Item& item) : _item(item)
	{
		_sizeLimit = 64;
		_size = 1;
	}
};

#endif