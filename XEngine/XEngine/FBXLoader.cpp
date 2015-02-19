/*
*  FBXLoader.cpp
*  FBX_Demo
*
*
*/

#include "FBXLoader.h"
#include "core\fbxmanager.h"

const float frameRate = 30.0f;

void MatrixScale(FbxAMatrix& pMatrix, double pValue)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pMatrix[i][j] *= pValue;
		}
	}
}


// Add a value to all the elements in the diagonal of the matrix.
void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
{
	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}


// Sum two matrices element by element.
void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pDstMatrix[i][j] += pSrcMatrix[i][j];
		}
	}
}
FBXLoader::FBXLoader(string fname)
{
	objectLoaded = false;

	filename = fname;

	if (!filename.empty())
		loadFBX(filename);
}

FBXLoader::~FBXLoader()
{
	for (int m = 0; m<(int)materials.size(); m++)
	{
		if (materials[m]->diffuseMap != NULL)
			delete materials[m]->diffuseMap;

		delete materials[m];
	}

	for (int i = 0; i<(int)objects.size(); i++)
	{
		delete objects[i];
	}
}

void FBXLoader::drawModel(long time)
{
	if (!objectLoaded) return;

	for (int i = 0; i<(int)objects.size(); i++)
	{
		Object *object = objects[i];
		drawObject(object, time);
	}
}

void FBXLoader::drawObject(Object *object, long time)
{
	static long lastTime = time;

	if (object == NULL) return;

	Mesh *mesh = object->mesh;

	if (time != lastTime)
	{
		mesh->frame += (float)(time - lastTime) / 1000.0f * frameRate;

		if (mesh->frame > (mesh->frameEnd - mesh->frameStart))
			mesh->frame = 0.0f;

		lastTime = time;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	FbxMatrix *lLinkDeformation = mesh->frames[(int)mesh->frame]->bones;
	double *lLinkWeight = mesh->frames[(int)mesh->frame]->weights;

	Vector3 *transformedVerts = new Vector3[mesh->numVertices];
	for (int i = 0; i<mesh->numVertices; i++)
	{
		FbxVector4 vertex(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
		FbxVector4 transformedVertex = vertex;

		double lWeight = lLinkWeight[i];

		if (lWeight != 0.0)
		{
			FbxMatrix matrix = lLinkDeformation[i];
			transformedVertex = matrix.MultT(vertex);
			transformedVertex += vertex * (1.0 - lWeight);
		}

		transformedVerts[i] = Vector3((float)transformedVertex[0], (float)transformedVertex[1], (float)transformedVertex[2]);
	}

	object->mesh->drawMesh(transformedVerts);
	delete[] transformedVerts;

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	for (int i = 0; i<(int)object->children.size(); i++)
	{
		drawObject(object->children[i], time);
	}
}

bool FBXLoader::loadFBX(string fname)
{
	bool result;
	FbxManager *manager;
	FbxScene *scene;

	manager = FbxManager::Create();
	if (manager == NULL)
		return false;

	scene = FbxScene::Create(manager, "");
	result = LoadScene(manager, scene, filename.data());
	if (result == false)
		return false;

	FbxNode *node = scene->GetRootNode();
	loadSupportedTextures(scene);

	if (node)
	{
		for (int i = 0; i < node->GetChildCount(); i++)
			loadContent(scene, node->GetChild(i), &objects);
	}

	objectLoaded = true;

	manager->Destroy();
}

void FBXLoader::loadContent(FbxScene *lScene, FbxNode *pNode, vector<Object*> *objects)
{
	Object *object = NULL;
	vector<Object*> *nextList = objects;

	if (pNode->GetNodeAttribute() == NULL)
	{
		// NULL NODE ATTRIBUTE
	}
	else
	{

		FbxNodeAttribute::EType lAttributeType = pNode->GetNodeAttribute()->GetAttributeType();

		switch (lAttributeType)
		{
		case FbxNodeAttribute::eLight:
		{
			Light *light = NULL;

			FbxLight *pLight = (FbxLight*)pNode->GetNodeAttribute();

			if (pLight->LightType.Get() == FbxLight::ePoint)
				light = new Light(POINT_LIGHT);
			else if (pLight->LightType.Get() == FbxLight::eDirectional)
				light = new Light(DIRECTIONAL_LIGHT);
			else if (pLight->LightType.Get() == FbxLight::eSpot)
				light = new Light(SPOT_LIGHT);

			if (pLight->LightType.Get() == FbxLight::eDirectional)
			{

				FbxMatrix direction = pNode->EvaluateGlobalTransform(FbxNode::eDestinationPivot);

				float x = (float)direction.Get(1, 0);
				float y = (float)direction.Get(1, 1);
				float z = (float)direction.Get(1, 2);
				light->setPosition(x, y, z);

			}
			else
			{
				FbxVector4 translate;

				pNode->EvaluateLocalTransform();

				float x = (float)translate[0];
				float y = (float)translate[1];
				float z = (float)translate[2];
				light->setPosition(x, y, z);
			}

			float intensity = (float)pLight->Intensity.Get();

			FbxColor lightColor;
			lightColor.mRed = pLight->Color.Get()[0] * intensity;
			lightColor.mGreen = pLight->Color.Get()[1] * intensity;
			lightColor.mBlue = pLight->Color.Get()[2] * intensity;

			light->setDiffuse((float)lightColor.mRed, (float)lightColor.mGreen, (float)lightColor.mBlue, 1.0f);
			break;
		}
		case FbxNodeAttribute::eMesh: // starts at frame 23530
		{
			object = new Object;
			object->mesh = NULL;

			object->matrix = new double[16];
			memcpy(object->matrix, (double*)pNode->EvaluateGlobalTransform(FbxNode::eDestinationPivot), sizeof(double) * 16);

			object->name = pNode->GetName();

			FbxMesh *pMesh = (FbxMesh*)pNode->GetNodeAttribute();

			xMaterial *material = NULL;

			FbxSurfaceMaterial *lMaterial = NULL;

			FbxLayerElementMaterial *lMaterialLayer = pMesh->GetLayer(0)->GetMaterials();

			material = new xMaterial();
			lMaterialLayer->GetDirectArray.GetCount();
			if (lMaterialLayer && lMaterialLayer->GetDirectArray.GetCount() > 0)
			{
				lMaterial = lMaterialLayer->GetDirectArray.GetAt(1);

				if (lMaterial)
				{

					FbxDouble3 lFbxDouble3;
					FbxColor diffuse, specular, ambient;

					if (lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
					{
						lFbxDouble3 = ((FbxSurfaceLambert *)lMaterial)->Diffuse;
						diffuse.Set(lFbxDouble3.Buffer()[0], lFbxDouble3.Buffer()[1], lFbxDouble3.Buffer()[2]);
						material->Kd[0] = (float)diffuse.mRed;
						material->Kd[1] = (float)diffuse.mGreen;
						material->Kd[2] = (float)diffuse.mBlue;

						lFbxDouble3 = ((FbxSurfaceLambert *)lMaterial)->Ambient;
						ambient.Set(lFbxDouble3.Buffer()[0], lFbxDouble3.Buffer()[1], lFbxDouble3.Buffer()[2]);
						material->Ka[0] = (float)ambient.mRed;
						material->Ka[1] = (float)ambient.mGreen;
						material->Ka[2] = (float)ambient.mBlue;
					}
					else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
					{
						lFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Diffuse;
						diffuse.Set(lFbxDouble3.Buffer()[0], lFbxDouble3.Buffer()[1], lFbxDouble3.Buffer()[2]);
						material->Kd[0] = (float)diffuse.mRed;
						material->Kd[1] = (float)diffuse.mGreen;
						material->Kd[2] = (float)diffuse.mBlue;

						lFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Specular;
						specular.Set(lFbxDouble3.Buffer()[0], lFbxDouble3.Buffer()[1], lFbxDouble3.Buffer()[2]);
						material->Ks[0] = (float)specular.mRed;
						material->Ks[1] = (float)specular.mGreen;
						material->Ks[2] = (float)specular.mBlue;

						lFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Ambient;
						ambient.Set(lFbxDouble3.Buffer()[0], lFbxDouble3.Buffer()[1], lFbxDouble3.Buffer()[2]);
						material->Ka[0] = (float)ambient.mRed;
						material->Ka[1] = (float)ambient.mGreen;
						material->Ka[2] = (float)ambient.mBlue;
					}
				}
			}

			FbxTexture *lTexture = NULL;

			FbxLayerElementTexture *lTextureLayer = pMesh->GetLayer(0)->GetTextures(FbxLayerElement::eTextureDiffuse);

			if (lTextureLayer && lTextureLayer->GetDirectArray().GetCount() > 0)
			{
				lTexture = lTextureLayer->GetDirectArray().GetAt(1);

				for (int i = 0; i < (int)Texture::textures.size(); i++)
				{
					if (Texture::textures[i]->name == lTexture->GetName())
					{
						material->diffuseMap = Texture::textures[i];
						break;
					}
				}
			}

			int lPolygonCount = pMesh->GetPolygonCount();

			Mesh *mesh = new Mesh();
			object->mesh = mesh;

			mesh->vertices = new Vector3[pMesh->GetControlPointsCount()];
			mesh->normals = new Vector3[pMesh->GetControlPointsCount()];
			mesh->UVs = new Vector3[pMesh->GetTextureUVCount()];

			mesh->numVertices = pMesh->GetControlPointsCount();

			FbxVector4 *lControlPoints = pMesh->GetControlPoints();
			for (int i = 0; i < mesh->numVertices; i++)
			{
				mesh->vertices[i] = Vector3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]);
			}

			FbxLayerElementNormal *lNormals = pMesh->GetLayer(0)->GetNormals();
			if (lNormals && lNormals->GetMappingMode() == FbxLayerElement::eByControlPoint)
			{
				for (int i = 0; i < mesh->numVertices; i++)
				{
					FbxVector4 lVector;
					if (lNormals->GetReferenceMode() == FbxLayerElement::eDirect)
					{
						lVector = lNormals->GetDirectArray().GetAt(i);
					}
					else
					{
						int index = lNormals->GetIndexArray().GetAt(i);
						lVector = lNormals->GetDirectArray().GetAt(index);
					}

					mesh->normals[i] = Vector3((float)lVector[0], (float)lVector[1], (float)lVector[2]);
				}
			}

			FbxLayerElementArrayTemplate<FbxVector2> *UVs = NULL;
			if (pMesh->GetTextureUV(&UVs, FbxLayerElement::eTextureDiffuse))
			{
				if (pMesh->GetLayer(0)->GetUVs()->GetMappingMode() == FbxLayerElement::eByControlPoint)
				{
					for (int i = 0; i < pMesh->GetTextureUVCount(); i++)
					{
						mesh->UVs[i] = Vector3((float)UVs->GetAt(i).mData[0], (float)UVs->GetAt(i).mData[1], 1.0f);
					}
				}
			}

			for (int i = 0; i < lPolygonCount; i++)
			{
				int lPolygonSize = pMesh->GetPolygonSize(i);

				// Triangulation Code
				for (int j = 1; j < lPolygonSize - 1; j++)
				{
					int index1 = pMesh->GetPolygonVertex(i, 0);
					int index2 = pMesh->GetPolygonVertex(i, j);
					int index3 = pMesh->GetPolygonVertex(i, j + 1);

					int uv1 = pMesh->GetTextureUVIndex(i, 0);
					int uv2 = pMesh->GetTextureUVIndex(i, j);
					int uv3 = pMesh->GetTextureUVIndex(i, j + 1);

					mesh->addFace(index1, index2, index3, index1, index2, index3, uv1, uv2, uv3, material);
				}
			}
			FbxString x;
		
			FbxArray<FbxString*> gTakeNameArray;
			lScene->FillPoseArray(gTakeNameArray);
			
				int numTakes = gTakeNameArray.GetCount();

			FbxTime gPeriod; 
			gPeriod.SetTime(0, 0, 0, 1, 0, lScene->GetGlobalSettings().GetTimeMode());
			for (int t = 2; t < numTakes; t++)
			{
				FbxTime gStart, gStop, currentTime;

				FbxTakeInfo *lCurrentTakeInfo = lScene->GetTakeInfo(*gTakeNameArray[t]);

				if (lCurrentTakeInfo)

					
					lScene->SetSceneInfo(gTakeNameArray[t]->Buffer());

				gStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
				gStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();

				FbxTime timeDiff = gStop - gStart;
				double diff = timeDiff.GetSecondDouble();

				int numFrames = (int)(diff * frameRate);

				mesh->frameStart = (float)(gStart.GetSecondDouble() * frameRate);
				mesh->frameEnd = (float)(gStop.GetSecondDouble() * frameRate);

				mesh->frame = mesh->frameStart;

				FbxPose *lPose = lScene->GetPose(0);

				for (int i = 0; i < numFrames; i++)
				{
					FbxAMatrix pGlobalPosition = GetGlobalPosition(pMesh->GetNode(), currentTime, lPose);

					computeTransformation(pGlobalPosition, pMesh, lControlPoints, mesh, currentTime, lPose);

					currentTime += gPeriod;
				}

				break;
			}
		}

			objects->push_back(object);

			nextList = &object->children;

			break;
		}
	}


	for (int i = 0; i<pNode->GetChildCount(); i++)
	{
		loadContent(lScene, pNode->GetChild(i), nextList);
	}
}

void FBXLoader::loadSupportedTextures(FbxScene *pScene)
{
	loadSupportedTexturesRecursive(pScene->GetRootNode());
}

void FBXLoader::loadSupportedTexturesRecursive(FbxNode *pNode)
{
	if (pNode)
	{
		FbxNodeAttribute *lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute)
		{
			FbxLayerContainer *lLayerContainer = NULL;

			switch (lNodeAttribute->GetAttributeType())
			{
			case FbxNodeAttribute::eNurbs:
			{
				lLayerContainer = pNode->GetNurbs();
				break;
			}
			case FbxNodeAttribute::ePatch:
			{
				lLayerContainer = pNode->GetPatch();
				break;
			}
			case FbxNodeAttribute::eMesh:
			{
				lLayerContainer = pNode->GetMesh();
				break;
			}
			}

			if (lLayerContainer && lLayerContainer->GetLayer(0))
			{
				FbxLayerElementTexture *lTextureLayer = lLayerContainer->GetLayer(0)->GetTextures(FbxLayerElement::eTextureDiffuse);

				if (lTextureLayer)
				{
					int lCount = lTextureLayer->GetDirectArray().GetCount();

					for (int x = 0; x<lCount; x++)
					{
						FbxTexture *lTexture = lTextureLayer->GetDirectArray().GetAt(x);
						
						string TextureFilename = lTexture->GetName();

						Texture *texture = NULL;

						bool loadTexture = true;

						for (int i = 0; i<(int)Texture::textures.size(); i++)
						{
							if (Texture::textures[i]->name == TextureFilename)
							{
								loadTexture = false;
								break;
							}
						}

						if (loadTexture)
						{
							texture = new Texture(TextureFilename);
							texture->name = lTexture->GetName();
						}
					}

				}
			}
		}
		int lCount = pNode->GetChildCount();

		for (int i = 0; i<lCount; i++)
			loadSupportedTexturesRecursive(pNode->GetChild(i));
	}
}

void FBXLoader::computeTransformation(FbxAMatrix &pGlobalPosition, FbxMesh *pMesh, FbxVector4 *pVertexArray, Mesh *mesh, FbxTime &pTime, FbxPose *pPose)
{
	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int i, j;
	int lClusterCount = 0;

	int lVertexCount = pMesh->GetControlPointsCount();

	FbxAMatrix* lClusterDeformation = new FbxAMatrix[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxMatrix));
	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (lClusterMode == FbxCluster::eAdditive)
	{
		for (i = 0; i < lVertexCount; i++)
		{
			lClusterDeformation[i].SetIdentity();
		}
	}

	for (i = 0; i<lSkinCount; ++i)
	{
		lClusterCount = ((FbxSkin *)pMesh->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (j = 0; j<lClusterCount; ++j)
		{
			FbxCluster* lCluster = ((FbxSkin *)pMesh->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
			if (!lCluster->GetLink())
				continue;
			 FbxAMatrix lReferenceGlobalInitPosition;
			FbxAMatrix lReferenceGlobalCurrentPosition;
			FbxAMatrix lClusterGlobalInitPosition;
			FbxAMatrix lClusterGlobalCurrentPosition;
			FbxAMatrix lReferenceGeometry;
			FbxAMatrix lClusterGeometry;

			FbxAMatrix lClusterRelativeInitPosition;
			FbxAMatrix lClusterRelativeCurrentPositionInverse;
			FbxAMatrix lVertexTransformMatrix;

			if (lClusterMode == FbxCluster::eAdditive && lCluster->GetAssociateModel())
			{
				lCluster->GetTransformAssociateModelMatrix(lReferenceGlobalInitPosition);
				lReferenceGlobalCurrentPosition = GetGlobalPosition(lCluster->GetAssociateModel(), pTime, pPose);

				// Geometric transform of the model
				lReferenceGeometry = GetGeometry(lCluster->GetAssociateModel());
				lReferenceGlobalCurrentPosition *= lReferenceGeometry;
			}
			else
			{
				lCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
				lReferenceGlobalCurrentPosition = pGlobalPosition;

				// Multiply lReferenceGlobalInitPosition by Geometric Transformation
				lReferenceGeometry = GetGeometry(pMesh->GetNode());
				lReferenceGlobalInitPosition *= lReferenceGeometry;
			}
			// Get the link initial global position and the link current global position.
			lCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			lClusterGlobalCurrentPosition = GetGlobalPosition(lCluster->GetLink(), pTime, pPose);

			// Compute the initial position of the link relative to the reference.
			lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			// Compute the current position of the link relative to the reference.
			lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

			// Compute the shift of the link relative to the reference.
			lVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;

			int k;
			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

			for (k = 0; k < lVertexIndexCount; ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];
				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
				{
					continue;
				}

				// Compute the influence of the link on the vertex.
				FbxAMatrix lInfluence = lVertexTransformMatrix;
				MatrixScale(lInfluence, lWeight);
				if (lClusterMode == FbxCluster::eAdditive)
				{
					// Multiply with to the product of the deformations on the vertex.
					MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxLink::eNORMALIZE || lLinkMode == FbxLink::eTOTAL1
				{
					// Add to the sum of the deformations on the vertex.
					MatrixAdd(lClusterDeformation[lIndex], lInfluence);

					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}

			}
		}
	}

	Frame *frame = new Frame();

	frame->bones = lClusterDeformation;
	frame->weights = lClusterWeight;

	mesh->frames.push_back(frame);
}


