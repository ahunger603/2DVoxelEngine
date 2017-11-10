#ifndef INVENTORYMENU_H
#define INVENTORYMENU_H

#include "InputManager.h"
#include "InGameMenu.h"
#include "ItemContainer.h"
#include "Player.h"
#include "ItemContainerMenuComponent.h"

class InGameMenu;
class ItemContainer;
class Player;

class InventoryMenu : public InGameMenu
{
private:
	static const int InvMenuWidth = 500;
	static const int InvMenuHeight = 500;

	static const int PPanelRX = 60;
	static const int PPanelRY = 76;
	static const int PPanelWidth = 126;
	static const int PPanelHeight = 175;

	static const int SPanelRX = 261;
	static const int SPanelRY = 76;
	static const int SPanelWidth = 206;
	static const int SPanelHeight = 175;

	bool _init;

	char* playerTxt;
	char* statsTxt;

	Player* _player;
public:
	InventoryMenu(GameState* gameState, int x, int y);
	~InventoryMenu();

	void init(Player* player, ItemContainer* playerInventory);

	void handleEvents();
	void update();
	void draw();
};



#endif