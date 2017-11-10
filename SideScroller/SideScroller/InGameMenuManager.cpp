#include "InGameMenuManager.h"

InGameMenuManager::InGameMenuManager()
{

}

InGameMenuManager::~InGameMenuManager()
{
	for (auto&& menu : menuVector) {
		delete menu;
	}
}

void InGameMenuManager::addMenu(InGameMenu* menu)
{
	menuVector.push_back(menu);
}

void InGameMenuManager::closeAll()
{
	for (auto&& menu : menuVector) 
	{
		menu->Close();
	}
}

void InGameMenuManager::handleEvents()
{
	for (auto&& menu : menuVector)
	{
		menu->handleEvents();
	}
}

void InGameMenuManager::update()
{
	int i = 0;
	for (auto&& menu : menuVector)
	{
		if ((!menu->isActive()) && menu->isVolatile())
		{
			delete menu;
			menuEraseQueue.push_back(i);
		}
		else
			menu->update();

		i++;
	}

	for (int erase_index : menuEraseQueue)
	{
		menuVector.erase(menuVector.begin() + erase_index);
	}
	menuEraseQueue.clear();
}

void InGameMenuManager::draw()
{
	for (auto&& menu : menuVector) 
	{
		menu->draw();
	}
}