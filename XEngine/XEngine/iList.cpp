#include "iList.h"
#include "Utilities.h"
const int h = 18;
const int yOffset =10 ;
iList::iList(int posX, int posY, int width, int height) :Inspector(posX, posY, width, height)
{
	index = -1;
	type = IList;
}

int iList::add(char* item)
{
	items.push_back(item);

	return 1;
}
void iList::remove(int index)
{
	int i = 0;
	for (vector<char*>::iterator it = items.begin(); it != items.end(); it++)
	{
		if (i == index)
		{
			items.erase(it);
			break;
		}
		i++;
	}

	if (index >= items.size())
	{
		index = items.size() - 1;
	}
}
void iList::setSelected(int _index)
{
	index = _index;
}

int iList::getIndex(){
	return index;
}
int iList::getCount(){
	return items.size();
}

bool iList::updateMouse(MouseInput &input){

	Inspector::updateMouse(input);
	int x = input.x;
	int y = input.y;
	
	if (isHover == true && input.LeftMouseButton)
	{
		int temp = (y - position.y) / h;

		if (temp >= 0 && temp < items.size())
		{
			index = temp;
			return true;
		}
	}

	return false;

}
void iList::drawInspectorUnit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, baseTexID);
	/*

		glColor4f(0.8f, 0.8f, 0.8f, 0.7f);
	glBegin(GL_QUADS);
	 glVertex2d(position.x + width, position.y);
 glVertex2d(position.x, position.y);
	glVertex2d(position.x, position.y + height);
 glVertex2d(position.x + width, position.y + height);
	glEnd();
	*/
	/*
		glLineWidth(8.0f);
		glColor4f(1.0f,1.0f,1.0f, 0.8f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(position.x + width, position.y);
	glVertex2d(position.x, posY);
	glVertex2d(position.x, posY + height);
	glVertex2d(position.x + width, position.y + height);
	glVertex2d(position.x + width, position.y);
	glEnd();
	*/

	if (index >= 0)
	{
	
		int currY = position.y + index *h;
		glBegin(GL_QUADS);
		 glVertex2d(position.x + width, currY);
		 glVertex2d(position.x, currY);
		 glVertex2d(position.x, currY + h);
	 glVertex2d(position.x + width, currY + h);
		glEnd();

		glColor4f(0.2, 0.8f, 0.1, 1.0f);
	}
	else
	glColor4f(1.0, 1.0f, 1.0, 1.0f);
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < items.size() ; i++)
		XUtils->glPrintf(position.x + yOffset, position.y + yOffset + i*h, GLUT_BITMAP_8_BY_13, items[i]);
}
char* iList::getName()
{
	return "iList";
}