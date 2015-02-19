#ifndef CUBE_H
#define CUBE_H

#include "BasePrimitive.h"
#include "Mesh.h"

class Cube : public BasePrimitive
{
public :
	static int cubeID;
	void create(char* name = "");


};

#endif
