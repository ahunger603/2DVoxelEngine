#include "Menu.h"

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------MENU
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//


Menu::Menu(GameState* gameState, int x, int y, int w, int h)
{
	_gameState = gameState;
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	mx = -1;
	my = -1;
}

Menu::~Menu()
{
	//Destroy Components
	auto compEnd = components.end();
	for (auto it = components.begin(); it != compEnd; it++)
	{
		MenuComponent* component = (*it);
		
		delete component;
	}
	components.clear();
}

//Protected Funtions

//Get Set
void Menu::getMouseXY(int* xy)
{
	xy[0] = mx;
	xy[1] = my;
}

void Menu::handleMouseEvents()
{
	//Get Raw Mouse Coords
	InputManager* inputManager = _gameState->getGameEngine()->getInputManager();
	int rawMouseXY[2] = { 0, 0 };
	inputManager->getMouseXY(rawMouseXY);

	//Check if in menu
	if (inMenu(rawMouseXY[0], rawMouseXY[1]))
	{
		//Set Mouse Coords
		mx = rawMouseXY[0];
		my = rawMouseXY[1];
	}
	else
	{
		//Null Mouse Coords
		mx = -1;
		my = -1;
	}

	//Pass Event to components
	auto compEnd = components.end();
	for (auto it = components.begin(); it != compEnd; it++)
	{
		MenuComponent* component = (*it);
		if (component != nullptr)
			component->mouseEvent(mx, my);
	}
}

void Menu::updateComponents()
{
	//Pass Event to components
	auto compEnd = components.end();
	for (auto it = components.begin(); it != compEnd; it++)
	{
		MenuComponent* component = (*it);
		component->update();
	}
}

void Menu::drawComponents()
{
	//Pass Event to components
	auto compEnd = components.end();
	for (auto it = components.begin(); it != compEnd; it++)
	{
		MenuComponent* component = (*it);
		component->draw();
	}
}

//Public Functions

//Get Set
GameState* Menu::getGameState() { return _gameState; }

void Menu::getXY(int* xy)
{
	xy[0] = _x;
	xy[1] = _y;
}

void Menu::setXY(int xx, int yy)
{
	_x = xx;
	_y = yy;
}

bool Menu::inMenu(int xx, int yy) //Top Left Origin
{
	int _x2 = _x + _w;
	int _y2 = _y + _h;

	if (xx > _x && xx < _x2 && yy > _y && yy < _y2)
		return true;

	return false;
}

void Menu::addComponent(MenuComponent* component)
{
	components.push_back(component);
}

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------MENU COMPONENT
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

int MenuComponent::menuCompCount = 0;

MenuComponent::MenuComponent(int rx, int ry, int w, int h, bool centered, Menu* parentMenu)
{
	_parentMenu = parentMenu;
	id = menuCompCount++;

	_w = w;
	_h = h;

	if (!centered)
	{
		_rx = rx;
		_ry = ry;
	}
	else
	{
		_rx = rx - w / 2;
		_ry = ry - h / 2;
	}
}

void MenuComponent::setWidth(int w)
{
	_w = w;
}

void MenuComponent::setHeight(int h)
{
	_h = h;
}

void MenuComponent::getXY(int* xy)
{
	int parentXY[2] = { 0, 0 };
	_parentMenu->getXY(parentXY);

	xy[0] = _rx + parentXY[0];
	xy[1] = _ry + parentXY[1];
}

void MenuComponent::getRXY(int* rxy)
{
	rxy[0] = _rx;
	rxy[1] = _ry;
}

int MenuComponent::getID() { return id; }

int MenuComponent::getWidth() { return _w; }

int MenuComponent::getHeight() { return _h; }

bool MenuComponent::inMenuComponent(int xx, int yy)
{
	int _rx2 = _rx + _w;
	int _ry2 = _ry + _h;

	int parentXY[2] = { 0, 0 };
	_parentMenu->getXY(parentXY);

	int rxx = xx - parentXY[0];
	int ryy = yy - parentXY[1];

	if (rxx > _rx && rxx < _rx2 && ryy > _ry && ryy < _ry2)
		return true;

	return false;
}

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------BUTTON
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//


Button::Button(int rx, int ry, int w, int h, bool centered, Menu* parentMenu) : MenuComponent(rx, ry, w, h, centered, parentMenu)
{
	hover = false;

	pressing = false;

	intentPress = false;
	intentRelease = false;
}

void Button::mouseEvent(int xx, int yy)
{
	if (xx == -1 || yy == -1)
	{
		//Reset Buttons Values
		hover = false;
	}
	else
	{
		//In Menu
		//Check if in button
		if (inMenuComponent(xx, yy))
		{
			hover = true;

			Mouse* mouse = _parentMenu->getGameState()->getGameEngine()->getInputManager()->getMouse();

			pressing = (mouse->isLeftDown() || mouse->isRightDown());

			if (mouse->isLeftPressed() || mouse->isRightPressed())
				intentPress = true;

			if (mouse->isLeftReleased() || mouse->isRightReleased())
				intentRelease = true;
		}
		else
		{
			hover = false;
			pressing = false;
		}
	}
}

void Button::update()
{
	if (intentPress)
	{
		pressed();
		intentPress = false;
	}
	if (intentRelease)
	{
		released();
		intentRelease = false;
	}
}

void Button::draw()
{
	if (!hover)
	{
		drawInactive();
	}
	else
	{
		if (!pressing)
		{
			drawHover();
		}
		else
		{
			drawPressed();
		}
	}
}