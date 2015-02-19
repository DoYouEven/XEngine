
#ifndef _FBXLOADER_H_
#define _FBXLOADER_H_

#include <string>
using std::string;
#include "GetPosition.h"
#include "scene\geometry\fbxlayer.h"
#include "Vector3.h"
#include "Mesh.h"
#ifndef TEXTURE_H
#include "Texture.h"
#endif
#include "Light.h"
#include "fbxsdk.h"
#include "core\fbxmanager.h"
#include"Common.h"

struct Object
{
	string				name;
	vector<Object*>		children;

	Mesh				*mesh;
	double				*matrix;

	Object()
	{
		mesh = NULL;
		matrix = NULL;
	}
};

class FBXLoader
{
private:
	vector<Object*>			objects;
	vector<xMaterial*>		materials;

	string					filename;
	bool					objectLoaded;

	void loadSupportedTextures(FbxScene *pScene);
	void loadSupportedTexturesRecursive(FbxNode *pNode);

	void computeTransformation(FbxAMatrix &pGlobalPosition, FbxMesh *pMesh, FbxVector4 *pVertexArray, Mesh *mesh, FbxTime &pTime, FbxPose *pPose);

public:
	FBXLoader(string fname = "");
	virtual ~FBXLoader();

	void drawModel(long time);
	void drawObject(Object *object, long time);

	bool loadFBX(string fname);
	void loadContent(FbxScene *lScene, FbxNode *pNode, vector<Object*> *objects);
};

#endif