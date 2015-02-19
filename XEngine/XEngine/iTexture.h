#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "Transform.h"
#include <list>
#include <string>
using std::string;
using std::list;




class iTexture
{
public:
	iTexture(char* text, int posX, int posY, int w, int h, int iTextureID);

	// ~iTexture();
	 void drawUITexture();
	 vec3 position;
	
	char* getName();

	char* name;
	int width, height;
	int posX, posY;
	int texID;
};

#endif