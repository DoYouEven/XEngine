#pragma once
#ifndef TERRAIN_LOADER_H
#define TERRAIN_LOADER_H
#include <string>
#include <vector>
#include <windows.h>
#ifndef GL_H
#ifndef GLEW_H
#define GLEW_H
#include <glew.h>
#endif
#endif
#ifndef GL_H
#define GL_H
#include <gl\GL.h>
#include <gl\GLU.h>
#endif
#include <glut.h>
#include <SOIL.h>
#include <sstream>
#include <fstream>
#include "glm.hpp"
#include <type_ptr.hpp>
#include <swizzle.hpp>
#include <quaternion.hpp>
#include <std_based_type.hpp>
#include <vector_angle.hpp>
#include <compatibility.hpp>
#include <matrix_operation.hpp>
#include <transform.hpp>
#include <quaternion.hpp>
#include <euler_angles.hpp>

#include <stdio.h>
#include <iostream>


//I had intially coded Terrain.h as basic terrain loaded which is included in this project, but was very unhappy with the outcome.
// After Following 4-6 tutorials on terrains, I put together a full fledged terrain loader
//Able to generate multi textures based on height,also highly optimized since uses vbo buffers
//Able to get normals and height at different positions in space;
//Most of this code here was reffered from tutorials 
using std::string;
using std::vector;
using std::istringstream;
using std::ifstream;
using glm::vec3;
using glm::vec4;
using glm::vec2;
using glm::mat4x4;
using glm::u32vec2;
using glm::clamp;
using namespace std;
class TerrainLoader
{
public:

	TerrainLoader(float width, float height);
	
	virtual ~TerrainLoader();

	void Terminate();

	bool LoadHeightmap(const string &filename, unsigned char bpp, unsigned int width, unsigned int height);
	bool LoadTexture(const string &filename, unsigned texture);
	vec3 TerrainLoader::NormalAt(const glm::vec3& position);
	float HeightAt(const vec3&position);

	void Render();
	void DebugRender();

	void GenerateIndexBuffer();

	void GenerateNormals();

	void GenerateVertexBuffers();
	void RenderNormals();
private:

	 vector<vec3> PositionBuffer;
	vector<vec4> ColorBuffer;
vector<vec3> NormalBuffer;
vector<vec2> Tex0Buffer;
vector<GLuint> IndexBuffer;



	// ID's for the VBO's
	GLuint GLVertexBuffer;
	GLuint GLNormalBuffer;
	GLuint GLColorBuffer;
	GLuint GLTex0Buffer;
	GLuint GLTex1Buffer;
	GLuint GLTex2Buffer;
	GLuint GLIndexBuffer;

	static const unsigned int uiNumTextures = 3;
	GLuint GLTextures[uiNumTextures];

	mat4x4 LocalToWorldMatrix;
	mat4x4 InverseLocalToWorldMatrix;
	
	u32vec2 HeightmapDimensions;


	float fHeightScale;

	float fBlockScale;
};

#endif