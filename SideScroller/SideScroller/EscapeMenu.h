#ifndef ESCAPE_MENU_H
#define ESCAPE_MENU_H

#include "Menu.h"
#include "PlayState.h"

class Menu;
class MenuComponent;

//Menu-----------------------------------
class EscapeMenu : public Menu
{
private:

public:
	EscapeMenu(GameState* gameState, int x, int y, int w, int h);
	~EscapeMenu();

	void handleEvents();

	void update();
	void draw();
};

//Resume Button
class ResumeButton : public Button
{
private:
	SDL_Texture* inactiveTexture;
	SDL_Texture* hoverTexture;
	SDL_Texture* pressedTexture;
	std::string text;
public:
	ResumeButton(int x, int y, Menu* parentMenu);
	~ResumeButton();

	void drawInactive();
	void drawHover();
	void drawPressed();

	void pressed();
	void released();
};

//Exit Game Button
class ExitGameButton : public Button
{
private:
	SDL_Texture* inactiveTexture;
	SDL_Texture* hoverTexture;
	SDL_Texture* pressedTexture;
	std::string text;
public:
	ExitGameButton(int x, int y, Menu* parentMenu);
	~ExitGameButton();

	void drawInactive();
	void drawHover();
	void drawPressed();

	void pressed();
	void released();
};


#endif