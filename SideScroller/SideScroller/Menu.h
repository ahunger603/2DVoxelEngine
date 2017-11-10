#ifndef MENU_H
#define MENU_H

#include <vector>
#include "GameEngine.h"
#include "GameState.h"
#include "MenuComponent.h"

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------MENU
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

class GameEngine;
class MenuComponent;

class Menu
{
protected:
	std::vector<MenuComponent*> components;

	GameState* _gameState;
	int _x;
	int _y;
	int _w;
	int _h;

	//Mouse
	int mx;
	int my;

	void getMouseXY(int* xy);
	void handleMouseEvents();
	void updateComponents();
	void drawComponents();
public:
	Menu(GameState* gameState, int x, int y, int w, int h);

	~Menu();

	GameState* getGameState();
	void getXY(int* xy);
	void setXY(int xx, int yy);

	bool inMenu(int xx, int yy);

	void addComponent(MenuComponent* component);

	//Pure Virtual Functions
	virtual void handleEvents() = 0;

	virtual void update() = 0;
	virtual void draw() = 0;
};

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------MENU COMPONENT
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

class MenuComponent
{
protected:
	static int menuCompCount;
	int id;
	int _rx;
	int _ry;
	int _w;
	int _h;

	Menu* _parentMenu;

	void setWidth(int w);
	void setHeight(int h);
public:
	MenuComponent(int rx, int ry, int w, int h, bool centered, Menu* parentMenu);

	void getXY(int* xy);
	void getRXY(int* rxy);
	int getID();
	int getWidth();
	int getHeight();

	bool inMenuComponent(int xx, int yy);

	//Pure Virtual Functions
	virtual void mouseEvent(int xx, int yy) = 0; 

	virtual void update() = 0;
	virtual void draw() = 0;
};

//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------BUTTON
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

class Button : public MenuComponent
{
private:
	bool hover;
	bool pressing;

	bool intentPress;
	bool intentRelease;
public:
	Button(int rx, int ry, int w, int h, bool centered, Menu* parentMenu);

	void mouseEvent(int xx, int yy);

	void update();
	void draw();

	virtual void drawInactive() = 0;
	virtual void drawHover() = 0;
	virtual void drawPressed() = 0;

	virtual void pressed() = 0;
	virtual void released() = 0;
};

#endif