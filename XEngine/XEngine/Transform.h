#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm.hpp"
#include "Node.h"
using glm::vec3;
class Transform : public Node
{


public:
	Transform();
	vec3 position;
	vec3 rotation;
	vec3 scale;
	void Translate(vec3 vector3);
	void Rotation(vec3 vector3);
	void Scale(vec3 vector3);
}; 
#endif