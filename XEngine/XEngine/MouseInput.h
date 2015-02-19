#ifndef MI_H
#define MI_H
#include <SDL\SDL.h>
struct  MouseInput
{
	bool LeftMouseButton;
	bool RightMouseButton;
	bool MiddleButton;
	bool ScrollUp;
	bool ScrollDown;
	int x;
	int y;
	char c;
	SDL_Event event;
	bool keydown;
	MouseInput()
	{
		LeftMouseButton = 0;
		RightMouseButton = 0;
		MiddleButton = 0;
	}

};

#endif