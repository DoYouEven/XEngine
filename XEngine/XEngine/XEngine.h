
#ifndef XENGINE_H
#define XENGINE_H

#define GLEW_STATIC
#ifndef GL_H
#ifndef GLEW_H
#define GLEW_H
#include <glew.h>
#endif
#endif
#include"Light.h"
#include"Camera.h"
#include "Skybox.h"
#include <SDL\SDL.h>
#include <SDL\SDL_mixer.h>
#include "AudioSource.h"
#include "Texture.h"
#include "ObjectLoader.h"
#include "Terrain.h"
#include "TerrainLoader.h"
#include "Inspector.h"
#include "iButton.h"
#include "Mesh.h"
#include "iList.h"
#include "iSlider.h"
#include "iTexture.h"
#include "iTextBox.h"
#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#define MAXROTXSPEED 0.5;
#define MAXROTYSPEED 0.5;
#define MAXZOOMSPEED 0.05;
/*
CSC 418 END OF TERM PROJECT
Wrapper List of relevent  class and 
includes all header files
This engine has been created over time during the semester
with many help thanks to various tutorials and through the 
thorough understanding of the Unity Engine. You may observer
alot of the functions i wrote were similar to the Unity engine in
terms of naming as it is my native gaming engine that I use to create games

You can import the engine into your project
thus 

Reuben Perira
*/
//Wrapper Classes that will
#define X_Engine XEngine ::getXEngine()
class XEngine
{
public :
	XEngine();
	~XEngine();
	void initGL(GLsizei width, GLsizei height);
	void establishProjecttionMatrix(GLsizei width, GLsizei height);
	static GLvoid UnInit();
	static XEngine *getXEngine();
	void Init(GLint w, GLint h);
};
#endif