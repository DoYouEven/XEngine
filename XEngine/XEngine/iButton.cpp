#include "iButton.h"
#include "Utilities.h"

iButton::iButton(char* _name, int posX, int posY, int width, int height, int texID,int _texIDhover) :Inspector(posX, posY, width, height)
{
	name = _name;
	baseTexID = texID;
	texIDHover = _texIDhover;
	isPressed = false;
	type = IButton;
}

bool iButton::updateMouse(MouseInput &input)

{
	Inspector::updateMouse(input);

	if (isHover == true)
	{
		if (input.LeftMouseButton)
		{
			isPressed = true;
		}
		else if (isPressed)
		{
			isPressed = false;

			return true;
		}
	}
	return false;
}
void iButton::drawInspectorUnit() {
	 
	 //glEnable(GL_BLEND);
	 //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 glEnable(GL_TEXTURE_2D);
	
	 glColor4f(1.0, 1.0, 1.0, 1.0);
	 if (isHover== true)
	 {
		 glBindTexture(GL_TEXTURE_2D, baseTexID);
	
	 }
	 else

	 glBindTexture(GL_TEXTURE_2D, texIDHover);
	 glBegin(GL_QUADS);
	 glTexCoord2f(1.0f, 0.0f); glVertex2d(position.x + width, position.y);
	 glTexCoord2f(0.0f, 0.0f); glVertex2d(position.x, position.y);
	 glTexCoord2f(0.0f, 1.0f); glVertex2d(position.x, position.y + height);
	 glTexCoord2f(1.0f, 1.0f); glVertex2d(position.x + width, position.y + height);
	 glEnd();


	 if (isHover == true)
		 glColor3f(0.1, 0.9f, 0.1);
	 else
	 glColor4f(0.7, 0.7f, 0.7, 1.0f); 

	 int textX = position.x + 5;
	 int textY = position.y + (height) / 2;
	 glDisable(GL_TEXTURE_2D);
	 XUtils->glPrintf(textX, textY, GLUT_BITMAP_TIMES_ROMAN_10, name);
	 glDisable(GL_BLEND);
	 //printf here;

}
char* iButton::getName()
{
	return "iButton";
}