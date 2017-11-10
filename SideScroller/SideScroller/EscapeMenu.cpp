#include "EscapeMenu.h"

const int BTTN_W = 200;
const int BTTN_H = 50;

//Menu-----------------------------------
EscapeMenu::EscapeMenu(GameState* gameState,int x, int y, int w, int h) : Menu(gameState, x, y, w, h)
{
	addComponent(new ResumeButton((BTTN_W / 2) + ((w - BTTN_W) / 2), (h - (BTTN_H / 2) - 10), this));
	addComponent(new ExitGameButton((BTTN_W / 2) + ((w - BTTN_W) / 2), 205, this));
}

EscapeMenu::~EscapeMenu(){}

void EscapeMenu::handleEvents()
{
	handleMouseEvents();
}

void EscapeMenu::update()
{
	updateComponents();
}

void EscapeMenu::draw()
{
	_gameState->getGameEngine()->getResManager()->drawMenu(_x, _y, _w, _h, false, -1);

	drawComponents();
}


//Menu Components------------------------

ResumeButton::ResumeButton(int x, int y, Menu* parentMenu) : Button(x, y, BTTN_W, BTTN_H, true, parentMenu)
{
	text = "Resume";

	inactiveTexture = NULL;
	hoverTexture = NULL;
	pressedTexture = NULL;
}

ResumeButton::~ResumeButton()
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

void ResumeButton::pressed(){}

void ResumeButton::released()
{
	//Set Menu Inactive
	_parentMenu->getGameState()->setActivity(false);
}

void ResumeButton::drawInactive()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (inactiveTexture == NULL)
	{
		/*
		//Inactive
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(LIGHT_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 48), text.c_str(), SDL_COLORS.at(LIGHT_GREY));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		inactiveTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(inactiveTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, inactiveTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(inactiveTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, inactiveTexture, NULL, &button);
	}
}
void ResumeButton::drawHover()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (hoverTexture == NULL)
	{
		/*
		//Hovering
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(LIGHT_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 48), text.c_str(), SDL_COLORS.at(WHITE));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(hoverTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, hoverTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(hoverTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, hoverTexture, NULL, &button);
	}
}
void ResumeButton::drawPressed()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (pressedTexture == NULL)
	{
		//Pressing
		/*
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(DARK_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 48), text.c_str(), SDL_COLORS.at(GREY));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		pressedTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(pressedTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, pressedTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(pressedTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, pressedTexture, NULL, &button);
	}

}


//EXIT GAME BUTTON---------------------------------------------------------------------

ExitGameButton::ExitGameButton(int x, int y, Menu* parentMenu) : Button(x, y, BTTN_W, BTTN_H, true, parentMenu)
{
	text = "Exit Game";

	inactiveTexture = NULL;
	hoverTexture = NULL;
	pressedTexture = NULL;
}

ExitGameButton::~ExitGameButton()
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

void ExitGameButton::pressed(){}

void ExitGameButton::released()
{
	//Set Menu Inactive
	_parentMenu->getGameState()->getGameEngine()->Quit();
}

void ExitGameButton::drawInactive()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (inactiveTexture == NULL)
	{
		/*
		//Inactive
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(LIGHT_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 40), text.c_str(), SDL_COLORS.at(LIGHT_GREY));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		inactiveTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(inactiveTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, inactiveTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(inactiveTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, inactiveTexture, NULL, &button);
	}
}
void ExitGameButton::drawHover()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (hoverTexture == NULL)
	{
		/*
		//Hovering
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(LIGHT_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 40), text.c_str(), SDL_COLORS.at(WHITE));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(hoverTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, hoverTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(hoverTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, hoverTexture, NULL, &button);
	}
}
void ExitGameButton::drawPressed()
{
	int XY[2] = { 0, 0 };
	getXY(XY);

	SDL_Renderer* renderer = _parentMenu->getGameState()->getGameEngine()->getRenderer();
	SDL_Rect button = { XY[0], XY[1], _w, _h };

	if (pressedTexture == NULL)
	{
		/*
		//Pressing
		ResourceManager* resManager = _parentMenu->getGameState()->getGameEngine()->getResManager();
		SDL_Surface* surface = resManager->CreateColorSurface(BTTN_W - 10, BTTN_H, HEX_COLORS.at(DARK_GREY));
		SDL_Surface* buttonSurf = TTF_RenderText_Solid(resManager->getFont(1, 40), text.c_str(), SDL_COLORS.at(GREY));
		SDL_Rect r = { (surface->w - buttonSurf->w) / 2, (surface->h - buttonSurf->h) / 2, buttonSurf->w, buttonSurf->h };
		SDL_BlitSurface(buttonSurf, NULL, surface, &r);
		pressedTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(pressedTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, pressedTexture, NULL, &button);

		SDL_FreeSurface(buttonSurf);
		*/
	}
	else
	{
		SDL_QueryTexture(pressedTexture, NULL, NULL, &button.w, &button.h);

		button.x += ((_w - button.w) / 2);
		button.y += ((_h - button.h) / 2);

		SDL_RenderCopy(renderer, pressedTexture, NULL, &button);
	}

}