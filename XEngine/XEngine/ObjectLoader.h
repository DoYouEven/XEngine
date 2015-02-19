#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H
#ifndef VECTOR_H
#include "Vector3.h"
#endif // VECTOR_H


#include "Texture.h"

#ifndef GL_H
#define GL_H
#endif
#include <gl\GL.h>
#include <gl\GLU.h>
#include <sstream>
#include <vector>


using std::istringstream;
using std::vector;
struct Material
{
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];
	float Tr[4];
	float shininess;
	float alpha;
	float illum;

	Texture *ambientMap;
	Texture *diffuseMap;
	Texture *specularMap;
	Texture *emissionMap;
	Texture *shininessMap;
	Texture *transparencyMap;
	Texture *bumpMap;

	string name;

	Material()
	{
		float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultTransparency[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		memcpy(Ka, defaultAmbient, sizeof(float) * 4);
		memcpy(Kd, defaultDiffuse, sizeof(float) * 4);
		memcpy(Ks, defaultSpecular, sizeof(float) * 4);
		memcpy(Ke, defaultEmission, sizeof(float) * 4);
		memcpy(Tr, defaultTransparency, sizeof(float) * 4);
		ambientMap = NULL;
		diffuseMap = NULL;
		specularMap = NULL;
		emissionMap = NULL;
	shininessMap = NULL;
		transparencyMap = NULL;
	bumpMap = NULL;
	shininess = 2.0f;
	alpha = 1.0f;
	illum = 10.0f;

	}

};

struct Face
{
	Vector3 **verts;
	int numVerts;

	Vector3 **normals;
	int numNormals;

	Vector3 **UVs;
	int numUVs;

	Material *material;

	Vector3 faceCenter;
	Vector3 faceNormal;

	Face()
	{
		verts = NULL;
		normals = NULL;
		UVs = NULL;
		material = NULL;
	}
};

struct GroupObject
{
	vector<Face*>faces;
	string objectName;
	string groupName;
};
class ObjectLoader
{
public:

	ObjectLoader(string filename);
	~ObjectLoader();

	void  drawModel();
	void drawObject(bool transparency = false);

	void drawFace(Face &face);

	void deleteObjects();

	bool loadObject(string filename);
	void loadMaterial(string filename);

	float getRadius();

	Vector3 getCenter();
	string getPath();
	string filename;


private:
	
	vector<GroupObject*> objects;
	vector<Vector3 *>   verts;
	vector<Vector3 *>   normals;
	vector<Vector3 *>   UVs;
	vector<Material *> materials;

	Vector3 boundingPoints[8];

	float			radius;
	Vector3 center;
	bool isObjectLoaded;

	GLuint displayList;

};

#endif