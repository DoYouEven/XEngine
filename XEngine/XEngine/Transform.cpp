#include "Transform.h"


Transform::Transform() :Node()
{
	scale = vec3(1, 1, 1);
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
}
void  Transform::Translate(vec3 vec)
{
	position.x = vec.x;
	position.y = vec.y;
	position.z = vec.z;
}
void  Transform::Rotation(vec3 vec)
{
	rotation.x = vec.x  ;
	rotation.y = vec.y  ;
	rotation.z = vec.z ;
}
void  Transform::Scale(vec3 vec)

{
	scale.x = vec.x;
	scale.y = vec.y;
	scale.z = vec.z;
}
	 