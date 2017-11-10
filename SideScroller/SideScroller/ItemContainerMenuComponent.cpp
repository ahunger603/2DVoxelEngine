#include "ItemContainerMenuComponent.h"

ItemContainerMenuComponent::ItemContainerMenuComponent(InGameMenu* parentMenu, ItemContainer* container, int rx, int ry) : MenuComponent(rx, ry, 0, 0, false, parentMenu)
{
	_container = container;

	_w = _container->getWidth();
	_h = _container->getHeight();

	setWidth(_w * (SLOT_SIZE + SPACER));
	setHeight(_h * (SLOT_SIZE + SPACER));

	holding_gr_xy[0] = -1;
	holding_gr_xy[1] = -1;
}

ItemContainerMenuComponent::~ItemContainerMenuComponent()
{
	
}

void ItemContainerMenuComponent::mouseEvent(int x, int y){}

void ItemContainerMenuComponent::itemContainerMouseEvent(int x, int y, TemporaryItemHolder& holder)
{
	if (x != -1 && y != -1)
	{
		if (inMenuComponent(x, y))
		{
			int xy[2] = { 0, 0 };
			_parentMenu->getXY(xy);

			int slot_x = 0;
			int slot_y = 0;

			for (int yy = 0; yy < _h; yy++)
			{
				for (int xx = 0; xx < _w; xx++)
				{
					slot_x = xy[0] + _rx + (xx * (SLOT_SIZE + SPACER));
					slot_y = xy[1] + _ry + (yy * (SLOT_SIZE + SPACER));
					if (x >= slot_x && y >= slot_y && x <= (slot_x + SLOT_SIZE) && y <= (slot_y + SLOT_SIZE))
					{
						Mouse* m = _parentMenu->getGameState()->getGameEngine()->getInputManager()->getMouse();
						if (m->isRightPressed())
						{
							holding_gr_xy[0] = xx;
							holding_gr_xy[1] = yy;
						}

						if (m->isRightReleased())
						{
							if (holding_gr_xy[0] != -1 && holding_gr_xy[1] != -1)
								_container->swapItemSlots(xx, yy, holding_gr_xy[0], holding_gr_xy[1]);

							holding_gr_xy[0] = -1;
							holding_gr_xy[1] = -1;
						}
					}
				}
			}
		}
		else
		{
			Mouse* m = _parentMenu->getGameState()->getGameEngine()->getInputManager()->getMouse();
			if (m->isRightReleased())
			{
				holding_gr_xy[0] = -1;
				holding_gr_xy[1] = -1;
			}
		}
	}
	else
	{
		Mouse* m = _parentMenu->getGameState()->getGameEngine()->getInputManager()->getMouse();
		if (m->isRightReleased())
		{
			holding_gr_xy[0] = -1;
			holding_gr_xy[1] = -1;
		}
	}
}

void ItemContainerMenuComponent::update()
{

}

void ItemContainerMenuComponent::draw()
{
	ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();

	//Inventory
	int XY[2] = { 0, 0 };
	_parentMenu->getXY(XY);
	for (int y = 0; y < _container->getHeight(); y++)
	{
		for (int x = 0; x < _container->getWidth(); x++)
		{
			if (!(x == holding_gr_xy[0] && y == holding_gr_xy[1]))
				_container->drawItemSlot(x, y, XY[0] + _rx + (x * (SLOT_SIZE + SPACER)), XY[1] + _ry + (y * (SLOT_SIZE + SPACER)), resManager);
			else
				resManager->drawItemContainerSlot(XY[0] + _rx + (x * (SLOT_SIZE + SPACER)), XY[1] + _ry + (y * (SLOT_SIZE + SPACER)), nullptr);
		}
	}

	if (holding_gr_xy[0] != -1 && holding_gr_xy[1] != -1)
	{
		Mouse* m = _parentMenu->getGameState()->getGameEngine()->getInputManager()->getMouse();
		int XY2[2] = { 0, 0 };
		m->getMouseXY(XY2);
		_container->drawItem(holding_gr_xy[0], holding_gr_xy[1], XY2[0] - (SLOT_SIZE / 2), XY2[1] - (SLOT_SIZE / 2), resManager);
	}
}

