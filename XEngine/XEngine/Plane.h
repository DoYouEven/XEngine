#ifndef PLANE_H
#define PLANE_H

#include "BasePrimitive.h"
#include "Mesh.h"

class Plane : public BasePrimitive
{
public :
	static int planeID;
	void    setSize(float w, float h); 
	void    setDivision(float num);
	void create(char* name = "");

	float width;
	float height;
	int divNum;
};

#endif
