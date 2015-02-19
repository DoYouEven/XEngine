#ifndef PLANE_COLLIDER_H
#define PLANE_COLLIDER_H
#include <iostream>
#include "glm.hpp";
using glm::vec3;
class PlaneCollider{
public:
	vec3 p[4];
	vec3 normal;
	PlaneCollider(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l);
	vec3 get1point();
	vec3 get2point();
	vec3 get3point();
	vec3 get4point();
	vec3 getnormal();
	bool isok();
};
#endif
