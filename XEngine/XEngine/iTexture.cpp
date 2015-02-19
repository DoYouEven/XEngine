
#include"iTexture.h"
#include "Utilities.h"

iTexture::iTexture(char* text, int posX, int posY, int w, int h, int iTextureID)
{
	position.x = posX;
	position.y = posY;
	name = text;
	width = w;
	height = h;
	texID = iTextureID;
}

void iTexture::drawUITexture()
{
	glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2d(position.x, position.y);
	glTexCoord2f(1.0f, 1.0f); glVertex2d(position.x + width, position.y);
	glTexCoord2f(1.0f, 0.0f); glVertex2d(position.x + width, position.y + height);
	glTexCoord2f(0.0f,0.0f); glVertex2d(position.x, position.y + height);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
