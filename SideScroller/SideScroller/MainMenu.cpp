#include "MainMenu.h"

//Menu-----------------------------------
MainMenu::MainMenu(GameState* gameState, int w, int h) : Menu(gameState, 0, 0, w, h)
{
	addComponent(new NewGameButton(w / 2, h / 2, this));
}

void MainMenu::handleEvents() 
{
	handleMouseEvents();
}

void MainMenu::update()
{
	updateComponents();
}

void MainMenu::draw()
{
	drawComponents();
}


//Menu Components-------------------------

//New Game Button
const int NEW_BTTN_W = 300;
const int NEW_BTTN_H = 100;

NewGameButton::NewGameButton(int x, int y, Menu* parentMenu) : Button(x, y, NEW_BTTN_W, NEW_BTTN_H, true, parentMenu)
{
	text = "NEW GAME";

	inactiveTexture = NULL;
	hoverTexture = NULL;
	pressedTexture = NULL;
}

NewGameButton::~NewGameButton()
{
	if (inactiveTexture != NULL)
	{
		SDL_DestroyTexture(inactiveTexture);
	}
	if (hoverTexture != NULL)
	{
		SDL_DestroyTexture(hoverTexture);
	}
	if (pressedTexture != NULL)
	{
		SDL_DestroyTexture(pressedTexture);
	}
}

void NewGameButton::pressed(){}

void NewGameButton::released()
{
	GameEngine* gameEngine = _parentMenu->getGameState()->getGameEngine();

	//Start Game
	gameEngine->PushState(new PlayState(gameEngine));

	//Set MainMenu Inactive
	_parentMenu->getGameState()->setActivity(false);
}

void NewGameButton::drawInactive()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();

	resManager->drawMenuPanel(XY[0], XY[1], _w, _h, 2);

	int wh[2] = { 0, 0 };
	resManager->queryText(font_ALPHABETA_48_LG, text, &wh[0], &wh[1]);
	resManager->drawText(font_ALPHABETA_48_LG, text, XY[0] + (_w / 2) - (wh[0] / 2) - 3, XY[1] + (_h / 2) - (wh[1] / 2));
}

void NewGameButton::drawHover()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();

	resManager->drawMenuPanel(XY[0], XY[1], _w, _h, 2);

	int wh[2] = { 0, 0 };
	resManager->queryText(font_ALPHABETA_48_W, text, &wh[0], &wh[1]);
	resManager->drawText(font_ALPHABETA_48_W, text, XY[0] + (_w / 2) - (wh[0] / 2) - 3, XY[1] + (_h / 2) - (wh[1] / 2));

}

void NewGameButton::drawPressed()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();


	resManager->drawMenuPanel(XY[0], XY[1], _w, _h, 2);

	int wh[2] = { 0, 0 };
	resManager->queryText(font_ALPHABETA_48_G, text, &wh[0], &wh[1]);
	resManager->drawText(font_ALPHABETA_48_G, text, XY[0] + (_w / 2) - (wh[0] / 2) - 3, XY[1] + (_h / 2) - (wh[1] / 2));
}