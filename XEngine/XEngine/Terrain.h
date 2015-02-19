#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include<vector>
#include<Windows.h>
#include <SDL\SDL.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <fstream>
#ifndef TEXTURE_H
#include "Texture.h"
#endif
#ifndef VECTOR_H
	#include "Vector3.h"
#endif
using std::vector;
using std::string;
using std::fstream;

class Terrain
{
public:
	Terrain(const char* filename,int width,int height,string name= "");
	~Terrain();

public:

	unsigned int width;
	unsigned int height;
	void renderTerrain();
	string name;
	static vector<Terrain*> terrains;
private:
	Texture *terrainTex;
	vector<vector<Vector3>> vert1;
	vector<vector<Vector3>> vert2;
	vector<vector<Vector3>> vert3;
	vector<vector<Vector3>> vert4;
	vector<vector<Vector3>> normals;
	vector<vector<float>> heights;
	bool loadTerrain(string filename);
	GLuint glList;



	
};

#endif