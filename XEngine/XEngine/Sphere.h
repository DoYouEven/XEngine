#ifndef SPHERE_H
#define SPHERE_H

#include "BasePrimitive.h"
#include "Mesh.h"
#define PI  3.14592635

class Sphere : public BasePrimitive
{
public:
	static int sphereID;
	float radius;
	int stacks;
	int slices;
	void Init(float radius, float stacks, float  slices);
	void create(char* name = "");

};
#endif