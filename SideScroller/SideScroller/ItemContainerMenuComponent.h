#ifndef ITEMCONTAINERCOMP_H
#define ITEMCONTAINERCOMP_H

#include "InputManager.h"
#include "ItemContainer.h"
#include "TemporaryItemHolder.h"
#include "InGameMenu.h"

class ItemContainer;
class InGameMenu;
class TemporaryItemHolder;

class ItemContainerMenuComponent : public MenuComponent
{
private:
	int _w;
	int _h;

	int holding_gr_xy[2];
	ItemContainer* _container;

public:
	ItemContainerMenuComponent(InGameMenu* parentMenu, ItemContainer* container, int rx, int ry);
	~ItemContainerMenuComponent();

	void mouseEvent(int x, int y);
	void itemContainerMouseEvent(int x, int y, TemporaryItemHolder& holder);

	void update();
	void draw();
};

#endif