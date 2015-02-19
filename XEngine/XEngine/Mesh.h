#ifndef MESH_H
#define MESH_H
#include <string.h>

#include "Texture.h"
#include "Node.h"
#include "glm.hpp"
#include "Transform.h"
#include "ObjectLoader.h"
#include "Skybox.h"
#include "TerrainLoader.h"
using std::string;
using glm::vec3;
using glm::vec2;
struct xMaterial
{
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];

	float shininess;
	float alpha;
	float illum;

	Texture *ambientMap;
	Texture *diffuseMap;
	string name;

	xMaterial()
	{
		float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultEmission[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		memcpy(Ka, defaultAmbient, sizeof(float) * 4);
		memcpy(Kd, defaultDiffuse, sizeof(float) * 4);
		memcpy(Ks, defaultSpecular, sizeof(float) * 4);
		memcpy(Ke, defaultEmission, sizeof(float) * 4);

		diffuseMap = NULL;
		shininess - 2.0f;

	}
};

struct xFace
{
	vec3* verts[3];
	vec3* normals[3];
	vec3* UVs[3];

	xMaterial *material;

	vec3 faceCenter;
	vec3 faceNormal;
public :
	xFace(vec3 *v1, vec3 *v2, vec3 * v3, vec3* n1, vec3* n2, vec3 *n3, vec3 *uv1, vec3* uv2, vec3* uv3)
	{
		verts[0] = v1;
		verts[1] = v2;
		verts[2] = v3;
		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		UVs[0] = uv1;
		UVs[1] = uv2;
		UVs[2] = uv3;
	}

};
enum MeshType
{
	OBJ,
	TERRAIN,
	FBX,
	SPHERE,
	BASIC,
	CUBE,
	SKYBOX,
};

class Mesh : public Node
{
public:
	

	Mesh(MeshType mtype = BASIC);
	virtual ~Mesh();
	MeshType mtype;
	bool drawMesh(bool drawChildren = true, Mesh* drawSelected = NULL,bool drawAxes = false,bool mode = false);
	void setMeshType(MeshType mtype);
	void setName(char* name);
	Transform _transform;
	Transform &getTransform();
	

	void drawFace(xFace * face);
	int addVertex(vec3 vec);
	int addNormal(vec3 vec);
	int addTexCoord(vec3 vec);
	

	ObjectLoader* obj;
	Skybox* skybox;
	TerrainLoader *terrain;
	GLUquadricObj* quadObj;
	float radius;
	int stacks;
	int slices;
	void InitSphere(float radius, int stacks,int slices);
	void RenderSphere();
	int texID;


	vector<vec3 *>vertices;
	vector<vec3 *>normals;
	vector<vec3 *>UVs;
	vec3 *getVertices();
	vec3 *getNormals();
	vec3 *getUVs();
	vector<xFace*> faces;
	void addFace(vec3 v, vec3 n, vec3 uv);

	void deleteObjects();

public :

	int     numVertices;


};

#endif