#ifndef ITEXTBOX_H
#define ITEXTBOX_H

#include "Inspector.h"
#include <Windows.h>
#include "Utilities.h"
#include <gl\GL.h>

class iTextBox : public Inspector
{
public:
	iTextBox(char* name, int posX, int posY, int width, int height, int basetexID);

	int baseTexID;
	virtual bool updateMouse(MouseInput &input);
	virtual void drawInspectorUnit();
	virtual char* getName();


	bool isPressed;
	bool isTyping;
	string text;
	void handleKey(SDL_Event event, bool keydown);
};

#endif