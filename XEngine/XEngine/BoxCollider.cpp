#include "BoxCollider.h"


list<BoxCollider*> BoxCollider::BoxColliders;
BoxCollider::BoxCollider(int _posX, int _posY, int w, int h)
{
	BoxColliders.push_back(this);
	posX = _posX;
	posY = _posY;
	width = w;
	height = h;
}

bool BoxCollider::updateControl(MouseInput &input)
{
	int x = input.x;
	int y = input.y;
	
	isHover = false;

	if (x >= posX && x <= posX + width && y >= posY && y <= posY + height)
	{
		isHover = true;
	}
	return isHover;
}


void BoxCollider::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

void BoxCollider::setSize(int _width, int _height)
{
	width = _width;
	height = _height;
}

int BoxCollider::getWidth()
{
	return width;
}
int BoxCollider::getHeight()

{
	return height;
}

BoxCollider *addBoxCollider(BoxCollider *boxCollider)
{
	static int lastX = 5;
	static int lastY = 5;
	boxCollider->setPos(lastX, lastY);
	return boxCollider;
}

