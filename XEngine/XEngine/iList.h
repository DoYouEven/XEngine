#ifndef ILIST_H
#define ILIST_H

#include "Inspector.h"
#include <Windows.h>
#include <gl\GL.h>

#include <vector>

using std::vector;
class iList : public Inspector

{
public:
	iList(int posX, int posY, int width, int height);
	
	int add( char* item);

		void remove(int index);

	void setSelected(int index);

	int getIndex();
	int getCount();

	virtual bool updateMouse(MouseInput &input);
	virtual void drawInspectorUnit();
	virtual char* getName();


	int index;
	vector<char*> items;

};

#endif