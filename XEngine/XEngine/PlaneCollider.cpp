#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
{
	p[0] = vec3(a, b, c);
	p[1] = vec3(d, e, f);
	p[2] = vec3(g, h, i);
	p[3] = vec3(j, k, l);
	normal = vec3(n1, n2, n3);
}

vec3 PlaneCollider::get1point()
{
	return p[0];
}
vec3 PlaneCollider::get2point()
{
	return p[1];
}

vec3 PlaneCollider::get3point()
{
	return p[2];
}

vec3 PlaneCollider::get4point()
{
	return p[3];
}

vec3 PlaneCollider::getnormal()
{
	return normal;
}

bool PlaneCollider::isok()
{
	return (normal.x != 0 && normal.y != 0 && normal.z != 0);
}
