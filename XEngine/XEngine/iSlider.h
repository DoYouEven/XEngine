#ifndef SLIDER_H
#define SLIDER_H

#include "Inspector.h"


class iSlider : public Inspector

{

public :
	iSlider(char* name, float min, float max, int posX, int posY, int w, int h,int baseTexID,int knobID, int bgID);

	static bool enable;
	void setVal(float *value);
	virtual bool updateMouse(MouseInput &input);
	virtual void drawInspectorUnit();
	virtual char* getName();

	
	int baseTexID;
	int knobID;
	int bgID;

	 int posYdyK;
	 int posYNdyK;
	float baseVal;
	float min;
	float max;
	float *current;
	bool isSlide;
	char* name;
};
#endif