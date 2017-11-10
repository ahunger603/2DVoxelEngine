#ifndef INGAMEMENUMANAGER_H
#define INGAMEMENUMANAGER_H

#include <vector>

#include "InGameMenu.h"
#include "InventoryMenu.h"

class InGameMenu;
class InventoryMenu;

class InGameMenuManager
{
private:
	std::vector<InGameMenu*> menuVector;
	std::list<int> menuEraseQueue;
public:
	InGameMenuManager();
	~InGameMenuManager();

	void addMenu(InGameMenu* menu);

	void closeAll();

	void handleEvents();
	void update();
	void draw();
};

#endif