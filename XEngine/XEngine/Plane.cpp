#include "Plane.h"
int Plane::planeID = 0;
void  Plane::setSize(float w, float h)
{
	width = w;
	height = h;
}
void Plane::create(char* _name)
{
	sprintf(name, "%s%d", "Plane ", planeID);

	float dx = width / (float)divNum;
	float dy = height / (float)divNum;

	addNormal(vec3(0, 1, 0));
	addTexCoord(vec3(0,0,0));

	for (float x = -width / 2; x < width / 2; x += dx)
	{
		for (float y = -height / 2; y < height / 2; y += dy)
		{
			int v1 = addVertex( vec3(x + dx,0, y + dy));//basically triangle Strips;
			int v2 = addVertex(vec3(x , 0, y + dy));
			int v3 = addVertex(vec3(x + dx, 0, y ));
			int v4 = addVertex(vec3(x , 0, y ));

			addFace(vec3(v1, v3, v4), vec3(0, 0, 0), vec3(0, 0, 0));

			addFace(vec3(v4, v2, v1), vec3(0, 0, 0), vec3(0, 0, 0));
	

		}
	}
}