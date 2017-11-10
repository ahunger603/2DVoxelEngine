#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "SDL.h"
#include "InputManager.h"
#include "GameEngine.h"
#include "GameState.h"
#include "Menu.h"
#include "PlayState.h"

class Menu;
class MenuComponent;

//Menu-----------------------------------
class MainMenu : public Menu
{
private:

public:
	MainMenu(GameState* gameState, int w, int h);

	void handleEvents();

	void update();
	void draw();
};

//Menu Components-------------------------

//New Game Button
class NewGameButton : public Button
{
private:
	SDL_Texture* inactiveTexture;
	SDL_Texture* hoverTexture;
	SDL_Texture* pressedTexture;
	char* text;
public:
	NewGameButton(int x, int y, Menu* parentMenu);
	~NewGameButton();

	void drawInactive();
	void drawHover();
	void drawPressed();

	void pressed();
	void released();
};

#endif