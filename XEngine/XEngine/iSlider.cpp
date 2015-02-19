
#include "iSlider.h"
#include "Utilities.h"
#include "glm.hpp"
#define SLIDER_SIZE 10
#define SLIDER_HEIGHT 2
#define KNOB_SIZE 15
const int yOffset = 15;
bool iSlider::enable;
iSlider::iSlider(char* _name, float _min, float _max, int posX, int posY, int w, int h, int _baseTexID, int _knobID, int _bgID):Inspector(posX, posY, width, height)
{
	baseVal =  0.0f;
	current = NULL;
	min = _min;
	max = _max;
	width = w;
	height = h;
	isSlide = false;
	name = _name;

	baseTexID = _baseTexID;
	 knobID = _knobID;
	 bgID = _bgID;

	 posYdyK = (height - KNOB_SIZE) / 2;
	 posYNdyK = (height + KNOB_SIZE) / 2;
}


void iSlider::setVal(float *value)
{
	
	current = value;
	if (current!= NULL)
		baseVal = *current;
}
bool iSlider::updateMouse(MouseInput &input)
{
	Inspector::updateMouse(input);

	int x = input.x;
	int y = input.y;

	if (isHover == true)
	{
		if (input.LeftMouseButton)
		{
			isSlide = true; 
		}
		if (input.RightMouseButton)
		{
			*current = baseVal;
		}

	}
	
	if (!input.LeftMouseButton)
		isSlide = false;

	if (isSlide == true)      //holding it down
	{
		if (current!=NULL)
		*current = glm::clamp((x - position.x) / (float)width* (max - min) + min,min,max);
	}

	return isSlide;
}
void iSlider::drawInspectorUnit(){
	if (enable)
	{


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);


		if (current != NULL)
		{
			glColor4f(0.8f, .8f, .8f, .8f);
			glBindTexture(GL_TEXTURE_2D, bgID);
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex2d(position.x + width, position.y);
			glTexCoord2f(0.0f, 0.0f); glVertex2d(position.x, position.y);
			glTexCoord2f(0.0f, 1.0f); glVertex2d(position.x, position.y + height);
			glTexCoord2f(1.0f, 1.0f); glVertex2d(position.x + width, position.y + height);
			glEnd();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, baseTexID);
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex2d(position.x + width, position.y + posYdyK + 5);
			glTexCoord2f(0.0f, 0.0f); glVertex2d(position.x, position.y + posYdyK + 5);
			glTexCoord2f(0.0f, 1.0f); glVertex2d(position.x, position.y + posYNdyK - 5);
			glTexCoord2f(1.0f, 1.0f); glVertex2d(position.x + width, position.y + posYNdyK - 5);
			glEnd();

			int _currXpos = (int)((*current - min) / (max - min) * width) + position.x;

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, knobID);
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex2d(_currXpos + KNOB_SIZE, position.y + posYdyK);
			glTexCoord2f(0.0f, 0.0f); glVertex2d(_currXpos, position.y + posYdyK);
			glTexCoord2f(0.0f, 1.0f); glVertex2d(_currXpos, position.y + posYNdyK);
			glTexCoord2f(1.0f, 1.0f); glVertex2d(_currXpos + KNOB_SIZE, position.y + posYNdyK);
			glEnd();


			glColor4f(0.7, 0.7f, 0.7, 1.0f);


			glDisable(GL_TEXTURE_2D);
			char val[250];
			sprintf(val, "%s :%f", name, *current);
			XUtils->glPrintf(position.x + yOffset, position.y + yOffset, GLUT_BITMAP_8_BY_13, val);
		}
	}
}
char* iSlider::getName(){
	return "iSlider";
}