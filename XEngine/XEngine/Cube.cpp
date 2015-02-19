#include "Cube.h"

int Cube::cubeID = 0;
void Cube::create(char* _name)
{
	sprintf(name, "%s%d", "Cube ", cubeID);
	cubeID++;
	addVertex(vec3(-1, -1, -1));//0
	addVertex(vec3(1, -1, -1));//1
	addVertex(vec3(1, -1, 1));//2
	addVertex(vec3(-1, -1, 1));//3
	addVertex(vec3(-1, 1, -1));//4
	addVertex(vec3(1, 1, -1));//5
	addVertex(vec3(1, 1, 1));//6
	addVertex(vec3(-1, 1, 1));//6

	addNormal(vec3(1, 0, 0));
	addNormal(vec3(-1, 0, 0));
	addNormal(vec3(1, 1, 0));
	addNormal(vec3(0, -1, 0));
	addNormal(vec3(0, 0, 1));
	addNormal(vec3(0, 0, -1));

	addTexCoord(vec3(0, 0, 0));
	addTexCoord(vec3(0, 1, 0));
	addTexCoord(vec3(1, 0, 0));
	addTexCoord(vec3(1, 1, 0));

	//-Z;
	addFace(vec3(0, 4, 5), vec3(5, 5, 5), vec3(0, 1, 2));
	addFace(vec3(5, 1, 0), vec3(5, 5, 5), vec3(3, 2, 0));

	//+Z;
	addFace(vec3(2, 6, 7), vec3(4, 4, 4), vec3(0, 1, 2));
	addFace(vec3(7, 3, 2), vec3(4, 4, 4), vec3(3, 2, 0));
	//-Y
	addFace(vec3(0, 1, 2), vec3(3, 3,3), vec3(0, 1, 2));
	addFace(vec3(2, 3, 0), vec3(3, 3, 3), vec3(3, 2, 0));
	//+Y
	addFace(vec3(5, 4, 7), vec3(2, 2, 2), vec3(0, 1, 2));
	addFace(vec3(7, 6, 5), vec3(2, 2, 2), vec3(3 ,2, 0));
	//-X
	addFace(vec3(3, 7, 4), vec3(1, 1, 1), vec3(0, 1, 2));
	addFace(vec3(4, 0, 3), vec3(1, 1, 1), vec3(3, 2, 0)); 

	addFace(vec3(1, 5, 6), vec3(0, 0, 0), vec3(0, 1, 2));
	addFace(vec3(6, 2, 1), vec3(0, 0, 0), vec3(3, 2, 0));
	
	

}