#include "InventoryMenu.h"

InventoryMenu::InventoryMenu(GameState* gameState, int x, int y) : InGameMenu(gameState, "Inventory", false, false, KEY_I, x, y, 477, 508)
{
	_init = false;

	ResourceManager* resManager = gameState->getGameEngine()->getResManager();

	playerTxt = "Player";
	statsTxt = "Stats";

	_player = nullptr;
}

InventoryMenu::~InventoryMenu()
{

}

void InventoryMenu::init(Player* player, ItemContainer* playerInventory)
{
	_player = player;
	addComponent(new ItemContainerMenuComponent(this, playerInventory, PPanelRX - 6 - 45, PPanelRY + PPanelHeight + 16));
	_init = true;
}

void InventoryMenu::handleEvents()
{
	if (_init)
	{
		defaultHandleEvents();
		if (active)
		{

		}
	}
}

void InventoryMenu::update()
{
	if (_init)
	{
		defaultUpdate();
		if (active)
		{

		}
	}
}

void InventoryMenu::draw()
{
	if (_init)
	{
		defaultDraw();
		if (active)
		{
			ResourceManager* resManager = _gameState->getGameEngine()->getResManager();
			//Panels
			resManager->drawMenuPanel(_x + PPanelRX, _y + PPanelRY, PPanelWidth, PPanelHeight, 3);
			resManager->drawMenuPanel(_x + SPanelRX, _y + SPanelRY, SPanelWidth, SPanelHeight, 4);
			 
			//Text
			int wh[2] = { 0, 0 };
			resManager->queryText(font_ALPHABETA_24_W, playerTxt, &wh[0], &wh[1]);
			resManager->drawText(font_ALPHABETA_24_W, playerTxt, _x + PPanelRX + (PPanelWidth / 2) - (wh[0] / 2), _y - (wh[1] / 2) + (((PPanelRY - TITLE_TAB_HEIGHT) / 2) + TITLE_TAB_HEIGHT));

			resManager->queryText(font_ALPHABETA_24_W, statsTxt, &wh[0], &wh[1]);
			resManager->drawText(font_ALPHABETA_24_W, statsTxt, _x + SPanelRX + (SPanelWidth / 2) - (wh[0] / 2), _y - (wh[1] / 2) + (((SPanelRY - TITLE_TAB_HEIGHT) / 2) + TITLE_TAB_HEIGHT));

			//Slots
			int ryInc = (PPanelHeight / 3) + 6;
			resManager->drawEquipmentSlot(_x + PPanelRX - 6 - 45, _y + PPanelRY, 1);
			resManager->drawEquipmentSlot(_x + PPanelRX - 6 - 45, _y + PPanelRY + ryInc, 2);
			resManager->drawEquipmentSlot(_x + PPanelRX - 6 - 45, _y + PPanelRY + (ryInc * 2), 5);
			resManager->drawEquipmentSlot(_x + PPanelRX + PPanelWidth + 6, _y + PPanelRY, 3);
			resManager->drawEquipmentSlot(_x + PPanelRX + PPanelWidth + 6, _y + PPanelRY + ryInc, 4);
			resManager->drawEquipmentSlot(_x + PPanelRX + PPanelWidth + 6, _y + PPanelRY + (ryInc * 2), 6);

			drawComponents();
		}
	}
}