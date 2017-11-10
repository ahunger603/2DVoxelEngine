#include "InGameMenu.h"

InGameMenu::InGameMenu(GameState* gameState, char* title, bool static_, bool volatile_, KEY hotKey, int x, int y, int w, int h) : Menu(gameState, x, y, w, h)
{
	ResourceManager* resManager = gameState->getGameEngine()->getResManager();
	//titleTextureID = resManager->createText(resManager->getFont(1, 24), title, SDL_COLORS.at(WHITE));

	_static = static_;
	_volatile = volatile_;
	active = false;
	dragging = false;

	last_mxy[0] = -1;
	last_mxy[1] = -1;

	SDL_GetWindowSize(gameState->getGameEngine()->getWindow(), &windowSize[0], &windowSize[1]);

	_hotKey = hotKey;

	//Buttons
	addComponent(new CloseButton(w - 18 - 9, (TITLE_TAB_HEIGHT / 2) - 9,this));
}

InGameMenu::~InGameMenu()
{
	//_gameState->getGameEngine()->getResManager()->destroyText(titleTextureID);
}

void InGameMenu::defaultHandleEvents()
{
	InputManager* inputManager = _gameState->getGameEngine()->getInputManager();

	if (inputManager->isKeyReleased(_hotKey))
	{
		if (active)
			active = false;
		else
			active = true;
	}

	if (active)
	{
		handleMouseEvents();


		//Menu Dragging
		if (!_static)
		{
			if (mx != -1 && my != -1)
			{
				if ((my - _y) <= TITLE_TAB_HEIGHT)
				{
					if (inputManager->getMouse()->isLeftPressed())
						dragging = true;
				}

			}

			if (dragging)
			{
				Mouse* m = inputManager->getMouse();
				int mxy[2] = { 0, 0 };
				m->getMouseXY(mxy);

				if (last_mxy[0] != -1 && last_mxy[1] != -1)
				{
					_x += mxy[0] - last_mxy[0];
					_y += mxy[1] - last_mxy[1];

					if (_x < 0)
						_x = 0;
					if (_y < 0)
						_y = 0;

					if (_x + _w > windowSize[0])
						_x -= (_x + _w - windowSize[0]);
					if (_y + _h > windowSize[1])
						_y -= (_y + _h - windowSize[1]);
				}

				last_mxy[0] = mxy[0];
				last_mxy[1] = mxy[1];
			}
			else
			{
				last_mxy[0] = -1;
				last_mxy[1] = -1;
			}

			if (!(inputManager->getMouse()->isLeftDown()))
				dragging = false;
		}

		if (mx != -1 && my != -1)
		{
			Mouse* m = inputManager->getMouse();
			m->Used();
		}
	}
}

void InGameMenu::defaultUpdate()
{
	if (active)
	{
		updateComponents();
	}
}

void InGameMenu::defaultDraw()
{
	if (active)
	{
		ResourceManager* resManager = _gameState->getGameEngine()->getResManager();
		resManager->drawMenu(_x, _y, _w, _h, true, 37);
		int textDim[2] = { 0, 0 };
		//resManager->queryText(titleTextureID, &textDim[0], &textDim[1]);
		//resManager->drawText(titleTextureID, _x + (_w / 2) - (textDim[0] / 2), _y + (TITLE_TAB_HEIGHT / 2) - (textDim[1] / 2));
	}
}

void InGameMenu::Close()
{
	active = false;
}
void InGameMenu::Open()
{
	active = true;
}

bool InGameMenu::isVolatile()
{
	return _volatile;
}

bool InGameMenu::isActive()
{
	return active;
}

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------CLOSE BUTTON
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

CloseButton::CloseButton(int rx, int ry, InGameMenu* parentMenu) : Button(rx, ry, 18, 18, false, parentMenu)
{
	
}

void CloseButton::drawInactive()
{
	int xy[2] = { 0, 0 };
	_parentMenu->getXY(xy);
	_parentMenu->getGameState()->getGameEngine()->getResManager()->drawEscapeButton(xy[0] + _rx, xy[1] + _ry, 1);
}
void CloseButton::drawHover()
{
	int xy[2] = { 0, 0 };
	_parentMenu->getXY(xy);
	_parentMenu->getGameState()->getGameEngine()->getResManager()->drawEscapeButton(xy[0] + _rx, xy[1] + _ry, 2);
}
void CloseButton::drawPressed()
{
	int xy[2] = { 0, 0 };
	_parentMenu->getXY(xy);
	_parentMenu->getGameState()->getGameEngine()->getResManager()->drawEscapeButton(xy[0] + _rx, xy[1] + _ry, 3);
}

void CloseButton::pressed(){}
void CloseButton::released()
{
	((InGameMenu*)_parentMenu)->Close();
}