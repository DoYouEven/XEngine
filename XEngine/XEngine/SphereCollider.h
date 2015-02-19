
#ifndef VECTOR3_H
#include "Vector3.h"
#endif
class SphereCollider{
public:

	float radius;

	Vector3 origin;
	SphereCollider();
	SphereCollider(const Vector3& vec, float radius);

};