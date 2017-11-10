#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <cmath>

#include "SDL.h"

const int VIEW_CHUNKS_WIDTH = 5;
const int VIEW_CHUNKS_HEIGHT = 3;

class Camera
{
private:
	int _screenW;
	int _screenH;
public:
	SDL_Rect view;

	//Constructor
	Camera(int* xy, const int screenW,const int screenH);

	//Pos
	void setPosByCenter(int* xy);
	bool inView(SDL_Rect* rect);
	int getDrawX(int x);
	int getDrawY(int y);
	void getViewXY(int* xy);
};

#endif