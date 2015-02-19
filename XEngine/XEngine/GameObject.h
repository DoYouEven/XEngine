#define GAMEOBJECT_H
#include <string>
#include <vector>
#ifndef MESH_H
#include "Mesh.h"
#endif

using std::string;
using std::vector; 
struct GameObject
{
	string name;
	vector <GameObject *> children;
	Mesh * mesh;
	double *matrix;
	GameObject()
	{
		mesh = NULL;
		matrix = NULL;
	}
};