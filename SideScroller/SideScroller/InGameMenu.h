#ifndef INGAMEMENU_H
#define INGAMEMENU_H

#include "InputManager.h"
#include "Menu.h"

class Menu;
class MenuComponent;

const int TITLE_TAB_HEIGHT = 37;

class InGameMenu : public Menu
{
protected:
	int titleTextureID;
	bool _static;
	bool _volatile;
	bool active;

	bool dragging;
	int last_mxy[2];
	int windowSize[2];

	KEY _hotKey;
	void defaultHandleEvents();
	void defaultUpdate();
	void defaultDraw();
public:
	InGameMenu(GameState* gameState, char* title, bool static_, bool volatile_, KEY hotKey, int x, int y, int w, int h);
	~InGameMenu();

	void Close();
	void Open();

	bool isVolatile();
	bool isActive();

	virtual void handleEvents() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------CLOSE BUTTON
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

class CloseButton : public Button
{
public:
	CloseButton(int rx, int ry, InGameMenu* parentMenu);

	void drawInactive();
	void drawHover();
	void drawPressed();

	void pressed();
	void released();
};


#endif