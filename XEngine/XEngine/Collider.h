#include "glm.hpp"
#include <cmath>
#ifndef Collider_H
#define Collider_H
using glm::vec3;
class Collider{
public:
	static bool sphereplane(vec3& sp, vec3 pn, vec3 p1, vec3 p2, vec3 p3, vec3 p4, float r);
	static bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, float* dist = NULL, vec3* collpoint = NULL);
	static bool rayplane(const float& nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd, vec3 p1, vec3 p2, vec3 p3, vec3 p4, float* dis = NULL, vec3* point = NULL);
	static float rayplanedist(float nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd);
	static float trianglearea(vec3 p1, vec3 p2, vec3 p3);
	static bool spheresphere(vec3& c1, float r1, vec3& c2, float r2);
	static float pointdistacesquare(vec3 p1, vec3 p2);
	static float pointdistace(vec3 p1, vec3 p2);

	/*	static bool sphereplane(vec3& sp,const vec3& pn,const vec3& p1,const vec3& p2,const vec3& p3,const vec3& p4, const float& r);
	static bool raysphere(const vec3& center,const vec3& direction,const vec3& startpoint,const float& r);
	static bool rayplane(const vec3& normal,const vec3& startpoint,const vec3& direction,const vec3& p1,const vec3& p2,const vec3& p3,const vec3& p4);
	static float trianglearea(const vec3& p1,const vec3& p2,const vec3& p3);
	static bool spheresphere(const vec3& c1,float r1,const vec3& c2,float r2);*/
};
#endif