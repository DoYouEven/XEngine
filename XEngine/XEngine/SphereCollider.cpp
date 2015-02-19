#include "SphereCollider.h"

SphereCollider::SphereCollider()
{
	radius = 0;
}
SphereCollider::SphereCollider(const Vector3& vec, float _radius)
{
	origin.update(vec);
	radius = _radius;
}
