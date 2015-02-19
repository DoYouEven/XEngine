#include "Sphere.h"


//**************************************AS per Implementation provied by  PUDN**************************(http://read.pudn.com/)
//*************************************LOGIC converted to be able to be used within the engine framework***

int Sphere::sphereID = 0;
void Sphere::Init(float _radius, float _stacks, float  _slices)
{
	radius = _radius;
	stacks = _stacks;
	slices = _slices;

}
void Sphere::create(char* _name)
{
	sprintf(name, "%s%d", "Sphere ", sphereID);
	sphereID++;
	setMeshType(SPHERE);
	
	InitSphere(radius, stacks, slices);


}