#include "Camera.h"

Camera::Camera(int* xy,const int screenW,const int screenH)
{
	_screenW = screenW;
	_screenH = screenH;

	view = { xy[0], xy[1], _screenW, _screenH };
}

void Camera::setPosByCenter(int* xy)
{
	view = { xy[0] - (_screenW / 2), xy[1] + (_screenH / 2), _screenW, _screenH };
}

bool Camera::inView(SDL_Rect* rect)
{
	int rectX2 = rect->x + rect->w;
	int rectY2 = rect->y - rect->h;
	int viewX2 = view.x + view.w;
	int viewY2 = view.y - view.h;
	
	if (rect->x > viewX2 || rect->y < viewY2 || view.x > rectX2 || view.y < rectY2)
		return false;

	return true;
}

int Camera::getDrawX(int x)
{
	return (x - view.x);
}

int Camera::getDrawY(int y)
{
	return (view.y - y);
}

void Camera::getViewXY(int* xy)
{
	xy[0] = view.x;
	xy[1] = view.y;
}