#include "Mesh.h"
#include"Utilities.h"


Mesh::Mesh(MeshType  _name) :Node()
{
	mtype = _name;
	int texID = -1;
}
Mesh:: ~Mesh()
{

}
void Mesh::setMeshType(MeshType _mtype)
{
	mtype = _mtype;
}


void  Mesh::setName(char* _name)
{
	//name = _name;
}
void Mesh::InitSphere(float _radius, int _stacks, int _slices)
{
	radius = _radius;
		stacks = _stacks;
	slices = _slices;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricOrientation(quadObj, GLU_OUTSIDE);
	gluQuadricTexture(quadObj, GLU_TRUE);
}

void Mesh::RenderSphere()
{
	glPushAttrib(GL_ENABLE_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();


	// Rotate an additional 90 degrees around the local X axis
	glRotatef(90, 1.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	if (texID > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texID);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(quadObj, radius, slices, stacks);
	if (texID != -1)
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glPopAttrib();
}
void drawCube()
{


	glBegin(GL_QUADS);


	glColor3f(1.0f, 1.0f, 1.0f);
	//Front fact
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0f, -1.0f, 1.0f);

	//Backface
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0f, -1.0f, -1.0f);

	//top face
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0f, 1.0f, -1.0f);
	//Bottom face
	glNormal3f(0, -1, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3d(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0f, -1.0f, -1.0f);

	//Left Face
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(-1.0f, -1.0f, -1.0f);

	//Right
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0f, -1.0f, -1.0f);

	glEnd();
}
bool Mesh::drawMesh(bool drawChildren, Mesh* currentMesh, bool drawAxes,bool mode)
{
	bool isRender = false;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if (texID > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
	glDisable(GL_TEXTURE_2D);
	}
	glEnable(GL_LIGHTING);
	glTranslatef(_transform.position.x, _transform.position.y, _transform.position.z);
	glScalef(_transform.scale.x, _transform.scale.y, _transform.scale.z);
	glRotatef(_transform.rotation.x, 1, 0, 0);
	glRotatef(_transform.rotation.y, 0, 1, 0);
	glRotatef(_transform.rotation.z, 0, 0, 1);


	if (currentMesh == NULL || currentMesh == this)
	{
		if (drawAxes && XUtils->EnableAxes)
			XUtils->draw_axes();
		glPushName((int)this); //Testing 0x0650cde8 = 105958888


		if (mtype == OBJ)
		{
			obj->drawObject();
		}
		else if (mtype == TERRAIN)
		{
			glTranslatef(0, -8.0,0 );
			terrain->Render();
		}
		else if (mtype == CUBE)
		{
			drawCube();
		}
		else if (mtype == SKYBOX )
		{
			if (mode ==true)
			skybox->LoadSkybox();
		}
		else if (mtype == SPHERE)
		{
			RenderSphere();
		}
		else
		{
			for (int i = 0; i < faces.size(); i++)
			{

				drawFace(faces[i]);
			}
		}
		glPopName();
		isRender = true;
	}

	if (drawChildren == true)
	{
		for (list<Node*>::iterator it = children.begin(); it != children.end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh*>(*it);
			if (mesh != NULL)
				isRender = mesh->drawMesh(true, mesh, drawAxes) | isRender;
		}
	}
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glPopAttrib();
	

	return isRender;
}


Transform &Mesh::getTransform()
{
	return _transform;
}


void  Mesh::drawFace(xFace * face)

{
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; i++)
		{
			glTexCoord2f(face->UVs[i]->x, face->UVs[i]->y);
			glNormal3d(face->normals[i]->x, face->normals[i]->y, face->normals[i]->z);
			glVertex3d(face->verts[i]->x, face->verts[i]->y, face->verts[i]->z);
		}
		glEnd();
}

int  Mesh::addVertex(vec3 vec)
{
	vertices.push_back(new vec3(vec.x, vec.y, vec.z));
	return (vertices.size() - 1);
}

int  Mesh::addNormal(vec3 vec)
{
	normals.push_back(new vec3(vec.x, vec.y, vec.z));
	return (normals.size() - 1);
}
int  Mesh::addTexCoord(vec3 vec)

{
	UVs.push_back(new vec3(vec.x, vec.y,vec.z));
	return (UVs.size() - 1);

}

void Mesh::addFace(vec3 v, vec3 n, vec3 uv)
{
	faces.push_back(new xFace(vertices[v.x], vertices[v.y], vertices[v.z], normals[n.x], normals[n.y], normals[n.z], UVs[uv.x], UVs[uv.y], UVs[uv.z]));
}

vec3 *getVertices();
vec3 *getNormals();
vec3 *getUVs();
vector<xFace*> faces;