#include "MouseInput.h"
#include <list>
#include <string>

using std::list;
using std:: string;

class BoxCollider
{

public:
	static list<BoxCollider*> BoxColliders;
	BoxCollider(int _posX, int _posY, int w, int h);

	virtual bool updateControl(MouseInput &input);
	virtual void drawControl(void) = 0;

	virtual string Type(void) = 0;

	void setPos(int x, int y);

	void setSize(int width, int height);

	bool isHover;

	int posX, posY;

	int width, height;

	int getWidth();
	int getHeight();
	


};
BoxCollider *addBoxCollider(BoxCollider *boxCollider);