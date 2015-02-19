#ifndef IBUTTON_H
#define IBUTTON_H

#include "Inspector.h"
#include <Windows.h>

#include <gl\GL.h>

class iButton : public Inspector
{
public:
	iButton(char* name, int posX, int posY, int width, int height, int basetexID,int texIDHover);

	int baseTexID,texIDHover;
	virtual bool updateMouse(MouseInput &input);
	virtual void drawInspectorUnit();
	virtual char* getName();
	

	bool isPressed;
};

#endif