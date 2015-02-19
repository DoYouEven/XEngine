#ifndef UTILITIES_H
#define UTILITIES_H

#ifndef GL_H
#include <Windows.h>
#include <gl\GL.h>
#include<gl\GLU.h>
#include "glut.h"
#endif
#include <time.h>
#include <stdio.h>
#include <fbxsdk.h>
#define XUtils Utilities::getUtility()
#define Time Utilities::getUtility()
class Inspector;
class Utilities
{
	Utilities();
	~Utilities();
	void UnInit();

public:
	static bool EnableAxes;
	void glDisable2D(void);
	void glEnable2D(void);
	Inspector *addInspector(Inspector *Inspector);
	Inspector *addInspectorTopFrame(Inspector *Inspector);
	static Utilities *getUtility();
	float maxTimeStep = 0.03333f;
	static float deltaTime();
	void SetMouseIn(bool in);
	void glPrintf(int x, int y, void* font, const char* string, ...);
	bool MouseIn;
	static float m_fMaxTimeStep;
	static float m_fPrevious;
	void draw_axes(void);
	void draw_grid();

	GLint gridList;

};
#endif