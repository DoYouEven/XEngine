
#include "ObjectLoader.h"



ObjectLoader::ObjectLoader(string _filename)
{
	isObjectLoaded = false;
	filename = _filename;
	if (_filename != "")
		loadObject(_filename);

	displayList = 0;
}
ObjectLoader::~ObjectLoader()
{
	deleteObjects();
}

void  ObjectLoader::drawModel()
{
	if (!isObjectLoaded)
		return;



	drawObject(false);

	//drawObject(true);
	glEndList();
	
}
void ObjectLoader::drawObject(bool transparency)
{
	Material * lastMaterial = NULL;
	for (int i = 0; i < objects.size(); i++)

	{

		GroupObject *object = objects[i];
		for (int n = 0; n < object->faces.size(); n++)
		{
			Face *face = object->faces[n];

			Material *material = face->material;

			if (material != lastMaterial)
			{
				if (transparency == false && material->alpha < 1.0f)
					continue;
				material->Kd[3] = material->alpha;
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)material->Ka);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)material->Kd);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)material->Ks);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)material->Ke);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,(GLfloat) material->shininess);

				if (material->diffuseMap != NULL)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, material->diffuseMap->texID);
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
				}
				lastMaterial = material;
			}
			drawFace(*face);
		}
	}

}

void ObjectLoader::drawFace(Face &face)
{

	if (face.numVerts <= 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_POLYGON);

	for (int v = 0; v < face.numVerts; v++)
	{
		if ((int)face.numUVs > v && face.UVs != NULL)
		{
			glTexCoord2f(face.UVs[v]->x, face.UVs[v]->y);
		}
		if ((int)face.numNormals> v && face.normals != NULL)
		{
			glNormal3d(face.normals[v]->x, face.normals[v]->y, face.normals[v]->z);
		}
		if ((int)face.numVerts> v && face.verts != NULL)
		{
			glVertex3d(face.verts[v]->x, face.verts[v]->y, face.verts[v]->z);
		}
	}
	glEnd();
}


void ObjectLoader::deleteObjects()
{
	for (int m = 0; m < materials.size(); m++)
	{
		delete materials[m];

		if (materials[m]->diffuseMap != NULL)
			delete materials[m]->diffuseMap;

		if (materials[m]->ambientMap != NULL)
			delete materials[m]->ambientMap;

		if (materials[m]->specularMap != NULL)
			delete materials[m]->specularMap;

		if (materials[m]->shininessMap != NULL)
			delete materials[m]->shininessMap;

		if (materials[m]->emissionMap != NULL)
			delete materials[m]->emissionMap;
		if (materials[m]->bumpMap != NULL)
			delete materials[m]->bumpMap;

		if (materials[m]->transparencyMap != NULL)
			delete materials[m]->transparencyMap;
	}
	
	for (int m = 0; m < verts.size(); m++)
		delete verts[m];
	for (int m = 0; m < normals.size(); m++)
		delete normals[m];
	for (int m = 0; m < UVs.size(); m++)
		delete UVs[m];
	for (int m = 0; m < objects.size(); m++)
	{
		GroupObject *object = objects[m];

		for (int f = 0; f < object->faces.size(); f++)
		{
			delete object->faces[f]->verts;
			delete object->faces[f]->normals;
			delete object->faces[f]->UVs;
			delete object->faces[f];
		}
	}

	UVs.clear();
	normals.clear();
	verts.clear();
	materials.clear();
	
}

void ObjectLoader::loadMaterial(string filename)
{
	ifstream istr(filename.data());

	if (!istr)
	{
		printf("filename invalid\n");
	}
	char path[256];
	strcpy_s(path, filename.data());

	for (int i = (int)filename.length(); path[i] != '\\' && path[i] != '/'; i--)
	{
		if (i < 0)
			break;
		path[i] = 0;
	}

	Material *material = NULL;

	char line[256];
	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);
		string firstWord;
		newLine >> firstWord;
		if (firstWord == "newmtl")
		{
			material = new Material;
			materials.push_back(material);
			newLine >> material->name;
			string test = material->name;
		}
		else if (firstWord == "illum")
		{
			newLine >> material->illum;
		}
		else if (firstWord == "Ka")
		{
			newLine >> material->Ka[0] >> material->Ka[1] >> material->Ka[2];
		}
		else if (firstWord == "Kd")
		{
			newLine >> material->Kd[0] >> material->Kd[1] >> material->Kd[2];
		}
		else if (firstWord == "Ks")
		{
			newLine >> material->Ks[0] >> material->Ks[1] >> material->Ks[2];
		}
		else if (firstWord == "Ke")
		{
			newLine >> material->Ke[0] >> material->Ke[1] >> material->Ke[2];
		}
		else if (firstWord == "Ns")
		{
			newLine >> material->shininess;
		}
		else if (firstWord == "d" || firstWord == "Tr")
		{
			newLine >> material->alpha;
		}
		else if (firstWord == "Tf")
		{

			Vector3 color;
			newLine >> color.x >> color.y >> color.z;
			material->alpha = (color.x + color.y + color.z) / 3.0f;
		}
		else if (firstWord == "map_Ka")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, TGA);
			material->ambientMap = map;
		}
		else if (firstWord == "map_Kd")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(filename, TGA);
			material->diffuseMap = map;
		}
		else if (firstWord == "map_ks")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(string(path) + filename, TGA);
			material->specularMap = map;
		}
		else if (firstWord == "map_ke")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(string(path) + filename, TGA);
			material->emissionMap = map;
		}
		else if (firstWord == "map_Ns")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(string(path) + filename, TGA);
			material->shininessMap = map;
		}
		else if (firstWord == "map_d")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, TGA);
			material->transparencyMap = map;
		}
		else if (firstWord == "bump")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(string(path) + filename, TGA);
			material->bumpMap = map;
		}



	}

}
bool ObjectLoader::loadObject(string filename)

{
	ifstream istr(filename.data());
	if (!istr)
	{
		printf("incorrect filename\n");
		return false;
	}

	deleteObjects();
	displayList = 0;
	isObjectLoaded = false;
	GroupObject *defaultObject = new GroupObject;

	GroupObject *currentGroup = defaultObject;
	objects.push_back(defaultObject);

	char path[256];
	strcpy_s(path, filename.data());
	
	for (int i = (int)filename.length(); path[i] != '\\' && path[i] != '/'; i--)
	{
		if (i < 0)
			break;
		path[i] = 0;
	}
	Material *currentMaterial = NULL;
	char line[256];
	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);

		string token;
		newLine >> token;

		if (token == "#")
		{


		}
		else if (token == "mtllib")
		{
			string matFilename;
			while (newLine >> matFilename)

			{
				loadMaterial(matFilename);
			}
			
		}

		else if (token == "usemtl")
		{
			string matName;
			newLine >> matName;

			for (int i = 0; i < materials.size(); i++)
			{
				if (materials[i]->name == matName)
				{
					currentMaterial = materials[i];
					break;
				}
			}
			

		}
		else if (token == "v")
		{
			Vector3 * vertex = new Vector3;
			newLine >> vertex->x >> vertex->y>> vertex->z;
			verts.push_back(vertex);

		}
		else if (token == "vt")
		{
			Vector3 * uvw= new Vector3;
			newLine >> uvw->x >> uvw->y >> uvw->z;
			UVs.push_back(uvw);

		}
		else if (token == "vn")
		{
			Vector3 * norm = new Vector3;
			newLine >> norm->x >> norm->y >> norm->z;
			normals.push_back(norm);
		}
		else if (token == "g")
		{
			string objectName;
			newLine >> objectName;
			if (objectName == "default")

			{
				currentGroup = defaultObject;
			}
			else

			{
				GroupObject *object = new GroupObject;
				object->objectName = objectName;
				newLine >> object->objectName;
				currentGroup = object;
				objects.push_back(object);
			}
		}
		else if (token == "f")
		{
			Face *newFace = new Face;
			newFace->material = currentMaterial;

			currentGroup->faces.push_back(newFace);

			vector<Vector3 *> tempVertices;
			vector<Vector3 *> tempNormals;
			vector<Vector3 *> tempUVs;

			while (!newLine.eof())
			{
				//if (newLine.peek() == '\n');
				//break;
				int norm = 0;
				int vert = 0;
				int uvw = 0;

				bool noUV = false;
				char temp;

				if ((int)verts.size() > 0)
				{
					newLine >> vert;

					while (newLine.get(temp))
						if (temp == '/' || temp == ' ') break;

					newLine.get(temp);
					if (temp == '/') noUV = true;
					else newLine.unget();

					if (vert - 1 < 0)
						break;
					tempVertices.push_back(verts[--vert]);
				}

				if ((int)UVs.size() > 0 && noUV == false)
				{
					newLine >> uvw;

					while (newLine.get(temp))
						if (temp == '/' || temp == ' ') break;

					tempUVs.push_back(UVs[--uvw]);
				}

				if ((int)normals.size() > 0)
				{
					newLine >> norm;

					while (newLine.get(temp))
						if (temp == '/' || temp == ' ') break;

					tempNormals.push_back(normals[--norm]);
				}
			}
			newFace->numVerts = (int)tempVertices.size();
			newFace->numNormals = (int)tempNormals.size();
			newFace->numUVs = (int)tempUVs.size();
			newFace->verts = new Vector3 *[newFace->numVerts];
			newFace->normals = new Vector3 *[newFace->numNormals];
			newFace->UVs = new Vector3 *[newFace->numUVs];

			for (int v = 0; v < newFace->numVerts; v++)
				newFace->verts[v] = tempVertices[v];
			for (int v = 0; v < newFace->numNormals; v++)
				newFace->normals[v] = tempNormals[v];
			for (int v = 0; v < newFace->numUVs; v++)
				newFace->UVs[v] = tempUVs[v];

			for (int v = 0; v < newFace->numVerts; v++)
				newFace->faceCenter += (*tempVertices[v]);

			newFace->faceCenter /= (float)newFace->numVerts;

			if (newFace->numVerts >= 3)

			{
				Vector3 vector1 = ((*newFace->verts[0]) - (*newFace->verts[1])).normalize();
				Vector3 vector2 = ((*newFace->verts[0]) - (*newFace->verts[1])).normalize();
				newFace->faceNormal = vector1 * vector2;
			}
	

		}

	
	}

	//Getmax
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
	center = Vector3(0, 0, 0);
	for (int n = 0; n < verts.size(); n++)
	{
		if (n == 0)
		{
			xmin = xmax = verts[n]->x;
			ymin = ymax = verts[n]->y;
			zmin = zmax = verts[n]->z;
		}
		if (verts[n]->x < xmin)
			xmin = verts[n]->x;
		if (verts[n]->x > xmax)
			xmax = verts[n]->x;

		if (verts[n]->y < ymin)
			ymin = verts[n]->x;
		if (verts[n]->y > ymax)
			ymax = verts[n]->x;

		if (verts[n]->z < zmin)
			zmin = verts[n]->z;
		if (verts[n]->z > zmax)
			zmax = verts[n]->z;
		
		center += *verts[n];
	}

	center /= (float)verts.size();

	boundingPoints[0] = Vector3(xmax, ymin, zmin);
	boundingPoints[1] = Vector3(xmin, ymax, zmin);
	boundingPoints[2] = Vector3(xmin, ymin, zmax);
	boundingPoints[3] = Vector3(xmin, ymax, zmax);
	boundingPoints[4] = Vector3(xmax, ymax, zmin);
	boundingPoints[5] = Vector3(xmax, ymin, zmax);
	boundingPoints[6] = Vector3(xmin, ymin, zmin);
	boundingPoints[7] = Vector3(xmax, ymax, zmax);
	
	
	radius = (Vector3(xmax, ymax, zmax) - Vector3(xmin, ymin, zmin)).magnitude() / 2.0f;

	isObjectLoaded = true;
	return true;
}



float ObjectLoader::getRadius()
{
	return radius;
}

Vector3 ObjectLoader::getCenter(){
	return center;
}
string ObjectLoader::getPath()
{
	return filename;
}