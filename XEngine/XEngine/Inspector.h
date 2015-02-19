#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "MouseInput.h"
#include "Transform.h"
#include <list>
#include <string>
#include <glm.hpp>
using std::string;
using std::list;
using glm::vec3;
#define TOP_FRAME_HEIGHT 79
#define BUTTON_WIDTH 70
#define BUTTON_HEIGHT 35
#define SLIDER_WIDTH 190
#define SLIDER_HEIGHT 15
#define TEXT_HEIGHT 13
#define TEXTBOX_HEIGHT 35
#define LR_FRAME_HEIGHT 796
enum UIType
{
	ITextBox,
	ISlider,
	IButton,
	IList
};

class Inspector 
{
public:

	vec3 position;
	Inspector(int posX, int posY, int w, int h);
	
	virtual ~Inspector();

	 virtual bool updateMouse(MouseInput &input);
	virtual void drawInspectorUnit() = 0;
	
	char* getName();
	

	static list<Inspector*> InspectorUnits;
	bool isHover;
	char* name;
	int width, height;
	int posX, posY;
	UIType type;
	int cursor;

};

#endif