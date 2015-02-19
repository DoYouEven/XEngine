#include"Inspector.h"
#include <SDL\SDL.h>
list<Inspector*>Inspector::InspectorUnits;
Inspector::Inspector( int posX, int posY, int w, int h)
{
	InspectorUnits.push_back(this);
	position.x = posX;
	position.y = posY;

	width = w;
	height = h;

}

Inspector:: ~Inspector()
{
	InspectorUnits.remove(this);
}

bool Inspector::updateMouse(MouseInput &input)
{

	int x = input.x;
	int y = input.y;

	//check 2d collision;

	if ((x >= position.x && x <= position.x + width) && y >= position.y  && y <= position.y + height)
	{
		isHover = true;
		return true;
	}
	else
		isHover = false;

	return false;
}

char* Inspector::getName()
{
	return "sd";
}
